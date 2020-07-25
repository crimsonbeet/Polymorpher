// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef wsiclassfactoryinterfacesH
#define wsiclassfactoryinterfacesH

class IWsiSerializerBase; // forward declaration.
class IWsiConsumerBase; // forward declaration.
class IWSIMarshaller; // forward declaration.



#ifndef IMPORTIPCINTERFACE
#define IMPORTIPCINTERFACE
#endif

#ifndef EXP
#define EXP IMPORTIPCINTERFACE
#endif

#include "./../IPCClassFactory/ExportSTLMacro.h"
#include "./../IPCGate/IPCGate.h"





/*
 * WSIClassFactoryInterfaces begin here.
 */



class SIPCRawString { // it is supposed to be used with callback functions. 
protected: 
	friend class CRawParser;
	SIPCRawString(const std::string& str): _string_read(str) { 
	} 
public: 
	const std::string& _string_read; 
	std::string _string_write; 
	SIPCRawString(): _string_write(), _string_read(_string_write) { 
	} 
}; 



struct EXP IWSIObject {
	PVOID _obj;

	IWSIObject(PVOID obj = 0): _obj(obj) {
	}
};

struct EXP IWSIStackObject {
	PVOID _stobj;

	IWSIStackObject(PVOID stobj = 0): _stobj(stobj) {
	}
};


// Main interface class that is used by parser to build
// a stack of intermidiate wsi_objects.

class EXP
	IWSIMaterializer {
public:
	virtual void reserve(size_t n) = 0;
	virtual void release() = 0;
//	virtual void place_composite_object_on_stack(wsi_object& obj) = 0;
	virtual void place_composite_object_on_stack(const IWSIObject& obj) = 0;
	virtual void process_flat_object(const IWSIObject& obj) = 0;
	virtual void process_raw_string(const SIPCRawString& obj) = 0;
	virtual void reduce_stack() = 0;
	virtual void flush() = 0;
//	virtual wsi_object& get_current() = 0;
	virtual IWSIObject get_current() = 0;
	virtual IWSIObject get_context_current() = 0;
	virtual ~IWSIMaterializer() {
	}
};



class EXP
	ISTREAMWriter: public wsi_gate {
public:
	virtual const char * GetName() = 0;
	virtual bool IsHumanReadableFormat() { return true; }
	virtual int Encoding() { return 0; } // 0 - no encoding, 1 - url(%) encoding, 2 - xml(&) encoding, 4 - json(\uXXXX) encoding. 
	virtual void SetHumanReadableFormat(bool value) {} 
	virtual void SetVersion(int version) {} 
	virtual int Version() { return 1; } 
	virtual bool PutAll(bool) { return true; } 

	virtual void BeginFile(const std::string& id) {}
	virtual void BeginObject(const std::string& id, int index, bool is_composite) = 0;
	virtual void AddAttribute(const std::string& id, const std::string& value, int index, bool out_of_order) = 0;
	virtual void EndIterator(const std::string& id, int elements_count) = 0;
	virtual void EndObject(const std::string& id, int index, bool is_composite, int nElements) = 0; 
	virtual void EndFile(const std::string& id) {}

	virtual void Append(const std::string& str) {} 
	virtual void Append(char c) {} 

	virtual void Initialize(const std::string& s) = 0;

	virtual void ToString(std::string& s) = 0;

	virtual void Reset() = 0;

	virtual long GetPos() = 0;

	virtual void SetPos(long pos) = 0;

	virtual ~ISTREAMWriter() {
	}

};

#define XXVERSION 3



class EXP
	ISTREAMReader {
public:
	virtual void parse(const std::string&, IWSIMaterializer&) const = 0;
	virtual void parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer&) const = 0;
	virtual ISTREAMWriter * create_writer() const = 0;
	//virtual bool SetValidator() { 
	//	return false; // returns false when not implemented. 
	//} 
	virtual int Encoding() const { return 0; } // 0 - no encoding, 1 - url(%) encoding, 2 - xml(&) encoding, 4 - json(\uXXXX) encoding

	virtual ~ISTREAMReader() {
	}
};






