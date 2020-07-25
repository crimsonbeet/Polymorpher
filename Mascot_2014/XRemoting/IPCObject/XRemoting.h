// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef ipc_xremotingH
#define ipc_xremotingH

#include "XParse.h"

#include "XMedia.h"


#ifdef WSI_NAMESPACE
#undef WSI_NAMESPACE
#endif
#define WSI_NAMESPACE XRemote

#ifdef autocreateserialization_not_necessaryH
#undef autocreateserialization_not_necessaryH
#endif


#include ".\..\Include\WSIClassFactory.h"

#include ".\..\Include\WSIParser.h"
#include ".\..\Include\XMLParser.h"
#include ".\..\Include\XMLWriter.h"
#include ".\..\Include\JSONParser.h"
#include ".\..\Include\FepParser.h"
#include ".\..\Include\FepWriter.h"
#include ".\..\Include\DartParser.h"
#include ".\..\Include\DartWriter.h"
#include ".\..\Include\HttpParser.h"
#include ".\..\Include\HttpWriter.h"




template<typename T> 
struct thread_objects_map: public std::map<DWORD, T>, public wsi_gate { 
	T& get_object() { 
		DWORD mythread = OSThreadCurrent(); 
		lock(); 
		thread_objects_map::iterator it = find(mythread); 
		if(it == end()) { 
			it = insert(value_type(mythread, T())).first; 
		} 
		unlock(); 
		return (*it).second; 
	} 
}; 



class XAddressable: public CRefCounter {
public:
	std::string _name;
	std::string _addr; // 999.999.999.999:99999
	int _index; 

	std::vector<std::string> _datatypes;

	std::string _parser_name;

	XConnector *_host_ptr;

	void FireException(int obj_type, int obj_index, BOOL is_connected);

	void Initialize(XConnector *host_ptr);
};


class XListener: public XAddressable {
public:
	XReadingHead _reader;

	ISTREAMReader *_parser; // default parser
	ISTREAMWriter *_format; // default writer

	int _max_parallel_requests;
	int _is_time_critical; 
	int _apc_queue_depth; 

	void Initialize(XConnector *host_ptr);
	void Start();
	void Shutdown();
};


class XSubscriber: public XAddressable {
public:
	int _log_receiver;
	std::vector<std::string> _returntypes;
	XWritingHead _writer;

	ISTREAMWriter *_format;

	int _persistent;
	int _static;

	int _is_default; 
	int _is_indexed; 
	int _is_nonblocking; // only for use with default subscriber on accept. 
	int _no_keepalive; // supresses tcp keep-alive. 
	int _in_buffer_size_kbytes; 
	int _out_buffer_size_kbytes; 

	int _version;

	int _writer_type; // 'W' - wsi, 'X' - xml, 'J' - json, 'F' - fep, 'D' - dart, 'H' - http, 'R' - raw string. 
	int _lazy_start; 

	int _is_suspended; // work field. 

	TLSSecurityToken _stoken;

	void Initialize(XConnector *host_ptr);
	void AddDataTypes(const std::vector<std::string>& datatypes); 

	XSubscriber();
	~XSubscriber();
};


class XSubscriberArray: public XSubscriber {
public:
	int _max_index; 
};



class XProducer: public XAddressable {
	XWorkItem *_pxwitem;

public:
	std::vector<std::string> _returntypes;
	int _is_a_log_server;

	std::string _valid_request;
	std::string _invalid_request;

	XSubscriber _request; // A Connector owning XProducer is a subscriber in this case.
	int _status;
	int _freeze;
	_timeb _lastcheckpoint;


	void Initialize(XConnector *host_ptr);
	void BuildRequest();
	void SendRequest(int request_type);
	void CancelRequest();
	void OnStatusChange(int status);
	void OnCheck(_timeb& checktime, std::vector<XSocket2Handler<XProducer> * >& new_clients);

	XProducer();
	~XProducer();
};


class XProducerArray: public XProducer {
public:
	int _max_index;
};



class XConnector: public IConnector {
	XParse::CWsiCollection *_parsecollection;

//	typedef std::multimap<std::string, XSubscriber*> datatype_map;
	class datatype_map: public std::multimap<std::string, XSubscriber*>, public wsi_gate {
	};

