// Created by Alex Nikol Bourakov (ANB) around April/07

#include "stdafx_ipcobject.h"

#include "IPCObject.h"
#include "XRemoting.h"


template <typename T, typename C, typename K>
void build_temp_copy(std::vector<T*>& temp, C& cont, K *key, int index = -1) {
	cont.lock();

	typename C::iterator iter;
	if(key) {
		iter = cont.find(*key);
	}
	else {
		iter = cont.begin();
	}

	while(iter != cont.end()) {
		if(key && (*iter).first != *key) {
			break;
		}
		if(index < 0 || (*iter).second->_index == index) {
			if((*iter).second->_addr.size()) {
				T *t = (*iter).second;
				t->AddRef();
				temp.push_back(t);
			}
		}
		++iter;
	}

	cont.unlock();
}



// XRemoting

void XRemoting::AddCollection(const IWsiCollection& coll) {
	if(!_objcollection) {
		_objcollection = new XRemote::CWsiCollection();
	}
	_objcollection->merge(coll);
}

IWsiSerializerBase * XRemoting::FindObj(const std::string& id){
	if(!_objcollection) {
		_objcollection = new XRemote::CWsiCollection();
	}
	return _objcollection->find(id);
}

bool XRemoting::IPCLog(const std::string& msg) { 
	TLSObject *tlsobj = GetTLSObject2();
	return _logserver.SendLogMessage(msg, tlsobj? tlsobj->_index: -1); 
} 


void XRemoting::Apply(XRemotingSettings& remset) {
	size_t x;
	for(x = 0; x < remset._listeners2ignore.size(); ++x) {
		if(std::find(_listeners2ignore.begin(), _listeners2ignore.end(), remset._listeners2ignore[x]) == _listeners2ignore.end()) {
			_listeners2ignore.push_back(remset._listeners2ignore[x]);
		}
	}
	for(x = 0; x < remset._subscribers2ignore.size(); ++x) {
		if(std::find(_subscribers2ignore.begin(), _subscribers2ignore.end(), remset._subscribers2ignore[x]) == _subscribers2ignore.end()) {
			_subscribers2ignore.push_back(remset._subscribers2ignore[x]);
		}
	}
	for(x = 0; x < remset._changeable_addresses.size(); ++x) {
		if(std::find(_changeable_addresses.begin(), _changeable_addresses.end(), remset._changeable_addresses[x]) == _changeable_addresses.end()) {
			_changeable_addresses.push_back(remset._changeable_addresses[x]);
		}
	}
}

void XRemoting::GetSettings(XRemotingSettings& remset) {
	remset._changeable_addresses = _changeable_addresses;
	remset._listeners2ignore = _listeners2ignore;
	remset._subscribers2ignore = _subscribers2ignore;
}

std::string XRemoting::GetChangeableAddress(const std::string& name, int index) {
	std::string addr;
	std::vector<XChangeableAddress>::iterator it = std::find(_changeable_addresses.begin(), _changeable_addresses.end(), name);
	while(it != _changeable_addresses.end() && (*it)._index != index) {
		it = std::find(++it, _changeable_addresses.end(), name);
	}
	if(it != _changeable_addresses.end()) {
		addr = (*it)._addr;
	}
	return addr;
}

bool XRemoting::SetChangeableAddress(const std::string& name, const std::string& addr, int index) {
	bool rc = false;
	std::vector<XChangeableAddress>::iterator it = std::find(_changeable_addresses.begin(), _changeable_addresses.end(), name);
	while(it != _changeable_addresses.end() && (*it)._index != index) {
		it = std::find(++it, _changeable_addresses.end(), name);
	}
	if(it != _changeable_addresses.end()) {
		(*it)._addr = addr;
		rc = true;
	}
	return rc;
}



void XRemoting::Configure() {
	_g_trace = _trace;

	AddCollection(GetWsiClassFactoryRootCollection());

	size_t x = 0;
	while(x < _connectors.size()) {
		XConnector& conn = _connectors[x];
		if(std::find(_listeners2ignore.begin(), _listeners2ignore.end(), conn._listener._name) != _listeners2ignore.end()) {
			_connectors.erase(_connectors.begin() + x);
		}
		else {
			++x;
			size_t y = 0;
			while(y < conn._static_subscribers.size()) {
				XSubscriber& subscr = conn._static_subscribers[y];
				if(std::find(_subscribers2ignore.begin(), _subscribers2ignore.end(), subscr._name) != _subscribers2ignore.end()) {
					conn._static_subscribers.erase(conn._static_subscribers.begin() + y);
				}
				else {
					++y;
				}
			}
			size_t z = 0;
			while(z < conn._producers.size()) {
				XProducer& prodcr = conn._producers[z];
				if(std::find(_subscribers2ignore.begin(), _subscribers2ignore.end(), prodcr._name) != _subscribers2ignore.end()) {
					conn._producers.erase(conn._producers.begin() + z);
				}
				else {
					++z;
				}
			}
		}
	}

	std::vector<XConnector>::iterator conn = _connectors.begin();
	while(conn != _connectors.end()) {
		conn->Configure(this);
		++conn;
	}
}

