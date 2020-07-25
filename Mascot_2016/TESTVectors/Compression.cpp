#include <Windows.h>

#include <io.h>
#include <time.h>
#include <sys\timeb.h>
#include <tchar.h>

#include <ctime>
#include <ios>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <utility>
#include <stack>
#include <vector>
#include <algorithm>
#include <functional>
#include <limits>



void Daub_5_3_transform(short int *s0/*in*/, int s0_length, short int *s1/*out; must be at least s0_length/2 */, short int *d1/*out; must be at least s0_length/2 */) {
	int k;
	int n;
	int N = s0_length;
	int K = s0_length >> 1;

	--K;
	for(k = 0, n = 0; k < K; ++k, n += 2) {
		d1[k] = s0[n + 1] - (((int)s0[n] + s0[n + 2]) >> 1);
	}
	++K;

	// separate computation for the last element d1 because s0 uses symmetric extension
	// s0[N] = s0[N - 2];
	// s0[N + 1] = s0[N - 3];
	//d1[K - 1] = s0[N - 2] - (((int)s0[N - 1] + s0[N - 3]) >> 1);

	// d1 has to be zero for a line, so with (8,9,10,11) for (N-3,N-2,N-1,N) it has to be zero.
	// 10 - (9 + 11) / 2 = 10 - (9 + (9 + (9-8)*2)) / 2 = 10 - 9*2 + 8.
	d1[K - 1] = s0[N - 1] - (((int)s0[N - 2]) << 1) + s0[N - 3];

	// separate computation for the first element s1 because d1 uses symmetric extension
	// d1[-1] = d1[1];
	// d1[-2] = d1[2];
	s1[0] = s0[0] + (((int)d1[1] + d1[0]) >> 2);

	for(k = 1, n = 2; k < K; ++k, n += 2) {
		s1[k] = s0[n] + (((int)d1[k - 1] + d1[k]) >> 2);
	}
}


void Daub_5_3_inverse(short int *s1/*in; must be at least s0_length/2 */, short int *d1/*in; must be at least s0_length/2 */, short int *s0/*out*/, int s0_length) {
	int k;
	int n;
	int N = s0_length;
	int K = s0_length >> 1;

	// separate computation for the first element s0 because d1 uses symmetric extension
	// d1[-1] = d1[1];
	// d1[-2] = d1[2];
	s0[0] = s1[0] - (((int)d1[1] + d1[0]) >> 2);

	for(k = 1, n = 2; n < N; ++k, n += 2) {
		s0[n] = s1[k] - (((int)d1[k - 1] + d1[k]) >> 2);
	}

	--N;
	for(k = 0, n = 0; n < N; ++k, n += 2) {
		s0[n + 1] = d1[k] + (((int)s0[n] + s0[n + 2]) >> 1);
	}
	++N;

	// separate computation for the last element s1 because s0 uses symmetric extension
	// s0[N] = s0[N - 2];
	// s0[N + 1] = s0[N - 3];
	//s0[N - 1] = (((int)s0[N - 2] - d1[K - 1]) << 1) - s0[N - 3];

	s0[N - 1] = d1[K - 1] + (((int)s0[N - 2]) << 1) - s0[N - 3];
} 



/*
The following routine converts the transformed values to streamified array of bytes. 
It stores the data in a form of relative differences which is beneficial for torque and angle traces. 
The streamify() routine compresses really well the angle traces. 
Format: 
First entry is 2 bytes (short int) which is the very first absolute value. 
The entries that follow represent the differences. 
The value entry can have 1 or 2 bytes. The 8th bit of the first byte of an entry is always zero for the value entry. 
If the 8th bit is non zero, then the entry is one byte marker. 
When the marker has the 7th bit set, then the next value entries are 2 byte entries, otherwise 1 byte. 
When the remaining bits of the marker are non zero, then they represent a number of repetitions of the previous value. 
*/



