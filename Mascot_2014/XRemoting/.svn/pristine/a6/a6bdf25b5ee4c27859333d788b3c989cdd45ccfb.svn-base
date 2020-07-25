// Created by Alex Nikol Bourakov (ANB) around April/07

#if defined(_UCC) 
#include "..\..\Include\stdafx_OS9.h"
#include <UNIX\os9def.h>
#include <modes.h>
#include <sched.h>
#include <process.h>

void IPCGateDllMain(); 


#elif defined(__GCC__) 
#include "../../Include/stdafx_linux.h"
#include <sched.h>

void IPCGateDllMain() __attribute__((constructor)); 


#elif defined(_MSC_VER) 
#include "..\..\Include\stdafx_windows.h" 
#include <Mmsystem.h>



#elif defined(__BORLANDC__)
#include "..\..\Include\stdafx_windows.h"
#include <stdio.h>
#include <time.h>
#include <Mmsystem.h>

#define __w64 




#else 

???

#endif


#if !defined(_WIN32_WINDOWS) 
#endif 