class EXP
	IWSIMarshaller {
public:
	virtual void Init() = 0;
	virtual int Encoding() const = 0; // 0 - no encoding, 1 - url(%) encoding, 2 - xml(&) encoding, 4 - json(\uXXXX) encoding

	// Materializer calls these methods to invoke registered serializers.
	// See add_callee method.
	virtual void operator()(const IWSIStackObject&, LONG_PTR = 0, BOOL use_clones = FALSE) = 0;
	virtual void operator()(const IWSIObject&, LONG_PTR = 0, BOOL use_clones = FALSE) = 0;

	// WsiSerializerBase calls this function when
	// it elects to process wsi_objects.
	virtual bool add_callee(IWsiSerializerBase* t, const std::string& wsi_id) = 0;
	// This function must be called by WsiSerializerBase when
	// it goes out of scope.
	virtual void remove_callee(IWsiSerializerBase* t) = 0;


	// Serializer calls this method to invoke registered consumers, when
	// a new object has been created.
	virtual void operator()(const std::string& name, void* obj, LONG_PTR = 0) = 0;

	// WsiConsumerBase calls this function when
	// it wants to get notified when an
	// event 'new-object' is fired.
	virtual bool add_callee(IWsiConsumerBase* t, const std::string& wsi_id) = 0;
	// This function is called by WsiConsumerBase when
	// it goes out of scope.
	virtual void remove_callee(IWsiConsumerBase* t) = 0;

//	Main interface for class Connector.
	virtual void operator()(const ISTREAMReader& parser, const std::string& wsi, LONG_PTR rpc_handle = 0, BOOL use_clones = FALSE) = 0;
	virtual void operator()(const ISTREAMReader& parser, std::string::const_iterator from, std::string::const_iterator to, LONG_PTR rpc_handle = 0, BOOL use_clones = FALSE) = 0;
	virtual void operator()(ISTREAMWriter& writer, const void *obj, const std::string& wsi_id, std::string& wsi) = 0;

	virtual ~IWSIMarshaller() {
	}

};

EXP IWSIMarshaller * CreateIWSIMarshaller();

EXP void DestroyIWSIMarshaller(IWSIMarshaller *marsh);





struct EXP XRPCHandle {
	LONG_PTR _marshaller;
	LONG_PTR _rpc_outstring; 
	LONG_PTR _rpc_socket;
	LONG_PTR _remoting_object;

	ISTREAMWriter& _writer;

	bool InvokeMarshaller(ISTREAMReader& reader, const std::string& inp_str, ISTREAMWriter *writer/*can be zero*/, std::string& out_str); 
	bool InvokeMarshaller(ISTREAMReader& reader, std::string::const_iterator from, std::string::const_iterator to); // only to parse the input string and populate the root object.
	XRPCHandle(ISTREAMWriter& writer);
};





//EXPORT_STL_SET(IWSIMarshaller*)

class EXP IWSICallable {
public:
	typedef std::set<IWSIMarshaller*> marshaller_cookies;
	marshaller_cookies *_pcookies;

public:
	std::string _wsi_id;
	long int _in_use;

	// This function gets called by WSICallback when
	// WSICallback gets destroyed.
	void remove_bookmark(IWSIMarshaller* mat);

	// This function gets called by WSICallback when
	// add_callee is called.
	void add_bookmark(IWSIMarshaller* mat);

	virtual void lock(); 
	virtual void unlock(); 

	IWSICallable(); 
	virtual ~IWSICallable(); 
};




class EXP IWsiSerializerBase: public IWSICallable {
protected:
	void *_inner_ptr;

	inline void * set_inner_ptr(void *inner_ptr) {
		return _inner_ptr = inner_ptr;
	}

	friend class IWsiArraySerializer;
	friend class IWsiVectorSerializer;

public:
	inline void * get_inner_ptr() {
		return _inner_ptr;
	}

	virtual void set_outer_ptr(void *obj) = 0;

	void register_with(IWSIMarshaller& mat);
	void remove_all();


	virtual IWsiSerializerBase* Create() const;
	virtual IWsiSerializerBase* CreateEmpty() const;
	virtual ~IWsiSerializerBase() {
		remove_all();
	}

	// These functions are called by parser.

	// This function converts wsi_object to plain members of associated class.
	virtual void operator()(const IWSIObject&, LONG_PTR = 0, IWSIMarshaller *current_host = NULL, BOOL dolock = FALSE, BOOL use_clones = FALSE) = 0;
	// This function converts top wsi_object to plain members of associated class.
	// It also converts to complex members the wsi_objects that follow.
	virtual void operator()(const IWSIStackObject&, LONG_PTR = 0, IWSIMarshaller *current_host = NULL, BOOL dolock = FALSE, BOOL use_clones = FALSE) = 0;

public:
	// It returns a string image of an object.
	virtual void operator()(ISTREAMWriter&, const void *obj, std::string&) = 0;

private:
	void invoke_callback(const std::string& name, void *obj, LONG_PTR = 0);

protected:
	void onNewObject(const std::string& name, void *obj, LONG_PTR rpc_handle = 0, IWSIMarshaller *current_host = NULL);

