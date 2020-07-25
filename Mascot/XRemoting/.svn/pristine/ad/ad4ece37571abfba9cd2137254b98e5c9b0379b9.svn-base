// Created by Alex Nikol Bourakov (ANB) around April/07

#ifdef __BORLANDC__
#pragma hdrstop
#endif


#include "stdafx_gate.h"
#include "IPCGate.h"


void my_mem_lock(long& cref) { 
	while(InterlockedIncrement(&cref) > 1) {
		InterlockedDecrement(&cref);
		OSWait();
	}
} 
void my_mem_unlock(long& cref) { 
	InterlockedDecrement(&cref);
} 


void wsi_gate::Init() {
	_terminating = false;
#if defined(WSI_GATE_USE_HOMEGROWN_SPINLOCK)
	_cref = 0;
#else
	_alive = 0;
#endif
}

wsi_gate::wsi_gate() {
	Init();
}

wsi_gate::wsi_gate(const wsi_gate&) {
	Init();
}
wsi_gate& wsi_gate::operator=(const wsi_gate&) { 
	Init();
	return *this; 
} 

wsi_gate::~wsi_gate() {
#if defined(WSI_GATE_USE_HOMEGROWN_SPINLOCK)
#else
	if(_alive) { 
		OSSemaphoreDestroy(_gate); 
	} 
#endif
}

void wsi_gate::lock() {
#if defined(WSI_GATE_USE_HOMEGROWN_SPINLOCK)
	if(!_terminating) {
		while(InterlockedIncrement(&_cref) > 1) {
			InterlockedDecrement(&_cref);
			OSWait();
		}
	}
#else
	if(!_terminating) { 
		if(!_alive) { 
			static long s_cref = 0; 
			bool done = false; 
			my_mem_lock(s_cref); 
			if(!_alive) { 
				done = true; 
				OSSemaphoreInit(_gate); 
				_alive = 1; 
			} 
			my_mem_unlock(s_cref); 
			if(!done) { 
				OSSemaphoreWait(_gate); 
			} 
		} 
		else { 
			OSSemaphoreWait(_gate); 
		} 
	}
#endif
}

void wsi_gate::unlock() {
#if defined(WSI_GATE_USE_HOMEGROWN_SPINLOCK)
	if(!_terminating) {
		InterlockedDecrement(&_cref);
	}
#else
	if(!_terminating) {
		if(_alive) { 
			OSSemaphorePost(_gate); 
		} 
	} 
#endif
}



void wsi_reenterantgate::Init() {
	_terminating = false;
	_cthreadid = (DWORD)-1;
	_threadcref = 0;
#if defined(WSI_REENTGATE_USE_HOMEGROWN_SPINLOCK)
	_cref = 0;
#else
	_alive = 0;
#endif
}

wsi_reenterantgate::wsi_reenterantgate() {
	Init();
}

wsi_reenterantgate::wsi_reenterantgate(const wsi_reenterantgate&) {
	Init();
}
wsi_reenterantgate& wsi_reenterantgate::operator=(const wsi_reenterantgate&) { 
	Init();
	return *this; 
} 

wsi_reenterantgate::~wsi_reenterantgate() {
#if defined(WSI_REENTGATE_USE_HOMEGROWN_SPINLOCK)
#else
	if(_alive) { 
		OSSemaphoreDestroy(_gate); 
	} 
#endif
}

void wsi_reenterantgate::lock() {
	if(!_terminating) {
		lock(OSThreadCurrent()); 
	}
}

void wsi_reenterantgate::lock(DWORD my_threadid) {
	if(!_terminating) {
#if defined(WSI_REENTGATE_USE_HOMEGROWN_SPINLOCK)
		while(InterlockedIncrement(&_cref) > 1 && _cthreadid != my_threadid) {
			InterlockedDecrement(&_cref);
			OSWait();
		}
#else
		if(!_alive) { 
			static long s_cref = 0; 
			my_mem_lock(s_cref); 
			if(!_alive) { 
				OSSemaphoreInit(_gate); 
				_alive = 1; 
				_cthreadid = my_threadid;
			} 
			my_mem_unlock(s_cref); 
		} 
		if(_cthreadid != my_threadid) { 
			OSSemaphoreWait(_gate); 
		} 
#endif
		_cthreadid = my_threadid;
		_threadcref++;
	}
}

void wsi_reenterantgate::unlock() {
	if(!_terminating) {
		if(--_threadcref == 0) {
			_cthreadid = (DWORD)-1;
#if !defined(WSI_REENTGATE_USE_HOMEGROWN_SPINLOCK)
			if(_alive) { 
				OSSemaphorePost(_gate); 
			} 
#endif
		}
#if defined(WSI_REENTGATE_USE_HOMEGROWN_SPINLOCK)
		InterlockedDecrement(&_cref);
#endif
	}
}


CRefCounter::CRefCounter(): _cref(1) {
}

CRefCounter::~CRefCounter() {
}

long int CRefCounter::AddRef() {
	return InterlockedIncrement(&_cref);
}

long int CRefCounter::Release() {
	long int res = InterlockedDecrement(&_cref);
	if(res <= 0) {
		delete this;
	}

	return res;
}












#if defined(_WIN32_WINDOWS)
void wsi_mutex::Init() {
	_cthreadid = (DWORD)-1; 
	_terminating = false;
	_mutex = (HANDLE)0xcdcdcdcd;
}

wsi_mutex::wsi_mutex() {
	Init();
}

wsi_mutex::wsi_mutex(const wsi_mutex&) {
	Init();
}

wsi_mutex::~wsi_mutex() {
	if(_mutex != (HANDLE)0xcdcdcdcd) {
		CloseHandle(_mutex);
	}
}

void wsi_mutex::lock() {
	if(!_terminating) {
		lock(OSThreadCurrent()); 
	}
}

void wsi_mutex::lock(DWORD threadid) {
	if(!_terminating) {
		if(_mutex == (HANDLE)0xcdcdcdcd) { 
			_mutex = CreateMutex(0, 0, 0);
		} 
		if(_mutex != (HANDLE)0xcdcdcdcd) {
			WaitForSingleObject(_mutex, INFINITE);
			_cthreadid = threadid; 
		}
	}
}

void wsi_mutex::unlock() {
	if(!_terminating) {
		if(_mutex != (HANDLE)0xcdcdcdcd) {
			_cthreadid = (DWORD)-1; 
			try { 
				ReleaseMutex(_mutex);
			} 
			catch(...) { 
			} 
		}
	}
}
#endif



