
#include "TESTCompression_stdafx.h"


/*
The following routine converts the transformed values to streamified array of bytes. 
It stores the data in a form of relative differences which is beneficial for torque and angle traces. 
The streamify() routine compresses realy well the angle traces. 
Format: 
First entry is 2 bytes (short int) which is the very first absolute value. 
The entries that follow represent the differences. 
The value entry can have 1 or 2 bytes. The 8th bit of the first byte of an entry is always zero for the value entry. 
If the 8th bit is non zero, then the entry is one byte marker. 
When the marker has the 7th bit set, then the next value entries are 2 byte entries, otherwise 1 byte. 
When the remaining bits of the marker are non zero, then they represent a number of repetitions of the value that was before. 
*/



size_t streamify(const std::vector<short int>& vTransformed_values, std::vector<char>& vStreamed_values) { 
	vStreamed_values.resize(vTransformed_values.size()); 

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

size_t destreamify(const std::vector<char>& vStreamed_values, std::vector<short int>& vTransformed_values) { 
	short int prev_delta = 0; 
	short int prev = ((short int)vStreamed_values[0]) << 8; 
	prev |= ((short int)vStreamed_values[1]) & 0xff; 

	vTransformed_values.resize(0); 
	vTransformed_values.push_back(prev); 
	
	char current_size = 0x40; // 2 - bytes; 7th bit.
	for(size_t s = 2; s < vStreamed_values.size(); ++s) { 
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
	memcpy(&vStreamed_values[0], &nSize, 2); 

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






void decompress_enhanced_graphics(std::string& vStreamed_values, std::vector<short int>& short_data) { 
	std::vector<short int> vRaw_values; 
	std::vector<short int> vDestreamified_values; 

	size_t nSize = 0; 
	memcpy(&nSize, &vStreamed_values[0], 2); 
	size_t nRSize = (nSize & 1)? (nSize + 1): nSize; 

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

	short_data.resize(nSize * 3); 
} 

