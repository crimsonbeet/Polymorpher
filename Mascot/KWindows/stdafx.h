// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#include "..\Include\stdafx_windows.h" 

#ifndef GWL_WNDPROC
#define GWL_WNDPROC GWLP_WNDPROC
#define GWL_USERDATA GWLP_USERDATA
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
#include <stdarg.h>
#include <math.h>

#define YYERROR_VERBOSE
#define YYINCLUDED_STDLIB_H

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
#include <list>
#include <algorithm>
#include <functional>
#include <limits>

#if _MSC_VER > 1700
#include <unordered_map>
#endif

void PrintLastError(DWORD dwErr = 0);


#include "WSIClassFactoryInterfaces.h"

#pragma comment(lib, "IPCClassFactory.lib")
#pragma comment(lib, "IPCGate.lib")
#pragma comment(lib, "IPCInterface.lib")
#pragma comment(lib, "Msimg32.lib")


#define EXP_KWIN EXPORTIPCINTERFACE

