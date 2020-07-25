// Created by Alex Nikol Bourakov (ANB) around April/07

#ifdef __BORLANDC__
#pragma warn -8057
#endif

#include "stdafx_gate.h"

#include "ThreadPool.h"

XThreadPool _g_threadpool;


return_t EXP __stdcall QueueWorkItem(return_t (__stdcall *afunction)(PVOID), PVOID aparameter, PVOID athreadctl) {
#ifdef __GCC__
	assert(_g_processid == getpid());
#endif
	return _g_threadpool.QueueWorkItem(afunction, aparameter, athreadctl);
}


void EXP __stdcall CleanupWorkItems() {
	_g_threadpool.Cleanup();
}


void EXP __stdcall WaitforWorkItems() {
	_g_threadpool.WaitFor();
}

int EXP __stdcall CountActiveWorkItems() {
	return _g_threadpool.CountActive();
}

void EXP __stdcall SignalWorkItems(int iSignal) {
	_g_threadpool.Signal(iSignal);
}

return_t EXP __stdcall RunAFunction(XThreadCtl& ctl) { 
	return _g_threadpool.RunAFunction(ctl); 
} 


XThreadPool::XThreadPool() { 
	_threadctls.resize(512); 
	_maxSequentialNumber = _minSequentialNumber = 0; 
} 

XThreadPool::~XThreadPool() {
	WaitFor();
	Cleanup();
}

XThreadCtl* XThreadPool::AllocateThreadCtl(XThreadCtl *actl) {
	LOCKER.lock();
	XThreadCtl *req = 0;
	if(_threadctls.size() == 0) {
		_threadctls.resize(64);
	}
	std::vector<XThreadCtl>::iterator it = _threadctls.begin();
	if(actl && actl->_hthread) {
		it = _threadctls.end();
		if(actl->_vacant && actl->_afunction == 0) {
			req = actl;
			req->_vacant = 0;
		}
	}
	while(req == 0 && it != _threadctls.end()) {
		if(it->_vacant) {
			req = &(*it); 
			req->_assignedSequentialNumber = _maxSequentialNumber++; 
			req->_vacant = 0;
		}
		else {
			++it;
		}
	}

	LOCKER.unlock();

	return req;
}

void XThreadPool::WaitFor() {
	_minSequentialNumber = _maxSequentialNumber; 
	std::vector<XThreadCtl>::iterator it;
	BOOL ok = FALSE;
	while(!ok) {
		ok = TRUE;
		it = _threadctls.begin();
		while(ok && it != _threadctls.end()) {
			if(!it->_vacant) {
				ok = FALSE;
			}
			else {
				++it;
			}
		}
		if(!ok) {
			OSWait();
		}
	}
	_maxSequentialNumber = _minSequentialNumber = 0; 
}

int XThreadPool::CountActive() {
	int rc = 0;
	std::vector<XThreadCtl>::iterator it = _threadctls.begin();
	while(it != _threadctls.end()) {
		if(!it->_vacant) {
			++rc;
		}
		++it;
	}
	return rc;
}

void XThreadPool::Signal(int iSignal) { 
	std::vector<XThreadCtl>::iterator it = _threadctls.begin();
	while(it != _threadctls.end()) {
		if(!it->_vacant) {
			#ifdef _UCC
			pthread_kill(it->_hthread, iSignal);
			#endif
		}
		++it;
	}
} 


#ifdef _UCC
extern "C" void DefaultSignalHandler(int iSignal) { 
	if(iSignal == 0x3300) { 
		signal(iSignal, DefaultSignalHandler); 
	} 
} 
#endif


return_t CALLBACK XThreadPool::AFunction(PVOID lpParameter) {
	XThreadCtl *ctl = (XThreadCtl*)lpParameter;

	#ifdef _UCC
	if(!ctl->_alertable) { // alertable means that an external signal handler has been installed.
		intercept(DefaultSignalHandler);
		signal(0x3300, DefaultSignalHandler);
	}
	#endif

	TLSObject tls_obj; 
	tls_obj.Init(); 
	tls_obj._hthread = ctl->_hthread; 
	SetTLSObject2(&tls_obj); 


	do {
		if(ctl->_p_minSequentialNumber) { 
			while(ctl->_assignedSequentialNumber > *(ctl->_p_minSequentialNumber)) { 
				OSWait(); 
			} 
			if(ctl->_assignedSequentialNumber == *(ctl->_p_minSequentialNumber)) { 
				*(ctl->_p_minSequentialNumber) = ctl->_assignedSequentialNumber + 1; 
			} 
		} 
		if(ctl->_afunction) {
			ctl->_afunction(ctl->_aparameter);
		}
		ctl->_afunction = 0;
		ctl->_aparameter = 0;
		ctl->_vacant = 1;

		if(OSSemaphoreWait(ctl->_event, ctl->_alertable) != 0) {
			break;
		}

	} while(ctl->_afunction);

	SetTLSObject2(NULL); 


	return 0;
}

return_t XThreadPool::RunAFunction(XThreadCtl& ctl) { // it is the caller's responsibility to make sure the ctl matches the thread.
	if(ctl._hthread == 0) {
		OSSemaphoreInit(ctl._event);
		ctl._hthread = OSThreadHandle();
	}

	AllocateThreadCtl(&ctl); // this will allocate it atomically.

	ctl._alertable = 1;
	return_t rc = AFunction((PVOID)&ctl);
	return rc;
}