int XRemoting::SetSecurity(int security_is_enabled) {
	int rc = _security_is_enabled;
	_security_is_enabled = security_is_enabled;
	std::vector<XConnector>::iterator conn = _connectors.begin();
	while(conn != _connectors.end()) {
		conn->_security_is_enabled = security_is_enabled;
		++conn;
	}
	return rc;
}


bool XRemoting::ChangeAddress(const std::string& name, const std::string& addr, int index, bool shutdown_if_same) {
	bool rc = false;
	std::vector<XConnector>::iterator conn = _connectors.begin();
	while(conn != _connectors.end()) {
		if(conn->ChangeAddress(name, addr, index, shutdown_if_same)) {
			rc = true;
		}
		++conn;
	}

	SetChangeableAddress(name, addr, index);

	return rc;
}


bool XRemoting::SendObject(const std::string& id, const void *obj, int subscr_index, bool mode, IWSIMarshaller *marsh) {
	std::vector<IConnector*>& temp = *_temp_pointers.get_object();
	//std::vector<IConnector*> temp;

	_sendobject_map.lock();

	datatype_map::iterator temp_iter = _sendobject_map.find(id);
	while(temp_iter != _sendobject_map.end()) {
		if((*temp_iter).first != id) {
			break;
		}
		temp.push_back((*temp_iter).second);
		++temp_iter;
	}

	_sendobject_map.unlock();

	bool rc = false;
	std::vector<IConnector*>::iterator iter = temp.begin();
	while(iter != temp.end()) {
		if((*iter)->SendObject(id, obj, subscr_index, mode, marsh)) {
			rc = true;
		}
		++iter;
	}

	temp.clear(); 

	_temp_pointers.return_object(&temp); 

	return rc;
}

bool XRemoting::ReturnObject(const std::string& id, const void *obj, LONG_PTR rpc_handle) {
	bool rc = false;

	XRPCHandle *rpc = (XRPCHandle*)rpc_handle;
	if(rpc && rpc->_rpc_outstring) {
		IWsiSerializerBase *serializer = FindObj(id);
		if(serializer) {
			std::string& wsi = *(std::string*)rpc->_rpc_outstring;
			serializer->lock();
			(*serializer)(rpc->_writer, obj, wsi); // 04/27/2009 XRPCHandle contains tuned CWSIWriter.
			serializer->unlock();
		}
	}

	return rc;
}

bool XRemoting::SetCallback(IWsiConsumerBase *obj) {
	bool rc = false;
	std::string& id = obj->_wsi_id;
	datatype_map::iterator iter = _setcallback_map.find(id);
	while(iter != _setcallback_map.end()) {
		if((*iter).first != id) {
			break;
		}
		rc = (*iter).second->SetCallback(obj);
		++iter;
	}

	return rc;
}

bool XRemoting::AddExceptionDelegate(IXFunction<XIPCObjectException, int> *obj) {
	bool rc = false;
	if(obj) {
		_onexception_delegates.Add(obj);
		rc = true;
	}
	return rc;
}

bool XRemoting::RemoveExceptionDelegate(IXFunction<XIPCObjectException, int> *obj) {
	bool rc = false;
	if(obj) {
		_onexception_delegates.Remove(obj);
		rc = true;
	}
	return rc;
}


void XRemoting::Start() {
	std::vector<XConnector>::iterator iter = _connectors.begin();
	while(iter != _connectors.end()) {
		(*iter).Start();
		++iter;
	}
}

void XRemoting::Shutdown() {
	std::vector<XConnector>::iterator iter = _connectors.begin();
	while(iter != _connectors.end()) {
		(*iter).Shutdown();
		++iter;
	}
}

void XRemoting::insert_datatype(datatype_map& a_map, const std::string& id, IConnector *conn) {
	a_map.lock();
	datatype_map::iterator iter = a_map.find(id);
	while(iter != a_map.end()) {
		if((*iter).second == conn) {
			break;
		}
		if((*iter).first != id) {
			iter = a_map.end();
		}
		else {
			++iter;
		}
	}
	if(iter == a_map.end()) {
		a_map.insert(datatype_map::value_type(id, conn));
	}
	a_map.unlock();
}

void XRemoting::Register_SendObject(const std::string& id, IConnector *conn) {
	insert_datatype(_sendobject_map, id, conn);
}

void XRemoting::Register_SetCallback(const std::string& id, IConnector *conn) {
	insert_datatype(_setcallback_map, id, conn);
}

bool XRemoting::GetChangeableAddresses(std::vector<std::string>& names, std::vector<std::string>& addresses, std::vector<std::string>& descriptions, std::vector<int>& indices) {
	bool rc = true;
	std::vector<XChangeableAddress>::iterator iter = _changeable_addresses.begin();
	while(iter != _changeable_addresses.end()) {
		names.push_back(iter->_name);
		addresses.push_back(iter->_addr);
		descriptions.push_back(iter->_description);
		indices.push_back(iter->_index);

		++iter;
	}

	return rc;
}

XRemoting::XRemoting(): _objcollection(0), _security_is_enabled(0) {
}

XRemoting::~XRemoting() {
	if(_objcollection) {
		delete _objcollection;
		_objcollection = 0;
	}
}




// XConnector

IWsiSerializerBase * XConnector::FindParser(const std::string& id) {
	if(!_parsecollection) {
		_parsecollection = new XParse::CWsiCollection();
	}
	return _parsecollection->find(id);
}

