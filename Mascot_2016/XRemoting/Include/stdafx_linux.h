
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/io.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h> 


#define _w64 long long
#define _addrint(x) ((char*)(x) - (char*)0)

typedef unsigned char BYTE;
typedef unsigned char UCHAR; 
typedef char CHAR; 
typedef char* PCHAR; 
typedef signed short SHORT;
typedef unsigned short WORD; 
typedef unsigned long int DWORD; 
typedef float FLOAT;
typedef unsigned short USHORT;
typedef signed long int LONG;
typedef unsigned long int ULONG;
typedef unsigned long int BOOL;
typedef void VOID;
typedef void *PVOID;
typedef void *LPVOID;
typedef int HANDLE;
typedef PVOID LONG_PTR;
typedef DWORD *LPDWORD;
typedef int HWND;
typedef pthread_mutex_t CRITICAL_SECTION; 
typedef PVOID return_t; 


typedef signed char __int8; 
typedef signed short __int16;
typedef signed long int __int32; 
typedef signed long long __int64; 

typedef unsigned char __uint8; 
typedef unsigned short __uint16;
typedef unsigned long int __uint32; 
typedef unsigned long long __uint64; 


#define TRUE 1
#define FALSE 0
#ifndef __stdcall
#define __stdcall 
#endif
#define CALLBACK __stdcall 
#define WINAPI __stdcall 

#define TLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)

#define _atoi64 atoi
#define _timeb timeb
#define _ftime ftime

LONG InterlockedIncrement(LONG volatile* Addend); // imported from IPCGate.dll
LONG InterlockedDecrement(LONG volatile* Addend); // imported from IPCGate.dll

int GetLastError(); // imported from IPCGate.dll


#define EXPORTIPCINTERFACE
#define IMPORTIPCINTERFACE

#define EXPORTDLLVARIABLES 
#define IMPORTDLLVARIABLES 

