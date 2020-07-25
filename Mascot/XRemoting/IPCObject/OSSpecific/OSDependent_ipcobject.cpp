// Created by Alex Nikol Bourakov (ANB) around April/07

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "stdafx_ipcobject.h"


#if !defined(_WIN32_WINDOWS)
void IPCObjectDllMain() {
	InitializeIPCInterface();
}
#endif


BOOL InitializeSOCKInterface() {
	BOOL rc = TRUE;
#if defined(_WIN32_WINDOWS)
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) { /* Load Winsock 2.0 DLL */
		rc = FALSE;
	}
#endif

	return rc;
}


#if defined(WSI_USE_EXPLICIT_SOCKET_LOCK)
std::vector<SOCKET> _g_sendlock_sockets; 
wsi_gate _g_sendlock; 

void LockSocket(SOCKET sock) { 
	BOOL do_wait = FALSE; 
	do { 
		if(do_wait) { 
			OSWait(); 
		} 
		_g_sendlock.lock(); 
		std::vector<SOCKET>::iterator iter = std::find(_g_sendlock_sockets.begin(), _g_sendlock_sockets.end(), sock); 
		do_wait = iter != _g_sendlock_sockets.end(); 
		if(!do_wait) { 
			_g_sendlock_sockets.push_back(sock); 
		} 
		_g_sendlock.unlock(); 
	} 
	while(do_wait); 
} 

void UnLockSocket(SOCKET sock) { 
	_g_sendlock.lock(); 
	_g_sendlock_sockets.erase(std::remove_if(_g_sendlock_sockets.begin(), _g_sendlock_sockets.end(), IXEqual<SOCKET>(sock)), _g_sendlock_sockets.end()); 
	_g_sendlock.unlock(); 
} 
#endif

int XSendv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, const DWORD count) {
#if defined(WSI_USE_EXPLICIT_SOCKET_LOCK)
	LockSocket(s); 
#endif

#if defined(_UCC)
	iovec *iv = reinterpret_cast<iovec*>(lpBuffers);
	errno = 0; 
	int nsent = 0; 
	int bytes_sent = nsent = writev(s, iv, dwBufferCount); 
	while(nsent > 0 && bytes_sent < count) { 
		DWORD len = 0; 
		for(DWORD x = 0; x < dwBufferCount; ++x) {
			len += lpBuffers[x].len; 
			if(len > bytes_sent) { 
				len -= bytes_sent;
				bytes_sent += (nsent = send(s, ((char*)lpBuffers[x].buf) + (lpBuffers[x].len - len), len, MSG_NOSIGNAL)); 
			} 
		} 
	} 
	*lpNumberOfBytesSent = bytes_sent; 

#elif defined(__GCC__)
	char buf[count];
	DWORD len = 0;
	for(DWORD x = 0; x < dwBufferCount; ++x) {
		memcpy(buf + len, lpBuffers[x].buf, lpBuffers[x].len);
		len += lpBuffers[x].len;
		assert(len <= count);
	}
	errno = 0;
	*lpNumberOfBytesSent = send(s, buf, len, MSG_NOSIGNAL);

#elif defined(_WIN32_WINDOWS)
	WSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, 0, NULL, NULL);

#endif

#if defined(WSI_USE_EXPLICIT_SOCKET_LOCK)
	UnLockSocket(s); 
#endif

	return errno;
}

