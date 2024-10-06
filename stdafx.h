// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma warning( disable:4996 4244 4267 4275)

#pragma once

#define WIN32_LEAN_AND_MEAN

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

#include <SYS\TIMEB.H>

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
#include <unordered_map>
#include <set>
#include <utility>
#include <stack>
#include <vector>
#include <algorithm>
#include <functional>
#include <limits>
#include <chrono>

#include <stdio.h>
#include <tchar.h>


#include "MASInterface.h"
#include "IPCGate.h"



struct SCommandLineArguments { 
	short N;
	char*A[32]; 
}; 

int MyCreateDirectory(const std::string& dir_name); 
