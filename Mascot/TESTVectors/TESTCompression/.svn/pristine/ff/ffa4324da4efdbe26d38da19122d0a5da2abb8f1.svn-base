#include <Windows.h>

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

short int destreamify_short(const char *streamed_value) { 
	short int v = ((short int)*streamed_value++) << 8; 
	v |= ((short int)*streamed_value) & 0xff; 
	if((v & 0x4000) != 0) { 
		v |= 0x8000; 
	} 
	return v; 
} 
short int destreamify_byte(const char *streamed_value) { 
	short int v = ((short int)*streamed_value) & 0xff; 
	if((v & 0x40) != 0) { 
		v |= 0xff80; 
	} 
	return v; 
} 
int destreamify(const char *vStreamed_values, int streamed_length, short int *vTransformed_values) { 
	short int *buffer = vTransformed_values; 
	short int prev_delta = 0; 
	short int prev = ((short int)vStreamed_values[0]) << 8; 
	char current_size = 0x40; // 2 - bytes; 7th bit.
	int s; 

	prev |= ((short int)vStreamed_values[1]) & 0xff; 

	*vTransformed_values++ = prev; 
	
	for(s = 2; s < streamed_length; ++s) { 
		char marker = vStreamed_values[s]; 
		if((marker & 0x80) != 0) { 
			int x = marker & 0x3f; 
			current_size = marker & 0x40; 
			while(x--) { 
				*vTransformed_values++ = (prev += prev_delta); 
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
			*vTransformed_values++ = prev; 
		} 
	} 
	return vTransformed_values - buffer; 
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

It returns the size of streamed data in bytes. 
The streamed data will be at the address pointed to by the buffer parameter. 
*/
int compress_enhanced_graphics(short int *traces, int traces_size/*in (short int)s*/, short int *buffer/*has to be at least 4/3 of traces_size*/) { 
	char *streamed_traces = (char*)buffer; 
	int s = 0; 
	int i; 

	int nSize = traces_size / 3; 
	int nRSize = (nSize & 1)? (nSize + 1): nSize; 

	memcpy(streamed_traces, &nSize, 2); 
	streamed_traces += (s = 2); 
	buffer += 1; 

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






int getbuffersize2decompress_enhanced_graphics(char *streamed_data/*in*/, int *work_buffer_size/*out*/, int *traces_size/*out*/) { 
	short int nSize = 0; 
	int nRSize = 0; 
	memcpy(&nSize, streamed_data, 2); 
	if(nSize < 0) { 
		nSize = 0; 
	} 
	nRSize = (nSize & 1)? (nSize + 1): nSize; 

	*work_buffer_size = nRSize * 2; 
	*traces_size = nSize * 3; 

	return nSize; 
} 

int decompress_enhanced_graphics(char *buffer, short int *work_buffer, short int *traces) { 
	short int nSize = 0; 
	int nRSize = 0; 
	int i; 
	int z; 
	int x; 

	memcpy(&nSize, buffer, 2); 
	if(nSize < 0) { 
		nSize = 0; 
	} 
	nRSize = (nSize & 1)? (nSize + 1): nSize; 

	buffer += 2; 

	for(i = 0; i < 3; ++i) { 
		short int *detransformed_data = work_buffer + nRSize; 
		short int streamed_length = 0; 

		memcpy(&streamed_length, buffer, 2); 
		buffer += 2; 

		destreamify(buffer, streamed_length, work_buffer); 
		buffer += streamed_length; 
		Daub_5_3_inverse(work_buffer, work_buffer + (nRSize >> 1), detransformed_data, nRSize); 

		for(x = i, z = 0; z < nSize; x += 3, ++z) { 
			traces[x] = detransformed_data[z]; 
		} 
	} 
	return nSize * 3; 
} 
