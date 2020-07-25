// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once



#include "stdafx_windows.h" 

#ifdef max 
#undef max 
#endif 

#ifdef min 
#undef min 
#endif 

#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

#include <math.h>
#include <Rpc.h>
#include <Commdlg.h>

#include "shellapi.h"


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


#if _MSC_VER > 1700
#include <unordered_map>
#endif


#include "resource.h"
/*
template<typename T> 
T range_rand(T upper_boundary) { 
//	r is a random floating point value in the range [0,1) {including 0, not including 1}
	double r = ((double)rand() / ((double)(RAND_MAX) + 1.0)); 
//	returns a random number of type T in the range [0,M) {including 0, not including M} 
//	M - upper_boundary
	return (T)(r * upper_boundary); 
} 
*/

std::string itostdstring(int x); 
std::string filenameFromCommandline(); 
std::vector<std::string> argumentsFromCommandline(); 
std::string trim2stdstring(const std::string& str); 
__int64 IEEE754Double2Int(double f2Int, BYTE scale_by_exp2); 


#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPCClassFactory.lib")
#pragma comment(lib, "IPCInterface.lib")
#pragma comment(lib, "IPCGate.lib")
//#pragma comment(lib, "IPCCommon.lib")
#pragma comment(lib, "KWindows.lib")