size_t streamify(const std::vector<short int>& vTransformed_values, std::vector<char>& vStreamed_values) { 
	vStreamed_values.resize(vTransformed_values.size() * 3); 

	// first comes the byte with the sign. 

	vStreamed_values[0] = (char)((vTransformed_values[0] >> 8) & 0xff); 
	vStreamed_values[1] = (char)(vTransformed_values[0] & 0xff); 

	short int prev = vTransformed_values[0]; 
	short int prev_delta = 0; 
	char current_size = 0x40; // 2 - bytes; 7th bit.
	size_t s = 2; 
	for(size_t z = 1; z < vTransformed_values.size(); ++z) {
		short int delta = vTransformed_values[z] - prev; 
		char marker = 0; 
		if(delta == prev_delta) { 
			char x = 0; 
			while(delta == prev_delta && ++x < 63 && ++z < vTransformed_values.size()) { 
				delta = vTransformed_values[z] - (prev += prev_delta); 
			} 
			if(z < vTransformed_values.size()) { 
				prev = vTransformed_values[z]; 
			} 
			marker |= x; 
			marker |= 0x80; // indicate that marker is valid. 
		} 
		else { 
			prev = vTransformed_values[z]; 
		} 
		if(std::abs(delta) < 0x3f) { 
			if(current_size == 0x40) { 
				marker |= 0x80; // indicate that marker is valid. 
				current_size = 0; 
			} 
		} 
		else { 
			if(current_size == 0) { 
				marker |= 0x80; // indicate that marker is valid. 
				current_size = 0x40; 
			} 
		} 
		if(marker) { 
			marker |= current_size;
			vStreamed_values[s++] = marker; 
		} 
		if(delta != prev_delta) { 
			if(current_size == 0x40) { 
				vStreamed_values[s++] = (char)(delta >> 8) & 0x7f; 
			} 
			else { 
				delta &= 0x7f; 
			} 
			vStreamed_values[s++] = (char)(delta & 0xff); 
			prev_delta = delta; 
		} 
	} 
	vStreamed_values.resize(s); 
	return s; 
} 
int streamify(short int *transformed_values, int transformed_size, char *streamed_values) { 
	char *buffer = streamed_values; 
	short int prev = *transformed_values; 
	short int prev_delta = 0; 
	char current_size = 0x40; // 2 - bytes; 7th bit.
	int z; 

	// first comes the byte with the sign. 

	*buffer++ = (char)(((*transformed_values) >> 8) & 0xff); 
	*buffer++ = (char)((*transformed_values) & 0xff); 

	for(z = 1; z < transformed_size; ++z) {
		short int delta = transformed_values[z] - prev; 
		char marker = 0; 
		if(delta == prev_delta) { 
			char x = 0; 
			while(delta == prev_delta && ++x < 63 && ++z < transformed_size) { 
				delta = transformed_values[z] - (prev += prev_delta); 
			} 
			if(z < transformed_size) { 
				prev = transformed_values[z]; 
			} 
			marker |= x; 
			marker |= 0x80; // indicate that marker is valid. 
		} 
		else { 
			prev = transformed_values[z]; 
		} 
		if(abs(delta) < 0x3f) { 
			if(current_size == 0x40) { 
				marker |= 0x80; // indicate that marker is valid. 
				current_size = 0; 
			} 
		} 
		else { 
			if(current_size == 0) { 
				marker |= 0x80; // indicate that marker is valid. 
				current_size = 0x40; 
			} 
		} 
		if(marker) { 
			marker |= current_size;
			*buffer++ = marker; 
		} 
		if(delta != prev_delta) { 
			if(current_size == 0x40) { 
				*buffer++ = (char)(delta >> 8) & 0x7f; 
			} 
			else { 
				delta &= 0x7f; 
			} 
			*buffer++ = (char)(delta & 0xff); 
			prev_delta = delta; 
		} 
	} 
	return buffer - streamed_values; 
} 