	datatype_map _sendobject_map;

	cached_work_objects<std::vector<XSubscriber*> > _temp_subscribers; 
	cached_work_objects<std::vector<XProducer*> > _temp_producers; 

public:
	IWSIMarshaller *_marshaller;

//	use_clones if true, tells a serializer via marshaller to create a clone of an object on stack, unlock itself and then call a callback delegates. 
//	use_clones helps to achieve a full parallelism. 
	int _use_clones; 

	IRemotingObject *_host_ptr;

//	service function.
//	it makes a string in format 999.999.999.999:NAME.
	std::string UniqueName(const std::string& name, const std::string& addr, int index);

//	Serializable members.
public:
	std::string _name;
	int _responsetimeoutsecs; 

	std::vector<XSubscriber> _static_subscribers;
	int _default_subscriber_idx; 
	int _generic_index; 

	std::vector<XSubscriberArray> _array_subscribers; // only as input on config

	XListener _listener;
	std::vector<XProducer> _producers;

	std::vector<XProducerArray> _array_producers; // only as input on config

	void RegisterListener(XListener&);
	void RegisterListenerDatatypes(std::vector<std::string>& datatypes);

	CWSIParser _wsiparser;
	CXMLParser _xmlparser;
	CJSONParser _jsonparser;
	CFepParser _fepparser;
	CDartParser _dartparser;
	CHttpParser _httpparser;
	CRawParser _rawparser;


public:
	class subscribers_map: public std::map<std::string, XSubscriber*>, public wsi_gate {
//		key field has format 999.999.999.999:NAME
	};

	subscribers_map _subscribers;

	void RegisterSubscriber(XSubscriber*);
	void RegisterSubscriberDatatypes(std::vector<std::string>& datatypes);
	void unRegisterSubscriber(XSubscriber*);

	XSubscriber * NewSubscriber(XSubscriber*);
	XSubscriber * NewClient(const std::string& addr);

	std::map<std::string, XSubscriber*>::iterator FindMappedSubscriber(const std::string& key);
	std::map<std::string, XSubscriber*>::iterator FindMappedSubscriber(const SOCKET& sock);

	template<typename Key> bool DeleteSubscriber(const Key& key, bool dynamic_only = false);
	template<typename Key> bool ShutdownSubscriber(const Key& key);

	bool ShutdownSubscribers(const std::string& addr);

public:
	class producers_map: public std::map<std::string, XProducer*>, public wsi_gate {
//		key field has format 999.999.999.999:NAME
	};

	producers_map _dynamicproducers;

	void RegisterProducer(XProducer&);
	void unRegisterProducer(XProducer&);

	XSubscriber * NewSubscribedProducer(XProducer*);

	XProducer * FindProducer(const std::string& key);
	void PollProducers(std::vector<XSocket2Handler<XProducer> * >& new_clients);
	BOOL CheckProducers(); // checks if any producer needs attention (_status != IPCReq_CONFIRMED).


public:
	IWsiConsumerBase *_callback_ptr; // &XConnector::NewSubscriber

//	For API.
	bool SendObject(const std::string& wsi, const void *obj, int subscr_index, bool synchronous, IWSIMarshaller *marsh);
	bool SetCallback(IWsiConsumerBase *obj);

public:
	void Configure(IRemotingObject *host_ptr);
	void Start();
	void Shutdown();

	bool _started; 
	LONG _activerequests_count; 

	int _security_is_enabled;

public:
	XConnector();
	virtual ~XConnector();

	IWsiSerializerBase * FindParser(const std::string& id);

public:
	bool ChangeAddress(const std::string& name, const std::string& addr, int index = 0, bool shutdown_if_same = false);

};




class XLogServer {
public:
	std::string _name;
	int _my_port; 
	void HookSubscriber(XSubscriber& subscriber);
	bool SendLogMessage(const std::string& msg, int origin_port = -1);

	XLoggingHead _logger;

	void Reset();

	XLogServer();
	~XLogServer();
};