void XConnector::Start() {
	_listener.Start();

	std::vector<XSubscriber*>& temp = *_temp_subscribers.get_object();
	//std::vector<XSubscriber*> temp;
	build_temp_copy(temp, _subscribers, (const std::string*)0);

	std::vector<XSubscriber*>::iterator iter = temp.begin();
	while(iter != temp.end()) {
		if((*iter)->_writer.IsLocalAddress() && !(*iter)->_lazy_start) {
			(*iter)->_writer.Connect();
		}
		(*iter)->Release(); // build_temp_copy calls AddRef().
		++iter;
	}
	temp.clear(); 

	_temp_subscribers.return_object(&temp); 

	_started = true;
}

void XConnector::Shutdown() {
	std::vector<XProducer>::iterator prod = _producers.begin();
	while(prod != _producers.end()) {
		(*prod).CancelRequest();
		++prod;
	}

	_started = false;
	while(_activerequests_count) {
		OSWait();
	}

	std::vector<XSubscriber*>& temp = *_temp_subscribers.get_object();
	//std::vector<XSubscriber*> temp;
	build_temp_copy(temp, _subscribers, (const std::string*)0);

	std::vector<XSubscriber*>::iterator iter = temp.begin();
	while(iter != temp.end()) {
		(*iter)->_writer.Shutdown();
		if(!(*iter)->_static) {
			DeleteSubscriber(UniqueName((*iter)->_name, (*iter)->_addr, (*iter)->_index));
		}
		(*iter)->Release(); // build_temp_copy calls AddRef().
		++iter;
	}
	temp.clear(); 

	_temp_subscribers.return_object(&temp); 

	_listener.Shutdown();

}

XConnector::XConnector():
	_host_ptr(0), _started(false), _activerequests_count(0), _default_subscriber_idx(-1) {
	_callback_ptr = 0;
	_parsecollection = 0;
	_marshaller = 0;
	_security_is_enabled = 0;
}

XConnector::~XConnector() {
	if(_callback_ptr) {
		delete _callback_ptr;
		_callback_ptr = 0;
	}
	if(_parsecollection) {
		delete _parsecollection;
		_parsecollection = 0;
	}
	if(_marshaller) {
		delete _marshaller;
		_marshaller = 0;
	}
}


void XConnector::Configure(IRemotingObject *host_ptr) {
	_host_ptr = host_ptr;

//	_marshaller.Init();
	_marshaller = CreateIWSIMarshaller();

	IWsiSerializerBase *ws = _host_ptr->FindObj("HttpFormdata");
	if(ws) {
		ws->register_with((*_marshaller));
	}

	_callback_ptr = SetCallbackNewXSubscriber(this);

	std::vector<XSubscriberArray>::iterator arraysubscr_ptr = _array_subscribers.begin();
	while(arraysubscr_ptr != _array_subscribers.end()) {
		while(arraysubscr_ptr->_index++ < arraysubscr_ptr->_max_index) {
			_static_subscribers.push_back(*arraysubscr_ptr);
		}
		++arraysubscr_ptr;
	}

	std::vector<XSubscriber>::iterator subscr_ptr = _static_subscribers.begin();
	while(subscr_ptr != _static_subscribers.end()) {
		subscr_ptr->_static = 1;
		subscr_ptr->Initialize(this);
		NewSubscriber(&(*subscr_ptr));

		if(subscr_ptr->_is_default > 0) {
			_default_subscriber_idx = (int)(subscr_ptr - _static_subscribers.begin());
		}
		++subscr_ptr;
	}

	RegisterListener(_listener);

	std::vector<XProducerArray>::iterator arrayprod_ptr = _array_producers.begin();
	while(arrayprod_ptr != _array_producers.end()) {
		while(arrayprod_ptr->_index++ < arrayprod_ptr->_max_index) {
			_producers.push_back(*arrayprod_ptr);
		}
		++arrayprod_ptr;
	}

	std::vector<XProducer>::iterator producer_ptr = _producers.begin();
	while(producer_ptr != _producers.end()) {
		RegisterProducer(*producer_ptr);
		++producer_ptr;
	}
}

bool XConnector::ChangeAddress(const std::string& name, const std::string& addr, int index, bool shutdown_if_same) {
	bool rc = false;
	bool found = false; 
	std::vector<XSubscriber>::iterator subscr_ptr = _static_subscribers.begin();
	while(subscr_ptr != _static_subscribers.end()) {
		if((*subscr_ptr)._name == name && (*subscr_ptr)._index == index) {
			if((*subscr_ptr)._addr != addr) {
				XSubscriber& subscr = (*subscr_ptr);
				std::string new_addr = subscr._writer.BuildWellFormedAddress(addr);

				DeleteSubscriber(UniqueName(subscr._name, subscr._addr, subscr._index));

				subscr._addr = new_addr;
				NewSubscriber(&subscr);

				rc = true;
			}
			else
			if(shutdown_if_same) {
				XSubscriber& subscr = (*subscr_ptr);
				ShutdownSubscriber(UniqueName(subscr._name, subscr._addr, subscr._index));
			}
			found = true; 
		}
		++subscr_ptr;
	}

	std::vector<XProducer>::iterator prod = _producers.begin();
	while(prod != _producers.end()) {
		if((*prod)._name == name && (*prod)._index == index) {
			if(shutdown_if_same || (*prod)._addr != addr) {
				XProducer& producer = (*prod);
				std::string new_addr = producer._request._writer.BuildWellFormedAddress(addr);

				unRegisterProducer(producer);

				producer._request._addr = producer._addr;
				producer._addr = new_addr;

//				producer._status == IPCReq_NOTPROCESSED at this point.
//				Make sure it is closed and the host is notified about new address.
				producer.OnStatusChange(IPCReq_CLOSED);

				RegisterProducer(producer);
				if(addr.size() == 0) { 
					producer._status = IPCReq_CONFIRMED; 
				} 
				rc = true;
			}
			found = true;
		}
		++prod;
	}

	if(!found) {
		if(_default_subscriber_idx > -1 && _add_subscriber_ondemand) {
			XSubscriber clone = _static_subscribers[_default_subscriber_idx];

			clone._index = index;
			clone._writer = XWritingHead();
			clone._name = name;

			clone.Initialize(this);
			NewSubscriber(&clone);
			rc = ChangeAddress(name, addr, index, shutdown_if_same);

			clone._addr.resize(0);
		}
	}

	return rc;
}


