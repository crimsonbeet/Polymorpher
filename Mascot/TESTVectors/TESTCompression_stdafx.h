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


void Daub_5_3_transform(short int *s0/*in*/, int s0_length, short int *s1/*out; must be at least s0_length/2 */, short int *d1/*out; must be at least s0_length/2 */);
void Daub_5_3_inverse(short int *s1/*in; must be at least s0_length/2 */, short int *d1/*in; must be at least s0_length/2 */, short int *s0/*out*/, int s0_length);
size_t streamify(const std::vector<short int>& vTransformed_values, std::vector<char>& vStreamed_values);
size_t destreamify(const std::vector<char>& vStreamed_values, std::vector<short int>& vTransformed_values);
void compress_enhanced_graphics(std::vector<short int>& short_data, std::string& vStreamed_values);
void decompress_enhanced_graphics(std::string& vStreamed_values, std::vector<short int>& short_data);

int compress_enhanced_graphics(short int *traces, int traces_size/*in short ints*/, short int *buffer/*has to be at least 4/3 + 1 of traces_size*/); 

int ReadTextFile(const std::string& filename, std::string& inp_str); 
int WriteTextFile(const std::string& filename, const std::string& out_str); 

