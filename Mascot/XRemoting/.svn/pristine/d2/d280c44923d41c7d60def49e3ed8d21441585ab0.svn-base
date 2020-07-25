#ifndef STDAFX_OS9_h
#define STDAFX_OS9_h

#define _OPT_PROTOS
#define __LONGLONG_BIT 64

#ifdef DEBUG
#undef DEBUG
#endif

#include <types.h>
#include <modes.h>
#include <stdio.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>


#define __w64 long

typedef unsigned char BYTE;
typedef unsigned char UCHAR;
typedef char CHAR;
typedef char* PCHAR;
typedef char* PTCHAR;
typedef signed short SHORT;
typedef unsigned long int DWORD;
typedef float FLOAT;
typedef unsigned short USHORT;
typedef signed short SSHORT;
typedef signed long int LONG;
typedef unsigned long int ULONG;
typedef unsigned char UBYTE;
typedef UBYTE* PBYTE;
typedef unsigned short UWORD;
typedef signed char SCHAR;
typedef unsigned long int BOOL;
#if defined(__cplusplus)
typedef unsigned short WORD;
typedef void VOID;
typedef void *PVOID;
typedef void *LPVOID;
typedef PVOID LONG_PTR;
#endif
typedef unsigned int HANDLE;
typedef DWORD *LPDWORD;
typedef int HWND;
typedef pthread_mutex_t CRITICAL_SECTION;
typedef void *return_t;
typedef semaphore sem_t;
//typedef int socklen_t;

#define __int8 char
#define __int16 short
#define __int32 int
#define __int64 long long

typedef unsigned char __uint8;
typedef unsigned short __uint16;
typedef unsigned int __uint32;
typedef unsigned long long __uint64;


#define TRUE 1
#define FALSE 0
#define CALLBACK
#ifndef __stdcall
#define __stdcall
#endif
#define WINAPI __stdcall
#define APIENTRY __stdcall

#define TLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)



struct _timeb {
	time_t time; /* seconds */
	unsigned short millitm;
	short timezone;
	short dstflag;
};


#if defined(__cplusplus)
extern "C" {
#endif

int _ftime(struct _timeb *tp); 

__int64 _atoi64(const char *str); 

#if defined(__cplusplus)
};
#endif




#if defined(__cplusplus)
#define _RWSTD_NO_OLD_COUNT
namespace std {
template <class InputIterator, class T>
size_t count(InputIterator first, InputIterator last, const T& value) {
	size_t n = 0;
	while (first != last)
		if (*first++ == value) ++n;
	return n;
}
double inline fabs(double v) {
	return ::fabs(v); 
} 
double inline abs(double v) {
	return ::fabs(v);
}
template <typename T>
T inline abs(T v) {
	return v >= 0? v: -v;
}
double inline pow(double v, int p) { 
	if(p == 2) { 
		return v*v; 
	} 
	if(v > 0) { 
		return ::pow(v, (double)p);
	} 
	if(v < 0) { 
		return ::pow(-v, (double)p);
	} 
	return 0;
}
double inline cos(double v) {
	return ::cos(v);
}
double inline sin(double v) {
	return ::sin(v);
}
double inline acos(double v) {
	return ::acos(v);
}
double inline asin(double v) {
	return ::asin(v);
}
double inline tan(double v) {
	return ::tan(v);
}
double inline atan(double v) {
	return ::atan(v);
}
double inline floor(double v) {
	return ::floor(v);
}
double inline sqrt(double v) {
	return ::sqrt(v);
}
}
#endif


#define EXPORTIPCINTERFACE
#define IMPORTIPCINTERFACE

#define EXPORTDLLVARIABLES
#define IMPORTDLLVARIABLES

#endif /*STDAFX_OS9_h*/
