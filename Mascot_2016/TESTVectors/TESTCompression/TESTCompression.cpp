// TESTCompression.cpp : Defines the entry point for the application.
//

#include "TESTCompression_stdafx.h"


int ReadTextFile(const std::string& filename, std::string& inp_str); 
int WriteTextFile(const std::string& filename, const std::string& out_str); 


template<typename T> 
void ByteSwap(T* buffer, int iCount) { 
	for(int k = 0; k < iCount; ++k, ++buffer) { 
		PBYTE begin = (PBYTE)buffer; 
		PBYTE end = begin + sizeof(T) - 1; 
		while(end > begin) { 
			BYTE temp = *begin; 
			*begin++ = *end; 
			*end-- = temp; 
		} 
	} 
} 

void DisplayOpenFileError(const std::string& file_name) { 
	std::string msg = "cannot open the file:" + file_name; 
	MessageBoxA(0, msg.c_str(), "TESTCompression", MB_OK | MB_ICONEXCLAMATION); 
} 

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	std::string inpFileName = lpCmdLine; 
	std::string compressedFileName = "compressed" + inpFileName; 
	std::string decompressedFileName = "decompressed" + inpFileName; 

	std::string bin_data; 
	if(!ReadTextFile(inpFileName.c_str(), bin_data)) { 
		DisplayOpenFileError(inpFileName); 
	} 
	if(bin_data.size() == 0) { 
		return 0; 
	} 


	std::vector<short int> short_data; 
	short_data.resize(bin_data.size() / 2); 
	memcpy(short_data.data(), bin_data.data(), bin_data.size()); 

	ByteSwap(&short_data[0], (int)short_data.size()); 


	// C version

	std::vector<short int> buffer; 
	buffer.resize(short_data.size() * (4 / 3 + 1)); 

	int x = compress_enhanced_graphics(&short_data[0], (int)short_data.size(), &buffer[0]); 
	std::string streamed_values2; 
	streamed_values2.resize(x); 
	memcpy((void*)streamed_values2.data(), (void*)buffer.data(), x); 

	int work_buffer_size; 
	int traces_size; 
	getbuffersize2decompress_enhanced_graphics((char*)streamed_values2.data()/*in*/, &work_buffer_size/*out*/, &traces_size/*out*/); 

	buffer.resize(work_buffer_size); 

	std::vector<short int> output_data2; 
	output_data2.resize(traces_size); 

	decompress_enhanced_graphics((char*)streamed_values2.data(), (short int*)buffer.data(), (short int*)output_data2.data()); 

	ByteSwap(&output_data2[0], (int)output_data2.size()); 


	// c++ version

	std::string streamed_values; 
	compress_enhanced_graphics(short_data, streamed_values); 

	if(!WriteTextFile(compressedFileName, streamed_values)) { 
		DisplayOpenFileError(compressedFileName); 
	} 

	std::vector<short int> output_data; 

	decompress_enhanced_graphics(streamed_values, output_data); 

	ByteSwap(&output_data[0], (int)output_data.size()); 



	bin_data.resize(short_data.size() * 2); 
	memcpy((PVOID)bin_data.data(), output_data.data(), bin_data.size()); 

	if(!WriteTextFile(decompressedFileName, bin_data)) { 
		DisplayOpenFileError(compressedFileName); 
	} 

	int rc = 0; 
	if(output_data.size() != output_data2.size() || memcmp(&output_data[0], &output_data2[0], output_data2.size()) != 0) { 
		MessageBoxA(0, "Promblem in C implemtation", "TESTCompression", MB_OK | MB_ICONEXCLAMATION); 
		rc = 1; 
	} 

	return rc; 
}


