// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

// Created by Alex Nikol Bourakov (ANB) around April/07

#pragma once


#include ".\OSSpecific\OSDependent_ipcobject.h"

#ifdef max 
#undef max 
#endif 

#ifdef min 
#undef min 
#endif 

#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

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
#include <queue>
#include <algorithm>
#include <functional>

#if _MSC_VER > 1700
#include <unordered_map>
#endif




#include ".\OSSpecific\OSDependent_Sockaddr_in.h"

#include ".\..\Include\IPCGate.h"
#include ".\..\IPCGate\ThreadPool.h"
#include ".\..\Include\WSIClassFactoryInterfaces.h"
#include ".\..\IPCInterface\IPCLogTemplates.h"
#include ".\..\Include\IPCInterface.h"


extern int _g_trace; 


// imported from IPCInterface.dll
void IMPORTIPCINTERFACE __stdcall IPCObjectSetInterface(IPCObjInterface*);

BOOL InitializeIPCInterface();
BOOL InitializeSOCKInterface();

// imported from IPCInterface.dll
BOOL IMPORTIPCINTERFACE __stdcall OUTPUTLogMessage(const std::string& msg);


//#define WSI_USE_EXPLICIT_SOCKET_LOCK

