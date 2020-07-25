// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#if defined(_MSC_VER)

#include "targetver.h"
#include "stdafx_windows.h" 

#pragma comment(lib, "IPCGate.lib")
#pragma comment(lib, "IPCInterface.lib")
#pragma comment(lib, "IPCClassFactory.lib")
#pragma comment(lib, "IPCObject.lib")

int EnumerateMyProcesses(); 

#else 

#include ".\..\Include\stdafx_OS9.h"

#endif




#ifdef max 
#undef max 
#endif 

#ifdef min 
#undef min 
#endif 

#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>


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
#include <queue>
#include <deque>

#if _MSC_VER > 1700
#include <unordered_map>
#endif