inline short int destreamify_short(const char *streamed_value) { 
	short int v = ((short int)*streamed_value++) << 8; 
	v |= ((short int)*streamed_value) & 0xff; 
	if((v & 0x4000) != 0) { 
		v |= 0x8000; 
	} 
	return v; 
} 
inline short int destreamify_byte(const char *streamed_value) { 
	short int v = ((short int)*streamed_value) & 0xff; 
	if((v & 0x40) != 0) { 
		v |= 0xff80; 
	} 
	return v; 
} 
size_t destreamify(const std::vector<char>& vStreamed_values, std::vector<short int>& vTransformed_values) { 
	short int prev_delta = 0; 
	short int prev = ((short int)vStreamed_values[0]) << 8; 
	prev |= ((short int)vStreamed_values[1]) & 0xff; 

	vTransformed_values.resize(0); 
	vTransformed_values.push_back(prev); 
	
	char current_size = 0x40; // 2 - bytes; 7th bit.
	for(size_t s = 2; s < vStreamed_values.size(); ++s) { 
		int x = vTransformed_values.size();
		char marker = vStreamed_values[s]; 
		if((marker & 0x80) != 0) { 
			current_size = marker & 0x40; 
			size_t x = marker & 0x3f; 
			while(x--) { 
				vTransformed_values.push_back(prev += prev_delta); 
			} 
		} 
		else { 
			switch(current_size) { 
			case 0x40: 
				prev += (prev_delta = destreamify_short(&vStreamed_values[s++])); 
				break; 
			case 0: 
				prev += (prev_delta = destreamify_byte(&vStreamed_values[s])); 
				break; 
			} 
			vTransformed_values.push_back(prev); 
		} 
	} 
	return vTransformed_values.size(); 
} 

void apply_zerothreshold(short int *begin, short int *end) { 
	while(begin < end) { 
		if(*begin) { 
			if(abs(*begin) <= 1) { 
				*begin = 0; 
			} 
		} 
		++begin; 
	} 
} 

/*
compress_enhanced_graphics() routine creates 3 streamified arrays out of incoming enhanced graphics array. 
It concatenates those streamified arrays together, so the output array contains a compressed image of the input array. 
Each streamified array is prepended with its size. 
The very first entry is <short int> value that represents the original size of torque data (3 times less than original total size).
*/

void compress_enhanced_graphics(std::vector<short int>& short_data, std::string& vStreamed_values) { 
	std::vector<short int> vRaw_values; 
	std::vector<short int> vTransformed_values; 

	size_t nSize = short_data.size() / 3; 
	size_t nRSize = (nSize & 1)? (nSize + 1): nSize; 

	vRaw_values.resize(nRSize); 
	vTransformed_values.resize(nRSize); 

	vStreamed_values.reserve(nSize); 
	vStreamed_values.resize(2); 
	memcpy(&vStreamed_values[0], &nRSize, 2); 

	for(size_t i = 0; i < 3; ++i) { 
		for(size_t x = i, z = 0; x < short_data.size(); x += 3, ++z) { 
			vRaw_values[z] = short_data[x]; 
		} 
		if(nSize < nRSize) { 
			vRaw_values[nSize] = vRaw_values[nSize - 1]; 
		} 
		Daub_5_3_transform(&vRaw_values[0], nRSize, &vTransformed_values[0], &vTransformed_values[nRSize >> 1]);
		apply_zerothreshold(&vTransformed_values[0] + (nRSize >> 1), &vTransformed_values[0] + (nRSize - (nRSize > nSize? 4: 2))); 

		std::vector<char> vValues; 
		size_t length = streamify(vTransformed_values, vValues); 

		size_t offset = vStreamed_values.size(); 
		vStreamed_values.resize(offset + 2 + length); 

		memcpy(&vStreamed_values[offset], &length, 2); 
		memcpy(&vStreamed_values[offset + 2], vValues.data(), length); 
	} 
} 

/*
compress_enhanced_graphics() routine creates 3 streamified arrays out of incoming enhanced graphics array. 
It concatenates those streamified arrays together, so the output array contains a compressed image of the input array. 
Each streamified array is prepended with its size. 
The very first entry is <short int> value that represents the original size of torque data (3 times less than original total size).

It returns the size of streamed data in bytes. 
The streamed data will be at the address pointed to by the buffer parameter. 
*/
int compress_enhanced_graphics(short int *traces, int traces_size/*in short ints*/, short int *buffer/*has to be at least 4/3 + 1 of traces_size*/) { 
	char *streamed_traces = (char*)buffer; 
	int s = 0; 

	int nSize = traces_size / 3; 
	int nRSize = (nSize & 1)? (nSize + 1): nSize; 

	int i; 
	for(i = 0; i < 3; ++i) { 
		int x; 
		int z; 
		short int *transformed = buffer + nRSize; 
		for(x = i, z = 0; x < traces_size; x += 3, ++z) { 
			buffer[z] = traces[x]; 
		} 
		if(nSize < nRSize) { 
			buffer[nSize] = traces[nSize - 1]; 
		} 
		Daub_5_3_transform(buffer, nRSize, transformed, transformed + (nRSize >> 1)); 
		apply_zerothreshold(transformed + (nRSize >> 1), transformed + (nRSize - (nRSize > nSize? 4: 2))); 

		x = streamify(transformed, nRSize, streamed_traces + 2); 
		memcpy(streamed_traces, &x, 2); 
		x += 2; 

		streamed_traces += x; 
		s += x; 
		buffer += x >> 1; 

		while((char*)buffer < streamed_traces) ++buffer; 
	} 
	return s; 
} 