void XConnector::RegisterListenerDatatypes(std::vector<std::string>& datatypes) {
	std::vector<std::string>::const_iterator type_ptr = datatypes.begin();
	while(type_ptr != datatypes.end()) {
		IWsiSerializerBase *ws = _host_ptr->FindObj(*type_ptr);
		if(ws) {
			ws->register_with((*_marshaller));
		}
		_host_ptr->Register_SetCallback(*type_ptr, this);

		++type_ptr;
	}
}

void XConnector::RegisterListener(XListener& listener) {
	listener.Initialize(this);

	RegisterListenerDatatypes(listener._datatypes);

	if(_name.size() == 0) {
		_name = listener._name;
	}
}


void XConnector::RegisterSubscriberDatatypes(std::vector<std::string>& datatypes) {
	std::vector<std::string>::const_iterator type_ptr = datatypes.begin();
	while(type_ptr != datatypes.end()) {
		IWsiSerializerBase *ws = _host_ptr->FindObj(*type_ptr);
		if(ws) {
			ws->register_with((*_marshaller));
		}
		_host_ptr->Register_SendObject(*type_ptr, this);

		++type_ptr;
	}
}

void XConnector::RegisterSubscriber(XSubscriber *subscr) {
	RegisterSubscriberDatatypes(subscr->_datatypes);
	RegisterListenerDatatypes(subscr->_returntypes);

	_sendobject_map.lock();

	std::vector<std::string>::const_iterator type_ptr = subscr->_datatypes.begin();
	while(type_ptr != subscr->_datatypes.end()) {
		_sendobject_map.insert(datatype_map::value_type(*type_ptr, subscr));
		++type_ptr;
	}

	_sendobject_map.unlock();
}


XSubscriber * XConnector::NewSubscriber(XSubscriber *x) {
	std::string key = UniqueName(x->_name, x->_addr, x->_index);

	_subscribers.lock();
#if _MSC_VER > 1700
	std::unordered_map<std::string, XSubscriber*>::iterator iter = FindMappedSubscriber(key);
#else 
	std::map<std::string, XSubscriber*>::iterator iter = FindMappedSubscriber(key);
#endif
	XSubscriber *subscr = 0;
	if(iter != _subscribers.end()) {
		subscr = (*iter).second;
	}
	_subscribers.unlock();

	if(subscr) {
		DeleteSubscriber(key);
	}

	if(x->_datatypes.size() == 0 && !x->_log_receiver) { // invalid subscriber - no data will be sent through it.
		return NULL;
	}

	subscr = new XSubscriber();
	if(!subscr) {
		return NULL;
	}
	*subscr = *x;

	subscr->Initialize(this);

	if(!subscr->_static) {
//		10/20/2008 - Server reuses existing connection instead of creating a new connection.
/*		if(!subscr->_writer.Connect()) {
			subscr->Release(); // will delete it
			return;
		} */
	}

	RegisterSubscriber(subscr);

	_subscribers.lock();
	bool ok = _subscribers.insert(subscribers_map::value_type(key, subscr)).second;
	_subscribers.unlock();

	if(!ok) {
		unRegisterSubscriber(subscr);
//		and delete it.
		if(!subscr->_static) {
			subscr->Release();
			subscr = 0;
		}
	}

	return subscr;
}

XSubscriber * XConnector::NewClient(const std::string& addr) {
	XSubscriber *subscr = 0;
	if(_default_subscriber_idx > -1) {
		XSubscriber clone = _static_subscribers[_default_subscriber_idx];

		if(clone._is_indexed) {
			clone._index = ++_generic_index;
		}

		clone._writer = XWritingHead();

		clone._addr = addr;
		clone._persistent = 0;
		clone._static = 0;
		clone._is_suspended = 1;

		clone.Initialize(this);

		subscr = NewSubscriber(&clone);

		clone._addr.resize(0);
	}
	return subscr;
}

