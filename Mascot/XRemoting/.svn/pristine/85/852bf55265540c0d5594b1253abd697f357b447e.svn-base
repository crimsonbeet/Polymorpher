// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef xthreadpoolH 
#define xthreadpoolH 

#include "IPCGate.h"

struct XThreadCtl { 

	DWORD _idthread; 
	pthread_t _hthread; 
	return_t (__stdcall *_afunction)(PVOID); 
	PVOID _aparameter; 
	unsigned char _vacant; 
	unsigned char _alertable;
	sem_t _event; 

	__int64 _assignedSequentialNumber; 
	volatile __int64 *_p_minSequentialNumber; 

	XThreadCtl() { 
		_idthread = 0; 
		_hthread = 0; 

#ifdef _WIN32_WINDOWS
		_event = 0; 
#else
		memset(&_event, 0, sizeof(_event)); 
#endif 

		_afunction = 0; 
		_aparameter = 0; 
		_vacant = 1; 

		_alertable = 0;

		_assignedSequentialNumber = 0; 
		_p_minSequentialNumber = 0; 
	} 

}; 

class XThreadPool { 

	wsi_gate LOCKER; 
	std::vector<XThreadCtl> _threadctls; 

	__int64 _maxSequentialNumber; 
	__int64 _minSequentialNumber; 
	
	XThreadCtl* AllocateThreadCtl(XThreadCtl *actl = 0);
	
	static return_t CALLBACK AFunction(PVOID lpParameter); 

public: 

	return_t QueueWorkItem(return_t (__stdcall *afunction)(PVOID), PVOID aparameter, PVOID athreadctl = 0);

	return_t RunAFunction(XThreadCtl&);

	void WaitFor(); 
	void Cleanup(); 
	void Signal(int iSignal = 9); 

	int CountActive(); 

	XThreadPool(); 
	~XThreadPool(); 

}; 

extern XThreadPool _g_threadpool; 
return_t EXP __stdcall RunAFunction(XThreadCtl& ctl); // for internal use only. 

#endif //xthreadpoolH 