return_t XThreadPool::QueueWorkItem(return_t (__stdcall *afunction)(PVOID), PVOID aparameter, PVOID athreadctl/*usually == 0; optional pointer to an external control block*/) {
	return_t ok = 0;
	XThreadCtl *ctl = AllocateThreadCtl((XThreadCtl*)athreadctl);
	if(ctl) {
		assert(ctl->_afunction == 0);

		ctl->_aparameter = aparameter;
		ctl->_p_minSequentialNumber = &_minSequentialNumber; 
		ctl->_afunction = afunction;

		if(ctl->_hthread == 0) {
			OSSemaphoreInit(ctl->_event);
			OSThreadCreate(ctl->_hthread, AFunction, (PVOID)ctl, &ctl->_idthread); 
			if(ctl->_hthread == 0) { 
				_minSequentialNumber = _maxSequentialNumber; 
			} 
		}
		else {
			OSSemaphorePost(ctl->_event);
		}

		ok = return_t(ctl->_hthread != 0);
	}

	return ok;
}

void XThreadPool::Cleanup() {
	LOCKER.lock();
	std::vector<XThreadCtl>::iterator it = _threadctls.begin();
	while(it != _threadctls.end()) {
		if(it->_vacant) {
			if(it->_hthread) {
				OSSemaphorePost(it->_event);
				OSThreadJoin(it->_hthread);
				OSSemaphoreDestroy(it->_event);
#ifdef _WIN32_WINDOWS
				it->_event = 0;
#else
				memset(&it->_event, 0, sizeof(it->_event));
#endif
				it->_hthread = 0;
				it->_idthread = 0;
				it->_vacant = 1;
			}
		}
		++it;
	}
	LOCKER.unlock();
}








wsi_gate _g_tlsgate; 

#if defined(_WIN32_WINDOWS)
DWORD 
#else 
u_int32 
#endif
_g_tlsindex = TLS_OUT_OF_INDEXES; 


void TLSObject::Init() { 
	memset(_connectorname, 0, sizeof(_connectorname));
	memset(_rpchandles, 0, sizeof(_rpchandles));
	_index = 0; 
	_onreturn_shutdownsocket = 0; 
	_hthread = 0; 
	_rpchandlespos = -1;
	_stoken = 0;
} 

void TLSObject::SetConnectorName(char *name, int size) { 
	if(size < 0) { 
		if(name != 0) { 
			size = strlen(name); 
		} 
	} 
	if(size > 0 && name != 0) { 
		if((size_t)size > (sizeof(_connectorname) - 1)) { 
			size = sizeof(_connectorname) - 1; 
		} 
		memcpy(_connectorname, name, size); 
		_connectorname[size] = 0; 
	} 
} 

PVOID TLSObject::GetRpcHandle() { 
#if defined(__BORLANDC__) && __BORLANDC__ < 0x630
	if(_rpchandlespos > -1 && _rpchandlespos < 16) {
#else 
	if(_rpchandlespos > -1 && _rpchandlespos < (int)ARRAY_NUM_ELEMENTS(_rpchandles)) {
#endif
		return _rpchandles[_rpchandlespos];
	} 
	return NULL; 
} 
PVOID TLSObject::PopRpcHandle() { 
#if defined(__BORLANDC__) && __BORLANDC__ < 0x630
	if(_rpchandlespos > -1 && _rpchandlespos < 16) {
#else 
	if(_rpchandlespos > -1 && _rpchandlespos < (int)ARRAY_NUM_ELEMENTS(_rpchandles)) {
#endif
		return _rpchandles[_rpchandlespos--];
	} 
	return NULL; 
} 
bool TLSObject::PushRpcHandle(PVOID obj) { 
#if defined(__BORLANDC__) && __BORLANDC__ < 0x630
	if(_rpchandlespos < 15) {
#else 
	if(_rpchandlespos < (int)(ARRAY_NUM_ELEMENTS(_rpchandles)-1)) {
#endif
		_rpchandles[++_rpchandlespos] = obj; 
		return true; 
	} 
	return false; 
} 


extern "C" {
static void free_tlsdatas(void *_vector) {
}
};

void InitTLSIndex() { 
	_g_tlsgate.lock(); 
	if(_g_tlsindex == TLS_OUT_OF_INDEXES) { 
#if defined(_WIN32_WINDOWS)
		_g_tlsindex = TlsAlloc(); 
#else 
		pthread_key_create(&_g_tlsindex, free_tlsdatas); 
#endif
	} 
	_g_tlsgate.unlock(); 
} 

EXP TLSObject* __stdcall GetTLSObject2() { 
	TLSObject *obj = 0; 

	if(_g_tlsindex == TLS_OUT_OF_INDEXES) { 
		InitTLSIndex(); 
	} 

#if defined(_WIN32_WINDOWS)
	obj = (TLSObject*)TlsGetValue(_g_tlsindex); 
#else 
	obj = (TLSObject*)pthread_getspecific(_g_tlsindex); 
#endif

	return obj; 
} 

VOID EXP __stdcall SetTLSObject2(TLSObject *obj) { 
	if(_g_tlsindex == TLS_OUT_OF_INDEXES) { 
		InitTLSIndex(); 
	} 
#if defined(_WIN32_WINDOWS)
	TlsSetValue(_g_tlsindex, obj); 
#else 
	pthread_setspecific(_g_tlsindex, obj); 
#endif
} 


EXP TLSObject __stdcall GetTLSObject() { 
	TLSObject *obj = GetTLSObject2(); 
	if(obj == NULL) { 
		TLSObject tlsobj; 
		tlsobj.Init(); 
		return tlsobj; 
	} 
	return *obj; 
} 

VOID EXP __stdcall SetTLSObject(TLSObject& tlsobj) { 
	TLSObject *obj = GetTLSObject2(); 
	if(obj != NULL) { 
		*obj = tlsobj; 
	} 
} 