// Notifications:
// Producer connected. DONE: 2008/1/12.
// Producer/Subscriber disconnected (at the time when ShutdownSubscriber() is called in XProducer::OnStatusChange). DONE: 2008/1/12.
// ???Subscriber connected. Only when JoinNetwork() is called. It should be combined with other connected/disconnected statuses.
// Do as follows:
// new XIPCObjectException();
// QueueWorkItem(InvokeDelegates, exception); see XREmoting::_onexception_delegates.
//
// Send function can be extended with optional pointer to a vector of exceptions.
// JoinNetwork can also be extended same way.




class XRemoting: public IRemotingObject {
	XRemote::CWsiCollection *_objcollection;

public:
	void AddCollection(const IWsiCollection& coll);
	IWsiSerializerBase * FindObj(const std::string& id); // finds serializer for data type.
	bool IPCLog(const std::string& msg);
	return_t RunXMarshRequestPipe();

//	Serializable members.
public:
	std::string _base_directory;
	std::vector<XChangeableAddress> _changeable_addresses;

	std::string GetChangeableAddress(const std::string& name, int index = 0);
	bool SetChangeableAddress(const std::string& name, const std::string& addr, int index = 0);

	std::vector<std::string> _subscribers2ignore;
	std::vector<std::string> _listeners2ignore;

	void Apply(XRemotingSettings&);
	void GetSettings(XRemotingSettings&);

	int SetSecurity(int);

protected:
//	typedef std::multimap<std::string, IConnector*> datatype_map;
	class datatype_map: public std::multimap<std::string, IConnector*>, public wsi_gate {
	};

	datatype_map _setcallback_map;
	datatype_map _sendobject_map;

	void insert_datatype(datatype_map& a_map, const std::string& id, IConnector *conn);


public:
	XLogServer _logserver;
	std::vector<XConnector> _connectors;
	int _trace; 
	int _security_is_enabled;

	cached_work_objects<std::vector<IConnector*> > _temp_pointers; 

	IXDelegate<XIPCObjectException> _onexception_delegates;


	void Configure();
	void Start();
	void Shutdown(); 

//	For API.
	bool SendObject(const std::string& id, const void *obj, int subscr_index, bool synchronous, IWSIMarshaller *marsh);
	bool SetCallback(IWsiConsumerBase *obj);
	bool AddExceptionDelegate(IXFunction<XIPCObjectException, int> *obj);
	bool RemoveExceptionDelegate(IXFunction<XIPCObjectException, int> *obj);

//	RPC stuff.
//	ReturnObject sends responce back to the caller.
	bool ReturnObject(const std::string& id, const void *obj, LONG_PTR rpc_handle);


//	For Connectors only.
	void Register_SendObject(const std::string& id, IConnector *conn);
	void Register_SetCallback(const std::string& id, IConnector *conn);

	bool ChangeAddress(const std::string& name, const std::string& addr, int index = 0, bool shutdown_if_same = false);
	bool GetChangeableAddresses(std::vector<std::string>& names, std::vector<std::string>& addresses, std::vector<std::string>& descriptions, std::vector<int>& indices);

	XRemoting();
	virtual ~XRemoting();
};




class XVersion {
public:
	int _version;

	XVersion() {
		_version = 0;
	}
	XVersion(int version) {
		_version = version;
	}
};



// 04/28/2009 Borland can not link CWsiSerializer<XVersion>::Init() method defined in stdafx.obj, when used from outside of stdaxf.obj.
// The class below is to overcome this issue.

class XVersionSerializer: public CWsiSerializerBase {
protected:
	void Init() {
	}

public:
	XVersionSerializer(XVersion& inner):
	  CWsiSerializerBase(&inner, "XVersion", false) {
		contains_flat_member(&XVersion::_version, 0, "Version", this);
	}

	~XVersionSerializer() {
		remove_all();
	}
};



const std::string& GetXSubscriberWsiid();

IWsiConsumerBase * SetCallbackNewXSubscriber(XConnector *This);

void DeSerializeXRemotingObject(XRemoting *remoobj, const std::string& config, IWsiSerializerBase **extra = 0, const int extra_cnt = 0);
void SerializeXRemotingObject(std::string& config, XRemoting *remoobj, IWsiSerializerBase **extra = 0, const int extra_cnt = 0);

extern bool g_network_isactive;


#endif //ipc_xremotingH

