// Created by Alex Nikol Bourakov (ANB) around April/07

#include "stdafx_classfactory.h"
#include "WSIMarshaller.h"
#include "WSIObject.h"


EXP IWSIMarshaller * CreateIWSIMarshaller() {
	return new CWSIMarshaller();
}

EXP void DestroyIWSIMarshaller(IWSIMarshaller *marsh) {
	if(marsh) {
		delete marsh;
	}
}



bool CWSIMarshaller::add_callee(IWsiSerializerBase *t, const std::string& wsi_id) {
	bool rc = false;

	lock();
	if(_mapFact.insert(object_serializers::value_type(wsi_id, t)).second != false) {
		_mapSerializersCookies.insert(
			serializer_cookies::value_type(t, wsi_id));
		t->add_bookmark(this);
		rc = true;
	}
	unlock();

	return rc;
}

void CWSIMarshaller::remove_callee(IWsiSerializerBase *t) {
	lock();
	serializer_cookies::iterator iter = _mapSerializersCookies.find(t);
	if(iter != _mapSerializersCookies.end()) {
		_mapFact.erase((*iter).second);
		_mapSerializersCookies.erase(iter);
	}
	unlock();
}


IWsiSerializerBase * CWSIMarshaller::find_serializer(const std::string& wsi_id) {
	IWsiSerializerBase *t = 0;
	lock();
	object_serializers::iterator iter = _mapFact.find(wsi_id);
	if(iter != _mapFact.end()) {
		t = (*iter).second;
		InterlockedIncrement(&(*t)._in_use);
	}
	unlock();

	return t;
}

void CWSIMarshaller::operator()(const IWSIStackObject& iwsistack, LONG_PTR rpc_handle, BOOL use_clones) {
	std::vector<wsi_object>& ostack = *(reinterpret_cast<std::vector<wsi_object>*>(iwsistack._stobj));
	IWsiSerializerBase* t =
		find_serializer(ostack[0]._header.first);
	if(t) {
		(*t)(iwsistack, rpc_handle, this, TRUE/*lock*/, use_clones);

		if(InterlockedDecrement(&(*t)._in_use) < 0) {
		}
	}

	size_t x = 0;
	while(x < ostack.size()) {
		ostack[x++].log("Object was not accepted ");
	}
	ostack.resize(0);
}


void CWSIMarshaller::operator()(const IWSIObject& iwsiobj, LONG_PTR rpc_handle, BOOL use_clones) {
	wsi_object& obj = *(reinterpret_cast<wsi_object*>(iwsiobj._obj));
	IWsiSerializerBase* t =
		find_serializer(obj._header.first);
	if(t) {
		(*t)(iwsiobj, rpc_handle, this, TRUE/*lock*/, use_clones);

		if(InterlockedDecrement(&(*t)._in_use) < 0) {
		}
	}
	else {
		obj.log("Unknown object ");
	}
}

void CWSIMarshaller::operator()(const std::string& id, void *obj, LONG_PTR rpc_handle) {
	IWsiConsumerBase * collable[64];
	size_t cnt = 0;

	lock();
	object_consumers::iterator iter = _mapCons.find(id);
	if(iter != _mapCons.end()) {
		do {
			collable[cnt++] = (*iter).second;
			InterlockedIncrement(&(*iter).second->_in_use);
		} while(++iter != _mapCons.end() && cnt < ARRAY_NUM_ELEMENTS(collable) && (*iter).first == id);
	}
	unlock();

	while(cnt > 0) {
		IWsiConsumerBase *t = collable[--cnt];
		(*t).lock();
		(*t)(obj, rpc_handle);
		(*t).unlock();

		if(InterlockedDecrement(&(*t)._in_use) < 0) {
		}
	}
}

bool CWSIMarshaller::add_callee(IWsiConsumerBase* t, const std::string& wsi_id) {
	consumer_cookies& map = _mapConsumersCookies;
	bool rc = false;

	lock();
	if(map.insert(consumer_cookies::value_type(t, wsi_id.c_str())).second != false) {
		_mapCons.insert(
			object_consumers::value_type(wsi_id, t));
		t->add_bookmark(this);
		rc = true;
	}
	unlock();

	return rc;
}

void CWSIMarshaller::remove_callee(IWsiConsumerBase *t) {
	lock();
	consumer_cookies::iterator iter = _mapConsumersCookies.find(t);
	if(iter != _mapConsumersCookies.end()) {
		_mapCons.erase((*iter).second);
		_mapConsumersCookies.erase(iter);
	}
	unlock();
}

void CWSIMarshaller::operator()(const ISTREAMReader& parser, const std::string& wsi, LONG_PTR rpc_handle, BOOL use_clones) {
	static const std::string& empty_str = "";
	static std::string::const_iterator it = empty_str.begin();
	if(wsi.size()) { 
		(*this)(parser, wsi, it, it, rpc_handle, use_clones);
	} 
}
void CWSIMarshaller::operator()(const ISTREAMReader& parser, std::string::const_iterator from, std::string::const_iterator to, LONG_PTR rpc_handle, BOOL use_clones) {
	static const std::string& empty_str = "";
	if(from != to) { 
		(*this)(parser, empty_str, from, to, rpc_handle, use_clones);
	} 
}
void CWSIMarshaller::operator()(const ISTREAMReader& parser, const std::string& wsi, std::string::const_iterator from, std::string::const_iterator to, LONG_PTR rpc_handle, BOOL use_clones) {
	int old_encoding = _encoding;

	_encoding = parser.Encoding();

	wsi_materializer mat = _materializer; 
	mat._rpc_handle = rpc_handle; 
	mat._use_clones = use_clones; 
	if(wsi.size()) {
		mat.reserve(wsi.size() / 64);
		parser.parse(wsi, mat);
		mat.flush();
	}
	if(to != from) {
		mat.reserve((to - from) / 64);
		parser.parse(from, to, mat);
		mat.flush();
	}
	mat.release();

	_encoding = old_encoding;
}

void CWSIMarshaller::operator()(ISTREAMWriter& writer, const void *obj, const std::string& wsi_id, std::string& wsi) {
	int old_encoding = _encoding;

	_encoding = writer.Encoding();

	IWsiSerializerBase* t = find_serializer(wsi_id);
	if(t) {
		(*t).lock();
		(*t)(writer, obj, wsi);
		(*t).unlock();

		if(InterlockedDecrement(&(*t)._in_use) < 0) {
		}
	}
	else {
		wsi.resize(0);
	}

	_encoding = old_encoding;
}

void CWSIMarshaller::Init() {
	_materializer._callback = this;
	_encoding = 3;
}

int CWSIMarshaller::Encoding() const {
	return _encoding;
}

CWSIMarshaller::CWSIMarshaller() {
	Init();
}

CWSIMarshaller::~CWSIMarshaller() {
	lock();
	serializer_cookies::iterator iter;
	for(iter = _mapSerializersCookies.begin(); iter != _mapSerializersCookies.end(); ++iter) {
		(*iter).first->remove_bookmark(this);
	}
	consumer_cookies::iterator iter2;
	for(iter2 = _mapConsumersCookies.begin(); iter2 != _mapConsumersCookies.end(); ++iter2) {
		(*iter2).first->remove_bookmark(this);
	}
	unlock();
}


