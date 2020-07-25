// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma warning( disable:4996 )

#pragma once



#include "stdafx_windows.h" 

#include <SDKDDKVer.h>


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
#include <random>
#include <limits>


#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


#include "MASInterface.h"

#if _MSC_VER > 1700
#include <unordered_map>
#endif


#pragma comment(lib, "IPCClassFactory.lib")
#pragma comment(lib, "IPCInterface.lib")
#pragma comment(lib, "IPCGate.lib")
#pragma comment(lib, "KWindows.lib")


struct SCommandLineArguments { 
	short N;
	char*A[32]; 
}; 

int MyCreateDirectory(const std::string& dir_name); 
