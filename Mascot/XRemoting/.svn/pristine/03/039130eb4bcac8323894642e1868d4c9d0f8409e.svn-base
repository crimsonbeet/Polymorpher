// Created by Alex Nikol Bourakov (ANB) around April/07

#include "stdafx_classfactory.h"
#include "WSIMaterializer.h"



struct XCachedLocator {
	std::stack<vectorpos_locator> *_pstack;
	std::vector<wsi_object> *_pcontext;

	XCachedLocator *_pNext; 

	static int s_nstaticCntFreedEntries; 
	static XCachedLocator* s_pstaticFreedEntries; 
	static int s_nstaticCntAllocatedEntries; 
	static wsi_gate s_gate; 

	void*operator new(size_t Size); 
	void operator delete(void *p); 

	XCachedLocator(): _pNext(0) { 
		if(_pstack == 0) { 
			_pstack = new std::stack<vectorpos_locator>(); 
		} 
		if(_pcontext == 0) { 
			_pcontext = new std::vector<wsi_object>(); 
		} 
	} 
};

int XCachedLocator::s_nstaticCntFreedEntries = 0; 
XCachedLocator* XCachedLocator::s_pstaticFreedEntries = 0; 
int XCachedLocator::s_nstaticCntAllocatedEntries = 0; 
wsi_gate XCachedLocator::s_gate; 

void*XCachedLocator::operator new(size_t Size) { 
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

void XCachedLocator::operator delete(void *p) { 
	s_gate.lock(); 

	XCachedLocator *t = reinterpret_cast<XCachedLocator*>(p); 
	t->_pNext = s_pstaticFreedEntries; 
	s_pstaticFreedEntries = t; 

	s_nstaticCntFreedEntries++; 

	s_gate.unlock(); 
} 






vectorpos_locator::vectorpos_locator(const size_t pos) {
	_pos = pos;
}

vectorpos_locator::operator size_t() const {
	return _pos;
}

wsi_materializer::wsi_materializer() {
	_callback = 0;
	_rpc_handle = 0;
	_use_clones = FALSE;
	_cached_context = NULL; 
}

wsi_materializer::wsi_materializer(const wsi_materializer& other) {
	*this = other; 
	_cached_context = NULL; 
}

wsi_materializer::~wsi_materializer() {
	if(_cached_context != NULL) { 
		release(); 
	} 
}

void wsi_materializer::reserve(size_t n) { 
	if(_context.capacity() == 0) { 
		if(_cached_context == NULL) { 
			_cached_context = new XCachedLocator(); 
			_context.swap(*((XCachedLocator*)_cached_context)->_pcontext); 
#if !defined(_UCC) && !defined(__BORLANDC__)
#if defined(_MSC_VER) && _MSC_VER > 1400
			_stack.swap(*((XCachedLocator*)_cached_context)->_pstack);
#endif
#endif
		} 
	} 
	if(n > 0) { 
		_context.reserve(n); 
	} 
} 

void wsi_materializer::release() { 
	if(_cached_context != NULL) { 
		if(_context.capacity()) { 
			_context.swap(*((XCachedLocator*)_cached_context)->_pcontext); 
#if !defined(_UCC) && !defined(__BORLANDC__)
#if defined(_MSC_VER) && _MSC_VER > 1400
			_stack.swap(*((XCachedLocator*)_cached_context)->_pstack); 
#endif
#endif
		} 
		delete ((XCachedLocator*)_cached_context); 
		_cached_context = NULL; 
	} 
	else 
	if(_context.capacity()) { 
		_context = std::vector<wsi_object>(); 
	} 
} 

inline int wsi_materializer::increment_obj_counter(size_t pos) {
	return ++_context[pos]._nInnerObjects;
}

void wsi_materializer::place_object_on_context_stack(const IWSIObject& iwsiobj) {
//void wsi_materializer::place_object_on_context_stack(wsi_object& obj) {
//	_context.push_back(obj);
	_context.push_back(*(reinterpret_cast<wsi_object*>(iwsiobj._obj)));
	if(!_stack.empty()) {
		increment_obj_counter(_stack.top());
	}
}

IWSIObject wsi_materializer::get_current() {
//wsi_object& wsi_materializer::get_current() {
	return IWSIObject(&_current);
}

IWSIObject wsi_materializer::get_context_current() {
	if(!_stack.empty()) {
		return IWSIObject(&_context[_stack.top()]);
	}
	return IWSIObject(&_current);
}


void wsi_materializer::reduce_stack() {
	if(!_stack.empty()) {
		_stack.pop();
		if(_stack.empty()) {
			if(_callback) {
				(*_callback)(IWSIStackObject(&_context), _rpc_handle, _use_clones);
			}
		}
	}
}

void wsi_materializer::place_composite_object_on_stack(const IWSIObject& iwsiobj) {
//void wsi_materializer::place_composite_object_on_stack(wsi_object& obj) {
//	place_object_on_context_stack(obj);
	place_object_on_context_stack(iwsiobj);
	_stack.push(_context.size() - 1);
//	obj.clear();
	(reinterpret_cast<wsi_object*>(iwsiobj._obj))->clear();
}

void wsi_materializer::process_flat_object(const IWSIObject& iwsiobj) {
//void wsi_materializer::process_flat_object(wsi_object& obj) {
	wsi_object& obj = *(reinterpret_cast<wsi_object*>(iwsiobj._obj));
	if(obj.empty()) {
		// Do nothing.
	}
	else
	if(!_stack.empty()) {
//		place_object_on_context_stack(obj);
		place_object_on_context_stack(iwsiobj);
	}
	else
	if(_callback) {
//		(*_callback)(obj, _rpc_handle);
		(*_callback)(iwsiobj, _rpc_handle, _use_clones);
	}
	obj.clear();
}

void wsi_materializer::process_raw_string(const SIPCRawString& obj) {
	(*_callback)(GetRootSerializer(&obj)._wsi_id, (void*)&obj, _rpc_handle);
}

void wsi_materializer::flush() {
	while(!_stack.empty()) {
		reduce_stack();
	}
}


