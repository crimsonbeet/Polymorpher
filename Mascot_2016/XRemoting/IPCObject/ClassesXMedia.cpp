// Created by Alex Nikol Bourakov (ANB) around April/07

#include "stdafx_ipcobject.h"
#include "IPCObject.h"
#include "XRemoting.h"


XWorkItem::XWorkItem(bool allocate_pitem):
_format_name(0), _pNext(0) { 
	if(allocate_pitem) { 
		if(_pitem == 0) { 
			_pitem = new std::string(); 
		} 
	} 
}

XWorkItem::~XWorkItem() {
}

int XWorkItem::s_nstaticCntFreedEntries = 0; 
XWorkItem* XWorkItem::s_pstaticFreedEntries = 0; 
int XWorkItem::s_nstaticCntAllocatedEntries = 0; 
wsi_gate XWorkItem::s_gate; 


void*XWorkItem::operator new(size_t Size) { 
	s_gate.lock(); 

	PVOID chunk = (PVOID)s_pstaticFreedEntries; 
	if(chunk == 0) { 
		chunk = malloc(Size); 
		memset(chunk, 0, Size); 
		s_nstaticCntAllocatedEntries++; 
	} 
	else { 
		s_pstaticFreedEntries = s_pstaticFreedEntries->_pNext; 
		s_nstaticCntFreedEntries--; 
	} 

	s_gate.unlock(); 
	return chunk; 
} 

void XWorkItem::operator delete(void *p) { 
	s_gate.lock(); 

	XWorkItem *t = reinterpret_cast<XWorkItem*>(p); 
	t->_pNext = s_pstaticFreedEntries; 
	s_pstaticFreedEntries = t; 

	s_nstaticCntFreedEntries++; 

	s_gate.unlock(); 
} 





void XReadingHead::Initialize(XListener * host_ptr) {
	_threadshutdown = TRUE;
	_isrunning = FALSE;
	_hmonitor = 0;
	_thread = 0;
	_host_ptr = host_ptr;
}

XConnector * XReadingHead::Connector() {
	return (XConnector*)_host_ptr->_host_ptr;
}

void XReadingHead::Startthread() {
	FD_ZERO(&_allsockets);
	_isrunning = _threadshutdown = FALSE;
	_maxsock = INVALID_SOCKET;
	OSThreadCreate(_hmonitor, Monitor, (PVOID)this, &_thread, 129);

	while(!_isrunning && !_threadshutdown) {
		OSWait();
	}
}

void XReadingHead::Shutdownthread() {
	if(_hmonitor) {
		_threadshutdown = TRUE;
		OSThreadJoin(_hmonitor);
		_hmonitor = 0;
		_thread = 0;
	}
}


void XReadingHead::Start() {
	Startthread();
}

void XReadingHead::Shutdown() {
	Shutdownthread();

	SSocketCtl::Close(_handler);

	if(_g_trace & 2) {
		LOGMessage(std::string("XReadingHead"), _host_ptr->_name, _host_ptr->_addr, std::string("shut down")); 
	} 
}

void XWritingHead::Initialize(XSubscriber *host_ptr) {
	_initialized = true;
	_host_ptr = host_ptr;
	_target.Reset();
	_target._raw_data = host_ptr->_writer_type == (int)'R'; 
}

bool XWritingHead::Shutdown() {
	if(!_initialized && _target._sock == INVALID_SOCKET) {
		return false;
	}
	bool do_fire_exception = false; 

	if(_initialized && _host_ptr) {
		XRemoting *robj = (XRemoting*)_host_ptr->_host_ptr->_host_ptr;
		if(robj->_logserver._logger.Get() == _target._sock) {
			robj->_logserver._logger.Reset();
		}

		if(_host_ptr->_static && _target._sock != INVALID_SOCKET) {
			do_fire_exception = Connector()->_listener._reader.RemoveSocket(_target);
		}
	}
	if(!do_fire_exception) { 
		do_fire_exception = SSocketCtl::Close(_target);
	} 

//	There can be worker threads running.
//	In this case the ref-counter of the _target must be greater than 1.
//	A previous Close must have caused a failure in the worker threads.
//	Wait for their normal termination.
	while(_target._cref > 1) {
			OSWait();
	}

	if(_host_ptr && _host_ptr->_addr.size()) {
		if(do_fire_exception) { 
			_host_ptr->FireException(1, _host_ptr->_index, FALSE);
		} 
	}
	return do_fire_exception; 
}

bool XWritingHead::IsDefaultWriter() {
	return _host_ptr->_writer_type == (int)'W' || _host_ptr->_writer_type == (int)'X' || _host_ptr->_writer_type == (int)'J';
}



XSocket2Handler<XProducer> * XWritingHead::TearOffAHandler(XProducer *handler) {
	XSocket2Handler<XProducer> * aHandler = new XSocket2Handler<XProducer>(_target._sock, _target._sockaddr, handler->_index);
//	Cancelation of a subscription is done by closing the socket.
//	Thus, Writer should preserve its open socket.
//	Do not reset the socket.
//	_target.Reset();

	aHandler->_handler = handler;

	return aHandler;
}



XConnector * XWritingHead::Connector() {
	return (XConnector*)_host_ptr->_host_ptr;
}
XListener * XWritingHead::Listener() {
	return &Connector()->_listener;
}

XReadingHead * XWritingHead::Reader() {
	return &Listener()->_reader;
}

