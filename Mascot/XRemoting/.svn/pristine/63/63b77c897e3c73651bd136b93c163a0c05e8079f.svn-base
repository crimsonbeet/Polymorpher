// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef osdependentstdafxH
#define osdependentstdafxH



#if defined(_UCC)
#include "..\..\Include\stdafx_OS9.h"

void IPCClassFactoryDllMain(); 


#elif defined(__GCC__) 
#include "../../Include/stdafx_linux.h" 

void IPCClassFactoryDllMain() __attribute__((constructor)); 


#elif defined(_MSC_VER) 
#include "..\..\Include\stdafx_windows.h" 

#define EXP_STL

#pragma comment(lib, "IPCGate.lib")
#pragma comment(lib, "IPCInterface.lib")



#elif defined(__BORLANDC__) 
#include "..\..\Include\stdafx_windows.h" 
#include <stdio.h>

#ifndef _w64
#define _w64
#endif

#pragma comment(lib, "IPCGateBorland.lib")
#pragma comment(lib, "IPCInterfaceBorland.lib")



#endif 



#endif //osdependentstdafxH