XSubscriber * XConnector::NewSubscribedProducer(XProducer *x) {
	std::string key = UniqueName(x->_request._name, x->_request._addr, x->_request._index);

	_subscribers.lock();
#if _MSC_VER > 1700
	std::unordered_map<std::string, XSubscriber*>::iterator iter = FindMappedSubscriber(key);
#else
	std::map<std::string, XSubscriber*>::iterator iter = FindMappedSubscriber(key);
#endif
	XSubscriber *subscr = 0;
	if(iter != _subscribers.end()) {
		subscr = (*iter).second;
	}
	if(subscr) {
		subscr->AddDataTypes(x->_returntypes);
	}
	_subscribers.unlock();

	if(subscr) {
		return subscr;
	}

	x->_request.AddDataTypes(x->_returntypes);

	return NewSubscriber(&x->_request);
}

void XConnector::unRegisterSubscriber(XSubscriber *x) {
	_sendobject_map.lock();
	datatype_map::iterator temp_iter = _sendobject_map.begin();
	while(temp_iter != _sendobject_map.end()) {
		if((*temp_iter).second == x) {
			_sendobject_map.erase(temp_iter++);
		}
		else {
			++temp_iter;
		}
	}
	_sendobject_map.unlock();
}


#if _MSC_VER > 1700
std::unordered_map<std::string, XSubscriber*>::iterator XConnector::FindMappedSubscriber(const std::string& key) {
#else
std::map<std::string, XSubscriber*>::iterator XConnector::FindMappedSubscriber(const std::string& key) {
#endif
	return _subscribers.find(key);
}

#if _MSC_VER > 1700
std::unordered_map<std::string, XSubscriber*>::iterator XConnector::FindMappedSubscriber(const SOCKET& sock) {
	std::unordered_map<std::string, XSubscriber*>::iterator iter = _subscribers.begin();
#else 
std::map<std::string, XSubscriber*>::iterator XConnector::FindMappedSubscriber(const SOCKET& sock) {
	std::map<std::string, XSubscriber*>::iterator iter = _subscribers.begin();
#endif
	while(iter != _subscribers.end()) {
		if((*iter).second->_writer._target._sock == sock) { // found
			break;
		}
		++iter;
	}
	return iter;
}

template<typename Key>
bool XConnector::DeleteSubscriber(const Key& key, bool dynamic_only) {
	_subscribers.lock();

#if _MSC_VER > 1700
	std::unordered_map<std::string, XSubscriber*>::iterator iter = FindMappedSubscriber(key);
#else
	std::map<std::string, XSubscriber*>::iterator iter = FindMappedSubscriber(key);
#endif
	XSubscriber *subscr = 0;
	if(iter != _subscribers.end()) {
		subscr = (*iter).second;
	}
	if(subscr) {
		if(dynamic_only && subscr->_static) {
			subscr = 0;
		}
		else {
			_subscribers.erase(iter);
		}
	}
	_subscribers.unlock();

	bool rc = false;
	if(subscr) {
		unRegisterSubscriber(subscr);
		subscr->Release();
		rc = true;
	}

	return rc;
}

template<typename Key>
bool XConnector::ShutdownSubscriber(const Key& key) {
	bool rc = false;

	_subscribers.lock();

#if _MSC_VER > 1700
	std::unordered_map<std::string, XSubscriber*>::iterator iter = FindMappedSubscriber(key);
#else 
	std::map<std::string, XSubscriber*>::iterator iter = FindMappedSubscriber(key);
#endif
	XSubscriber *subscr = 0;
	if(iter != _subscribers.end()) {
		subscr = (*iter).second;
		if(subscr) {
			subscr->AddRef();
		}
	}

	_subscribers.unlock();

	if(subscr) {
		subscr->_writer.Shutdown();
		subscr->Release();
		rc = true;
	}

	return rc;
}

bool XConnector::ShutdownSubscribers(const std::string& addr) {
	std::vector<XSubscriber*> subscribers;
	bool rc = false;
	size_t addr_len = addr.size();

	_subscribers.lock();

#if _MSC_VER > 1700
	std::unordered_map<std::string, XSubscriber*>::iterator iter = _subscribers.begin();
#else
	std::map<std::string, XSubscriber*>::iterator iter = _subscribers.begin();
#endif
	while(iter != _subscribers.end()) {
		if((*iter).second->_addr.size() >= addr_len) {
			if(memcmp((*iter).second->_addr.data(), addr.data(), addr_len) == 0) { // found
				XSubscriber *subscr = (*iter).second;
				subscribers.push_back(subscr);
				subscr->AddRef();
			}
		}
		++iter;
	}

	_subscribers.unlock();

	for(size_t x = 0; x < subscribers.size(); ++x) {
		subscribers[x]->_writer.Shutdown();
		subscribers[x]->Release();
		rc = true;
	}

	return rc;
}


void Instantiate_XConnector_DeleteSubscriber(XConnector *obj) {
	obj->DeleteSubscriber(std::string());
	obj->DeleteSubscriber(SOCKET());
	obj->ShutdownSubscriber(std::string());
	obj->ShutdownSubscriber(SOCKET());
}




void XConnector::RegisterProducer(XProducer& producer) {
	std::string key = UniqueName(producer._name, producer._addr, producer._index);
	if(FindProducer(key)) {
//		producer exists.
		return;
	}

	producer.Initialize(this);

	RegisterListenerDatatypes(producer._datatypes);
	RegisterSubscriberDatatypes(producer._returntypes);

	XSubscriber& request = producer._request;

	request._datatypes.resize(0);
	if(request._parser_name.size() == 0) { 
		request._parser_name = _listener._parser_name;
	} 
	request._name = producer._name + _listener._name;
	request._addr = producer._addr;

	_dynamicproducers.lock();
	_dynamicproducers.insert(producers_map::value_type(key, &producer));
	_dynamicproducers.unlock();

	producer._freeze = 0;
}

void XConnector::unRegisterProducer(XProducer& producer) {
	std::string key = UniqueName(producer._name, producer._addr, producer._index);

	producer.CancelRequest();

	_dynamicproducers.lock();
	_dynamicproducers.erase(_dynamicproducers.find(key));
	_dynamicproducers.unlock();
}

XProducer * XConnector::FindProducer(const std::string& key) {
	_dynamicproducers.lock();

	XProducer *producer = 0;
	producers_map::iterator iter = _dynamicproducers.find(key);
	if(iter != _dynamicproducers.end()) {
		producer = (*iter).second;
	}

	_dynamicproducers.unlock();
	return producer;
}

void XConnector::PollProducers(std::vector<XSocket2Handler<XProducer> * >& new_clients) {
	_timeb checktime;
	_ftime(&checktime);

	InterlockedIncrement(&_activerequests_count);

	std::vector<XProducer*>& temp = *_temp_producers.get_object();
	//std::vector<XProducer*> temp;
	if(_started) {
		build_temp_copy(temp, _dynamicproducers, (const std::string*)0);
	}

	std::vector<XProducer*>::iterator iter = temp.begin();
	while(iter != temp.end()) {
		if(_started) {
			(*iter)->OnCheck(checktime, new_clients);
		}
		(*iter)->Release(); // build_temp_copy calls AddRef().
		++iter;
	}

	InterlockedDecrement(&_activerequests_count);

	temp.clear(); 

	_temp_producers.return_object(&temp); 
}

BOOL XConnector::CheckProducers() {
	BOOL needs_attention = FALSE;

	_dynamicproducers.lock();
	producers_map::iterator iter = _dynamicproducers.begin();
	while(iter != _dynamicproducers.end()) {
		if((*iter).second) {
			if((*iter).second->_status != IPCReq_CONFIRMED) {
				needs_attention = TRUE;
				break;
			}
			if((*iter).second->IsKeepAlive()) {
				needs_attention = TRUE;
				break;
			}
		}
		++iter;
	}
	_dynamicproducers.unlock();

	return (needs_attention && this->_started)? TRUE: FALSE;
}


bool XConnector::SendObject(const std::string& id, const void *obj, int subscr_index, bool synchronous, IWSIMarshaller *marsh) {
	if(!this->_started) {
		return false;
	}

	InterlockedIncrement(&_activerequests_count);

	std::vector<XSubscriber*>& temp = *_temp_subscribers.get_object();
	//std::vector<XSubscriber*> temp;
	build_temp_copy(temp, _sendobject_map, &id, subscr_index);

	bool rc = false;
	XWorkItem *wsi = new XWorkItem();

	std::vector<XSubscriber*>::iterator iter = temp.begin();
	while(iter != temp.end()) {
		XSubscriber *subscr = *iter;

		bool subscr_ok = true;

		if(!this->_started) {
			subscr_ok = false;
		}
		else
		if(subscr->_is_suspended) {
			subscr_ok = false;
		}
		else
		if(subscr->_index != subscr_index && subscr_index != -1) {
			subscr_ok = false;
		}
		else
		if(!subscr->_static && !subscr->_writer.IsValid()) {
			DeleteSubscriber(UniqueName(subscr->_name, subscr->_addr, subscr->_index));
			subscr_ok = false;
		}
		else
		if(!subscr->_format) {
			subscr_ok = false;
		}
		else
		if(subscr->_persistent && !subscr->_writer.IsValid()) {
			subscr_ok = subscr->_writer.Connect();
		}
		else
		if(subscr->_static && !subscr->_writer.IsValid() && synchronous) {
			subscr_ok = subscr->_writer.Connect();
		}
		else
		if(subscr->_static && !subscr->_writer.IsValid() && !synchronous) {
			subscr_ok = true;
		}
		else
		if(!subscr->_writer.IsValid()) {
			subscr_ok = false;
		}

		if(subscr_ok) {
			if(wsi->_format_name != subscr->_format->GetName() || wsi->_version != subscr->_version) {
				if(wsi->_format_name) {
					wsi->Release();
					wsi = new XWorkItem();
				}
				wsi->_format_name = subscr->_format->GetName();
				wsi->_version = subscr->_version;

				(*_marshaller)(*(subscr->_format), obj, id, *wsi->_pitem);
			}

			if(synchronous) {
				if(subscr->_writer.SendXWorkItem(wsi, marsh? marsh: _marshaller)) {
					rc = true;
				}
			}
			else {
				if(subscr->_writer.PostXWorkItem(wsi)) {
					rc = true;
				}
			}
		}
		subscr->Release(); // build_temp_copy calls AddRef().
		++iter;
	}

	wsi->Release();

	InterlockedDecrement(&_activerequests_count);

	temp.clear(); 

	_temp_subscribers.return_object(&temp); 

	return rc;
}

bool XConnector::SetCallback(IWsiConsumerBase *obj) {
	bool rc = true;
	obj->register_with((*_marshaller));
	return rc;
}




// XAddressable

void XAddressable::Initialize(XConnector *host_ptr) {
	_host_ptr = host_ptr;

	XRemoting *host = reinterpret_cast<XRemoting*>(_host_ptr->_host_ptr);
	if(_addr.size() == 0) {
		_addr = host->GetChangeableAddress(_name, _index);
	}
	else {
		host->SetChangeableAddress(_name, _addr, _index);
	}

	if(_parser_name.size() == 0) {
		_parser_name = "CWSIParser";
	}
}

void XAddressable::FireException(int obj_type, int obj_index, BOOL is_connected) {
	XIPCObjectException excpt;
	excpt._type = obj_type;
	excpt._index = obj_index;
	excpt._is_connected = is_connected;
	excpt._name = _name;
	excpt._addr = _addr;

	XRemoting *host = reinterpret_cast<XRemoting*>(_host_ptr->_host_ptr);
	if(host) {
		host->_onexception_delegates(excpt);
	}
}



// XListener

void XListener::Initialize(XConnector *host_ptr) {
	XAddressable::Initialize(host_ptr);

	_parser = 0;
	_format = 0;

	IWsiSerializerBase *ws = _host_ptr->FindParser(_parser_name);
	if(ws) {
		_parser = (ISTREAMReader*)ws->get_inner_ptr();
	}
	if(_parser) {
		_format = _parser->create_writer();
	}

	if(_max_parallel_requests == 0) {
		_max_parallel_requests = 16;
	}
	_apc_queue_depth = 0; 

	_reader.Initialize(this);
}

void XListener::Start() {
	_reader.Start();
}

void XListener::Shutdown() {
	_reader.Shutdown();
}




// XSubscriber

XSubscriber::XSubscriber() {
	_format = 0;
	_persistent = 0;
	_static = 0;
	_lazy_start = 0;
	_is_suspended = 0;
	_is_default = 0;
	_is_indexed = 0; 
	_no_keepalive = 0; 
	_in_buffer_size_kbytes = 0; 
	_out_buffer_size_kbytes = 0; 
	_is_nonblocking = 0; 
	_version = XXVERSION;
	_writer._initialized = false;
}
XSubscriber::~XSubscriber() {
	_writer.Shutdown();
	if(_format) {
		delete _format;
		_format = 0;
	}
}
void XSubscriber::Initialize(XConnector *host_ptr) {
	XAddressable::Initialize(host_ptr);

	IWsiSerializerBase *ws = _host_ptr->FindParser(_parser_name);
	if(ws) {
		_format = ((ISTREAMReader*)ws->get_inner_ptr())->create_writer();
		_format->SetVersion(_version);
		if(_format->GetName() == std::string(CWSIWriter().GetName())) {
			_writer_type = (int)'W';
		}
		else
		if(_format->GetName() == std::string(CCRFWriter().GetName())) {
			_writer_type = (int)'V';
		}
		else
		if(_format->GetName() == std::string(CXMLWriter().GetName())) {
			_writer_type = (int)'X';
		}
		else
		if(_format->GetName() == std::string(CJSONWriter().GetName())) {
			_writer_type = (int)'J';
		}
		else
		if(_format->GetName() == std::string(CFepWriter().GetName())) {
			_writer_type = (int)'F';
			_lazy_start = 1;
		}
		else
		if(_format->GetName() == std::string(CDartWriter().GetName())) {
			_writer_type = (int)'D';
			_lazy_start = 1;
		}
		else
		if(_format->GetName() == std::string(CHttpWriter().GetName())) {
			_writer_type = (int)'H';
		}
		else
		if(_format->GetName() == std::string(CRawWriter().GetName())) {
			_writer_type = (int)'R';
		}
		else {
			_writer_type = (int)' ';
		}
	}

	if(!_writer.IsValid()) {
		_writer.Initialize(this);
	}
}

void XSubscriber::AddDataTypes(const std::vector<std::string>& datatypes) {
	for(std::vector<std::string>::const_iterator type_ptr = datatypes.begin(); type_ptr != datatypes.end(); ++type_ptr) {
		if(std::find(_datatypes.begin(), _datatypes.end(), (*type_ptr)) == _datatypes.end()) {
			_datatypes.push_back(*type_ptr);
		}
	}
}


// XProducer

XProducer::XProducer(): _is_a_log_server(0) {
	_status = 0; 
	_freeze = 1; 
	memset(&_lastcheckpoint, 0, sizeof(_lastcheckpoint));
	_pxwitem = 0; 
}
XProducer::~XProducer() {
}

void XProducer::Initialize(XConnector *host_ptr) {
	XAddressable::Initialize(host_ptr);
	if(_addr.size() == 0) {
		_status = IPCReq_CONFIRMED;
	}

	_valid_request.resize(0);
	_invalid_request.resize(0);

	_request._name = _name + _host_ptr->_listener._name;
	_request._addr = _host_ptr->_listener._addr;
	_request._index = _index;
	_request._parser_name = _parser_name;

	_request.Initialize(_host_ptr);
}

void XProducer::BuildRequest() {
	std::string out_addr;
	if(!_request._writer.Connect(&out_addr)) {
		return;
	}

	XSubscriber extobj;

	extobj._datatypes.resize(0);

	extobj._name = (_name.size()? _name: _host_ptr->_listener._name) + "Inject";
	extobj._addr = out_addr;
	extobj._index = _index;
	extobj._parser_name = _request._parser_name;

	(*_host_ptr->_marshaller)(*(_request._format), &extobj, "XSubscriber", _invalid_request);

	std::vector<std::string>::const_iterator type_ptr = _datatypes.begin();
	while(type_ptr != _datatypes.end()) {
		extobj._datatypes.push_back(*type_ptr);
		++type_ptr;
	}

	for(type_ptr = _returntypes.begin(); type_ptr != _returntypes.end(); ++type_ptr) {
		extobj._returntypes.push_back(*type_ptr);
	}

	extobj._log_receiver = _is_a_log_server;

	(*_host_ptr->_marshaller)(*(_request._format), &extobj, "XSubscriber", _valid_request);
}

void XProducer::SendRequest(int request_type/*1 - subscribe, 2 - unsubscribe*/) {
	if(!_valid_request.size()) {
		BuildRequest();
	}
	else
	if(!_request._writer.Connect()) {
		request_type = 0;
	}
	if(!_pxwitem) { 
		_pxwitem = new XWorkItem(); 
	} 
	switch(request_type) {
		case 1: *_pxwitem->_pitem = _valid_request;
		break;
		case 2: *_pxwitem->_pitem = _invalid_request;
		break;
		default:
		_pxwitem->_pitem->resize(0);
		break;
	}
	if(_pxwitem->_pitem->size()) {
		_status = IPCReq_PROCESSING;
		if(!_request._writer.PostXWorkItem(_pxwitem)) {
			_status = IPCReq_NOTPROCESSED;
		}
		_ftime(&_lastcheckpoint);
	}
	else {
		_status = IPCReq_NOTPROCESSED;
	}
}

void XProducer::CancelRequest() {
	_freeze = 1;

	do {
		OSWait();
	}
	while(_status != IPCReq_NOTPROCESSED && _status != IPCReq_CONFIRMED && _host_ptr->_listener._reader._isrunning);

	if(_status == IPCReq_CONFIRMED) {
//		10/20/2008. It is not neccessary to send a cancelation request.
//		Just closing the socket will be sufficient to trigger an action on server's side.
//		SendRequest(2);
		if(_request._writer.Shutdown()) { 
			while(_status != IPCReq_NOTPROCESSED && _host_ptr->_listener._reader._isrunning) {
				OSWait();
			}
		} 
	}
	_status = IPCReq_NOTPROCESSED; 
	if(_pxwitem) { 
		_pxwitem->Release(); 
		_pxwitem = 0; 
	} 
} 

void XProducer::OnStatusChange(int new_status) {
	bool do_notify = false;

	if(new_status == _status) {
		return;
	}

	switch(new_status) {
		case IPCReq_CLOSED:
			do_notify = true;

		case IPCReq_NOTPROCESSED:
			if(_status == IPCReq_CONFIRMED) {
//				Server has disconnected.
//				Shutdown a possible static subscriber that points to same server.
				_host_ptr->ShutdownSubscribers(_request._addr);
				_host_ptr->DeleteSubscriber(_host_ptr->UniqueName(_request._name, _request._addr, _request._index));
				_request._datatypes.clear();
			}
			_request._writer.Shutdown();
			_status = IPCReq_NOTPROCESSED;
			break;

		case IPCReq_CONFIRMED:
			if(_returntypes.size()) {
				if(_request._datatypes.size() == 0) { // not added yet.
					_host_ptr->NewSubscribedProducer((XProducer*)this);
				}
			}
			do_notify = true;
//			10/20/2008 - Server reuses existing connection instead of creating a new connection.
//			_request._writer.Shutdown();

			_status = IPCReq_CONFIRMED;
			break;
	}

	if(do_notify) {
		FireException(2/*producer*/, _index, _status == IPCReq_CONFIRMED? TRUE: FALSE);
	}
	if(_pxwitem) { 
		_pxwitem->Release(); 
		_pxwitem = 0; 
	} 
}

//void XProducer::OnCheck(_timeb& checktime) {
void XProducer::OnCheck(_timeb& checktime, std::vector<XSocket2Handler<XProducer> * >& new_clients) {
	if((checktime.time - 1/*seconds*/) < _lastcheckpoint.time) {
		return;
	}

	if(_status == IPCReq_PROCESSING) {
		_status = _pxwitem->_status;
	}

	if(_status == IPCReq_NOTPROCESSED) {
		if(!_freeze) {
			SendRequest(1);
		}
	}

	if(_status != IPCReq_SENT) {
		if(_status == IPCReq_CONFIRMED && (_lastcheckpoint.time + 30) < checktime.time) {
			DoKeepAlive();
			_ftime(&_lastcheckpoint);
		}
		return;
	}

	if((checktime.time - 17/*seconds*/) > _lastcheckpoint.time) {
		OnStatusChange(IPCReq_NOTPROCESSED);
	}

//	10/20/2008 - Server reuses existing connection instead of creating a new connection.
	if(_request._writer.IsValid()) {
		new_clients.push_back(_request._writer.TearOffAHandler(this));
//		TearOffAHandler does not reset _writer's socket,
//		because CancelRequest needs to trigger an action on server's side.
//		An action on server's side is done by closing the socket.
		OnStatusChange(IPCReq_CONFIRMED);
	}
}