	// if not overriden, it just calls onNewObject with same input parameters. 
	virtual BOOL onNewObjectClone(const std::string& name, void *obj, LONG_PTR rpc_handle = 0, IWSIMarshaller *current_host = NULL, BOOL locked = FALSE);
};

//EXP void DestroyIWsiSerializerBase(IWsiSerializerBase *iwsisb);




class EXP IWsiConsumerBase: public IWSICallable {
public:
	void register_with(IWSIMarshaller& mat);
	void remove_all();

	virtual ~IWsiConsumerBase() {
		remove_all();
	}
	// This function is called by Serializer through IWSIMarshaller.

	// It is an event handler.
	// It is invoked when a new object has been created.
	virtual void operator()(void *obj, LONG_PTR = 0) = 0;
};

//EXP void DestroyIWsiConsumerBase(IWsiConsumerBase *iwsicb);



EXP void DestroyIWSICallable(IWSICallable *iwsi); // only for use with for_each algorithm to delete unused objects.



class EXP
	IWsiCollection {
public:
	typedef std::map<std::string, IWsiSerializerBase*> object_serializers;
	virtual const object_serializers& Get() const = 0;
	static void m_insert(object_serializers& map, IWsiSerializerBase* serializer);
	virtual ~IWsiCollection() {
	}
};

typedef IWsiCollection *PIWsiCollection;


class EXP
	IConnector {
public:
//	For API.
	virtual bool SendObject(const std::string& id, const void *obj, int subscr_index, bool synchronous, IWSIMarshaller *marsh) = 0;
	virtual bool SetCallback(IWsiConsumerBase *obj) = 0;
	virtual ~IConnector() {
	}
};


class EXP
	IRemotingObject: public IConnector {
public:
	virtual IWsiSerializerBase * FindObj(const std::string& id) = 0;
	virtual bool IPCLog(const std::string& msg) = 0;

//	For Connectors only.
	virtual void Register_SendObject(const std::string&, IConnector*) = 0;
	virtual void Register_SetCallback(const std::string&, IConnector*) = 0;
};




// Helper templates that automate creation of callback objects.


template <class T>
EXP
IWsiSerializerBase& GetRootSerializer(const T *p);

#if defined(_UCC)
template<> EXP IWsiSerializerBase& GetRootSerializer<SIPCRawString>(const SIPCRawString *p);
#endif



template <class Base, typename Arg>
class CWSIMemCallback: public IWsiConsumerBase {
	void (Base::*Fn)(Arg*);
	Base *Bc;

public:
	CWSIMemCallback(void (Base::*f)(Arg*), Base *b = 0, const std::string& wsi_id = ""):
		Fn(f),
		Bc(b) {
		_wsi_id = wsi_id;
	}

	void set_base(Base *b) {
		Bc = b;
	}

	void operator()(void* obj, LONG_PTR) {
		(Bc->*Fn)((Arg*)obj);
	}
};


template <typename Arg>
class CWSICallback: public IWsiConsumerBase {
	void (*Fn)(Arg*);

public:
	CWSICallback(void (*f)(Arg*), const std::string& wsi_id):
		Fn(f) {
		_wsi_id = wsi_id;
	}

	void operator()(void* obj, LONG_PTR) {
		(Fn)((Arg*)obj);
	}
};


EXP void BindAPITriplet(IWsiConsumerBase *c, IWsiSerializerBase *s, IWSIMarshaller *m); // All three must be nonzero.


template <class Base, typename Arg>
CWSIMemCallback<Base, Arg> *
CreateAPICallback(void (Base::*f)(Arg*), Base *b, IWSIMarshaller *m = 0, IWsiSerializerBase *ser = 0) {
	if(ser == 0) {
		ser = &GetRootSerializer((Arg*)0);
	}
	CWSIMemCallback<Base, Arg> *res =
		new CWSIMemCallback<Base, Arg>(f, b, ser->_wsi_id);

	BindAPITriplet(res, ser, m);
	return res;
}


template <typename Arg>
CWSICallback<Arg> *
CreateAPICallback(void (*f)(Arg*), IWSIMarshaller *m = 0, IWsiSerializerBase *ser = 0) {
	if(ser == 0) {
		ser = &GetRootSerializer((Arg*)0);
	}
	CWSICallback<Arg> *res =
		new CWSICallback<Arg>(f, ser->_wsi_id);

	BindAPITriplet(res, ser, m);
	return res;
}


#endif //wsiclassfactoryinterfacesH