void decompress_enhanced_graphics(std::string& vStreamed_values, std::vector<short int>& short_data) { 
	std::vector<short int> vRaw_values; 
	std::vector<short int> vDestreamified_values; 

	size_t nRSize = 0; 
	memcpy(&nRSize, &vStreamed_values[0], 2); 

	vRaw_values.resize(nRSize); 
	vDestreamified_values.resize(nRSize); 

	short_data.resize(nRSize * 3); 

	size_t offset = 2; 
	std::vector<char> vValues; 

	for(size_t i = 0; i < 3; ++i) { 
		size_t length = 0; 
		memcpy(&length, &vStreamed_values[offset], 2); 
		offset += 2; 

		vValues.resize(length); 
		memcpy(vValues.data(), &vStreamed_values[offset], length); 
		offset += length; 

		destreamify(vValues, vDestreamified_values); 
		Daub_5_3_inverse(&vDestreamified_values[0],  &vDestreamified_values[nRSize >> 1], &vRaw_values[0], nRSize); 

		for(size_t x = i, z = 0; z < vRaw_values.size(); x += 3, ++z) { 
			short_data[x] = vRaw_values[z]; 
		} 
	} 
} 



// Encription


void Daub_5_3_transform(char *s0/*in*/, int s0_length, char *s1/*out; must be at least s0_length/2 */, char *d1/*out; must be at least s0_length/2 */) {
	int k;
	int n;
	int N = s0_length;
	int K = s0_length >> 1;

	--K;
	for(k = 0, n = 0; k < K; ++k, n += 2) {
		d1[k] = s0[n + 1] - (((int)s0[n] + s0[n + 2]) >> 1);
	}
	++K;

	// separate computation for the last element d1 because s0 uses symmetric extension
	// s0[N] = s0[N - 2];
	// s0[N + 1] = s0[N - 3];
	//d1[K - 1] = s0[N - 2] - (((int)s0[N - 1] + s0[N - 3]) >> 1);

	// d1 has to be zero for a line, so with (8,9,10,11) for (N-3,N-2,N-1,N) it has to be zero.
	// 10 - (9 + 11) / 2 = 10 - (9 + (9 + (9-8)*2)) / 2 = 10 - 9*2 + 8.
	d1[K - 1] = s0[N - 1] - (((int)s0[N - 2]) << 1) + s0[N - 3];

	// separate computation for the first element s1 because d1 uses symmetric extension
	// d1[-1] = d1[1];
	// d1[-2] = d1[2];
	s1[0] = s0[0] + (((int)d1[1] + d1[0]) >> 2);

	for(k = 1, n = 2; k < K; ++k, n += 2) {
		s1[k] = s0[n] + (((int)d1[k - 1] + d1[k]) >> 2);
	}
}





void Daub_5_3_inverse(char *s1/*in; must be at least s0_length/2 */, char *d1/*in; must be at least s0_length/2 */, char *s0/*out*/, int s0_length) {
	int k;
	int n;
	int N = s0_length;
	int K = s0_length >> 1;

	// separate computation for the first element s0 because d1 uses symmetric extension
	// d1[-1] = d1[1];
	// d1[-2] = d1[2];
	s0[0] = s1[0] - (((int)d1[1] + d1[0]) >> 2);

	for(k = 1, n = 2; n < N; ++k, n += 2) {
		s0[n] = s1[k] - (((int)d1[k - 1] + d1[k]) >> 2);
	}

	--N;
	for(k = 0, n = 0; n < N; ++k, n += 2) {
		s0[n + 1] = d1[k] + (((int)s0[n] + s0[n + 2]) >> 1);
	}
	++N;

	// separate computation for the last element s1 because s0 uses symmetric extension
	// s0[N] = s0[N - 2];
	// s0[N + 1] = s0[N - 3];
	//s0[N - 1] = (((int)s0[N - 2] - d1[K - 1]) << 1) - s0[N - 3];

	s0[N - 1] = d1[K - 1] + (((int)s0[N - 2]) << 1) - s0[N - 3];
}


