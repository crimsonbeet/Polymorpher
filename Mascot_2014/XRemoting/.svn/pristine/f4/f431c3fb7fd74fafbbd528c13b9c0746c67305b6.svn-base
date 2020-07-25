// Created by Alex Nikol Bourakov (ANB) around April/07

#pragma hdrstop

#include "stdafx_interface.h"
#include "WSIClassFactoryInterfaces.h"




void IWsiCollection::m_insert(IWsiCollection::object_serializers& map, IWsiSerializerBase* serializer) {
	bool rc = map.insert(object_serializers::value_type(serializer->_wsi_id, serializer)).second;
	assert(rc);
}


IWSICallable::IWSICallable(): _pcookies(0), _in_use(0) {
}
IWSICallable::~IWSICallable() {
}

void IWSICallable::lock() { 
} 

void IWSICallable::unlock() { 
} 

void IWSICallable::remove_bookmark(IWSIMarshaller* mat) {
	if(_pcookies) {
		_pcookies->erase(mat);
	}
}

void IWSICallable::add_bookmark(IWSIMarshaller* mat) {
	if(!_pcookies) {
		_pcookies = new marshaller_cookies();
	}
	if(_pcookies) {
		_pcookies->insert(mat);
	}
}

template<class I>
void IWSICallable_remove_all(I* obj) {
	if(obj->_pcookies) {
		IWSICallable::marshaller_cookies::iterator iter = obj->_pcookies->begin();
		while(iter != obj->_pcookies->end()) {
			(*iter++)->remove_callee(obj);
		}
		delete obj->_pcookies;
		obj->_pcookies = 0;
	}
	// If this object is still in use, then
	// the following lock will suspend the destruction until object is released.
	while(obj->_in_use) {
		OSWait();
	}
}

EXP void DestroyIWSICallable(IWSICallable *iwsi) { // only for use with for_each algorithm to delete unused objects.
	if(iwsi) {
		delete iwsi;
	}
}




IWsiSerializerBase* IWsiSerializerBase::Create() const {
	return NULL;
}

IWsiSerializerBase* IWsiSerializerBase::CreateEmpty() const {
	return NULL;
}

void IWsiSerializerBase::remove_all() {
	IWSICallable_remove_all(this);
}

void IWsiSerializerBase::register_with(IWSIMarshaller& mat) {
	mat.add_callee(this, _wsi_id);
}

void IWsiSerializerBase::invoke_callback(const std::string& name, void* obj, LONG_PTR rpc_handle) {
	if(!_pcookies) {
		_pcookies = new marshaller_cookies();
	}
	marshaller_cookies::iterator iter = _pcookies->begin();
	while(iter != _pcookies->end()) {
		(*(*iter))(name, obj, rpc_handle);
		++iter;
	}
}

void IWsiSerializerBase::onNewObject(const std::string& name, void* obj, LONG_PTR rpc_handle, IWSIMarshaller *current_host) {
	if(current_host) {
		(*current_host)(name, obj, rpc_handle);
	}
	else
	if(_pcookies) {
		if(_pcookies->size()) {
			invoke_callback(name, obj, rpc_handle);
		}
	}
}

BOOL IWsiSerializerBase::onNewObjectClone(const std::string& name, void *obj, LONG_PTR rpc_handle, IWSIMarshaller *current_host, BOOL locked) { 
	onNewObject(name, obj, rpc_handle, current_host); 
	return locked; 
} 

void IWsiConsumerBase::remove_all() {
	IWSICallable_remove_all(this);
}

void IWsiConsumerBase::register_with(IWSIMarshaller& mat) {
	mat.add_callee(this, _wsi_id);
}



EXP void BindAPITriplet(IWsiConsumerBase *c, IWsiSerializerBase *s, IWSIMarshaller *m) {
	if(c && s && m) {
		c->register_with(*m);
		s->register_with(*m);
	}
}






bool XRPCHandle::InvokeMarshaller(ISTREAMReader& reader, const std::string& inp_str, ISTREAMWriter *writer, std::string& out_str) { 
	bool delete_writer = false; 
	bool rc = false; 
	if(writer == NULL) { 
		writer = reader.create_writer(); 
		delete_writer = true; 
	} 
	XRPCHandle rpc(*writer); 
	rpc._marshaller = _marshaller;
	rpc._remoting_object = _remoting_object;
	rpc._rpc_outstring = (LONG_PTR)&out_str; 

	TLSObject *tlsobj = GetTLSObject2();
	if(tlsobj) { 
		if(tlsobj->PushRpcHandle(&rpc)) { 
			(*((IWSIMarshaller*)_marshaller))(reader, inp_str, (LONG_PTR)&rpc); 

			tlsobj->PopRpcHandle(); 
			rc = true; 
		} 
	} 
	if(delete_writer) { 
		delete writer; 
	} 
	return rc; 
} 

bool XRPCHandle::InvokeMarshaller(ISTREAMReader& reader, std::string::const_iterator from, std::string::const_iterator to) {
	bool rc = false;
	if(_marshaller) {
		(*((IWSIMarshaller*)_marshaller))(reader, from, to);
		rc = true;
	}
	return rc;
}

XRPCHandle::XRPCHandle(ISTREAMWriter& writer):
	_writer(writer),
	_marshaller(0),
	_remoting_object(0),
	_rpc_socket(0),
	_rpc_outstring(0) {
}
