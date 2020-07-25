// Created by Alex Nikol Bourakov (ANB) around April/07

#if defined(_UCC) 
#include "..\..\Include\stdafx_OS9.h"
#include <fstream>

void IPCInterfaceDllMain(); 

#define NO_DEFAULT_COUT TRUE



#elif defined(__GCC__) 
#include "../../Include/stdafx_linux.h"
#include <unistd.h>
#include <dlfcn.h> 
#include <ext/stdio_filebuf.h>

void IPCInterfaceDllMain() __attribute__((constructor)); 

extern pid_t _g_processid; 
  
#define NO_DEFAULT_COUT FALSE



#elif defined(_MSC_VER)  
#include "..\..\Include\stdafx_windows.h" 

#pragma comment(lib, "IPCGate.lib")

#define EXP_STL

#define NO_DEFAULT_COUT FALSE


#elif defined(__BORLANDC__) 
#include "..\..\Include\stdafx_windows.h" 
#include <stdio.h>

#ifndef _w64
#define _w64
#endif 

#pragma comment(lib, "IPCGateBorland.lib")

#define NO_DEFAULT_COUT FALSE


#endif


#if !defined(_WIN32_WINDOWS) 
int GetLastError(); 
#endif 




