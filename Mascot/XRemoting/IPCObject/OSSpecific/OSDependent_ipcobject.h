// Created by Alex Nikol Bourakov (ANB) around April/07

#if defined(_UCC)
#include "..\..\Include\stdafx_OS9.h"

void IPCInterfaceDllMain();

#include <sys\types.h>
#include <sys\socket.h>
#include <sys\uio.h>
#include <UNIX\ioctl.h>
#include <sys\ioctl.h>
#include <netinet\in.h>
#include <netinet/tcp.h>
#include <sys\select.h>
#include <sched.h>

#define closesocket close
#define ioctlsocket ioctl
#define SD_SEND 1
#define SD_RECEIVE 0
#define SD_BOTH 2
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
/* is now available for OS9000 v5.2 sys/socket.h:493:*/
#define     MSG_NOSIGNAL    0x0400 /*do not generate SIGPIPE on EOF */

typedef int SOCKET;


// LINUX

#elif defined(__GCC__)
#include "../../Include/stdafx_linux.h"

#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sched.h>
#include <dlfcn.h>

#define closesocket close
#define ioctlsocket ioctl
#define caddr_t
#define SD_SEND SHUT_WR
#define SD_RECEIVE SHUT_RD
#define SD_BOTH SHUT_RDWR
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

typedef int SOCKET;

void IPCObjectDllMain() __attribute__((constructor));



// WINDOWS

#elif defined(_MSC_VER)
#include "..\..\Include\stdafx_windows.h"

#define FD_SETSIZE 256

#include "winsock2.h"
#include "Ws2tcpip.h"

#define caddr_t
/* is now available for OS9000 v5.2 sys/socket.h:493:*/
#define     MSG_NOSIGNAL    0x0400 /*do not generate SIGPIPE on EOF */
#if _MSC_VER < 1500
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif

#pragma comment(lib, "IPCClassFactory.lib")
#pragma comment(lib, "IPCInterface.lib")
#pragma comment(lib, "IPCGate.lib")




#elif defined(__BORLANDC__)
#include "..\..\Include\stdafx_windows.h"
#include <stdio.h>


#include "winsock2.h"
#include "Ws2tcpip.h"

#define caddr_t
/* is now available for OS9000 v5.2 sys/socket.h:493:*/
#define     MSG_NOSIGNAL    0x0400 /*do not generate SIGPIPE on EOF */
#define EWOULDBLOCK WSAEWOULDBLOCK

#ifndef _w64
#define _w64
#endif

#define _timeb timeb
#define _ftime ftime

#pragma comment(lib, "IPCClassFactoryBorland.lib")
#pragma comment(lib, "IPCInterfaceBorland.lib")
#pragma comment(lib, "IPCGateBorland.lib")
#pragma comment(lib, "Ws2_32.lib")




#endif




#if !defined(_WIN32_WINDOWS)
typedef struct __WSABUF { // mimics iovec therefore reinterpret_cast<> can be used.
	void *buf;
	size_t len;
} WSABUF, *LPWSABUF;

#endif

int XSendv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, const DWORD dwCount);




//struct iovec {
//	void *iov_base;
//	size_t iov_len;
//}

