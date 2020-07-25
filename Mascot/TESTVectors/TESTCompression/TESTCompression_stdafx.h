// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
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


/*
C++ - CompressionEx.cpp
*/

void compress_enhanced_graphics(std::vector<short int>& short_data, std::string& vStreamed_values);
void decompress_enhanced_graphics(std::string& vStreamed_values, std::vector<short int>& short_data);


/*
C - Compression.c
*/


#ifdef  __cplusplus
extern "C" {
#endif

short int destreamify_short(const char *streamed_value); 
short int destreamify_byte(const char *streamed_value); 
void apply_zerothreshold(short int *begin, short int *end); 


void Daub_5_3_transform(short int *s0/*in*/, int s0_length, short int *s1/*out; must be at least s0_length/2 */, short int *d1/*out; must be at least s0_length/2 */);
void Daub_5_3_inverse(short int *s1/*in; must be at least s0_length/2 */, short int *d1/*in; must be at least s0_length/2 */, short int *s0/*out*/, int s0_length);


int compress_enhanced_graphics(short int *traces, int traces_size/*in short ints*/, short int *buffer/*has to be at least 4/3 + 1 of traces_size*/); 
int getbuffersize2decompress_enhanced_graphics(char *streamed_data/*in*/, int *work_buffer_size/*out*/, int *traces_size/*out*/);
int decompress_enhanced_graphics(char *buffer, short int *work_buffer, short int *traces); 

#ifdef  __cplusplus
} 
#endif