const char* const DEC2HEX = "0123456789ABCDEF";

char HEX2DEC(char hex) {
	switch(hex) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			return hex - '0';
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
			return (hex - 'a') + 10;
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
			return (hex - 'A') + 10;
	}
	return 0;
}


void hex2string(char *hex_image, char *str, int license_len) {
	int x = license_len - 2;
	int y = 1;
	int z = 0;
	while(y < license_len) {
		if((z++ & 1) == 1) {
			*str++ = (char)(((int)HEX2DEC(hex_image[y])) << 4) | ((int)HEX2DEC(hex_image[x]));
		}
		else {
			*str++ = (char)(((int)HEX2DEC(hex_image[x])) << 4) | ((int)HEX2DEC(hex_image[y]));
		}
		x -= 2;
		y += 2;
	}
	*str = 0;
}

void string2hex(char *str, char *hex_image, int license_len) {
	int x = 2 * license_len - 2;
	int y = 1;
	int z = 0;
	hex_image[x + 2] = 0;

	while(license_len-- > 0) {
		int c = (int)(*str++) & 0xff;
		if((z++ & 1) == 1) {
			hex_image[y] = DEC2HEX[(c >> 4) & 0x0f];
			hex_image[x] = DEC2HEX[c & 0x0f];
		}
		else {
			hex_image[x] = DEC2HEX[(c >> 4) & 0x0f];
			hex_image[y] = DEC2HEX[c & 0x0f];
		}
		x -= 2;
		y += 2;
	}
}



extern "C" __declspec(dllexport)
void __stdcall DecriptString(char *str, char *decripted) {
	if(str == NULL) {
		return;
	}

	size_t raw_length = strlen(str);
	if(raw_length == 0) {
		return;
	}
	if((raw_length & 1) == 1) {
		return;
	}

	std::vector<char> vStreamed_values;
	vStreamed_values.reserve((raw_length >> 1) + 1);
	vStreamed_values.resize(raw_length >> 1); 
	hex2string(str, &vStreamed_values[0], raw_length);

	std::vector<short int> vTransformed_values;
	destreamify(vStreamed_values, vTransformed_values);

	std::string encripted;
	encripted.resize(vTransformed_values.size());
	char *ptr = (char*)encripted.data();
	for(auto v : vTransformed_values) {
		*ptr++ = (char)(v & 0xff);
	}

	int str_length = vTransformed_values.size(); 
	Daub_5_3_inverse((char*)encripted.data(), (char*)encripted.data() + (str_length >> 1), decripted, str_length);
	decripted[str_length] = 0; 
}


int range_rand(int upper_boundary) {
//	r is a random floating point value in the range [0,1) {including 0, not including 1}
double r = ((double)rand() / ((double)(RAND_MAX) + 1.0));
//	returns a random number of type T in the range [0,M) {including 0, not including M}
//	M - upper_boundary
return (int)(r * upper_boundary);
}

extern "C" __declspec(dllexport)
void __stdcall EncriptString(char *str, char *encripted) {
	if(str == NULL) {
		return;
	}

	size_t str_length = strlen(str);
	if(str_length == 0) {
		return;
	}

	std::string raw_str;
	if((str_length & 1) == 1) {
		raw_str.reserve(++str_length);
		raw_str = str;
		raw_str.resize(str_length);
		raw_str[str_length - 1] = 0;
	}
	else {
		raw_str = str;
	}

	std::string transformed;
	transformed.resize(str_length);
	Daub_5_3_transform(&raw_str[0], str_length, &transformed[0], (char*)transformed.data() + (str_length >> 1));

	std::vector<short int> vTransformed_values;
	vTransformed_values.resize(str_length);
	std::vector<short int>::iterator it = vTransformed_values.begin();
	for(auto c : transformed) {
		*(it++) = ((short int)range_rand(256) << 8) | (short int)c;
	}

	std::vector<char> vStreamed_values;
	streamify(vTransformed_values, vStreamed_values);

	string2hex(&vStreamed_values[0], encripted, (int)vStreamed_values.size());
}

