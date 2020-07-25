// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef rpccalltemplatesH 
#define rpccalltemplatesH 


// Helper templates that are used by SetRpcCallee interface template. 
// RpcCalee is a function that returns an object on stack, or a pointer to an object, 
// therefore there are 2 flawors of templates. 
// RpcCalee is called via QueryObject interface template. 


template<class T> 
int ReturnObject2(const T *obj, LONG_PTR rpc_handle) { 
	int rc = 0; 
	IPCObjInterface *r = IPCObjectGetInterface(); 
	if(r && r->XReturnObject && obj) { 
		rc = r->ReturnObject(GetRootSerializer(obj)._wsi_id.c_str(), obj, rpc_handle); 
	} 
	return rc; 
} 

template<class T> 
inline int ReturnObject(const T& obj, LONG_PTR rpc_handle) { 
	return ReturnObject2(&obj, rpc_handle); 
} 


// Flawor 1: Returns an object on stack. 

template <class Base, typename Arg, typename Rval> 
class CWSIMemRpcCall: public IWsiConsumerBase { 
	Rval (Base::*Fn)(Arg*); 
	Base *Bc; 

public: 
	CWSIMemRpcCall(Rval (Base::*f)(Arg*), Base *b, const std::string& wsi_id): 
		Bc(b), 
		Fn(f) { 
		_wsi_id = wsi_id; 
	} 

	void operator()(void* obj, LONG_PTR rpc_handle) { 
		ReturnObject((Bc->*Fn)((Arg*)obj), rpc_handle); 
	} 
}; 

template <typename Arg, typename Rval> 
class CWSIRpcCall: public IWsiConsumerBase { 
	Rval (*Fn)(Arg*); 

public: 
	CWSIRpcCall(Rval (*f)(Arg*), const std::string& wsi_id): 
		Fn(f) { 
		_wsi_id = wsi_id; 
	} 

	void operator()(void* obj, LONG_PTR rpc_handle) { 
		ReturnObject((Fn)((Arg*)obj), rpc_handle); 
	} 
}; 


template <class Base, typename Arg, typename Rval> 
CWSIMemRpcCall<Base, Arg, Rval> * 
CreateRpcCallee(Rval (Base::*f)(Arg*), Base *b, IWSIMarshaller *m = 0, IWsiSerializerBase *ser = 0) { 
	if(ser == 0) { 
		ser = &GetRootSerializer((Arg*)0); 
	} 
	CWSIMemRpcCall<Base, Arg, Rval> *res = 
		new CWSIMemRpcCall<Base, Arg, Rval>(f, b, ser->_wsi_id); 

	BindAPITriplet(res, ser, m); 
	return res; 
} 


template <typename Arg, typename Rval> 
CWSIRpcCall<Arg, Rval> * 
CreateRpcCallee(Rval (*f)(Arg*), IWSIMarshaller *m = 0, IWsiSerializerBase *ser = 0) { 
	if(ser == 0) { 
		ser = &GetRootSerializer((Arg*)0); 
	} 
	CWSIRpcCall<Arg, Rval> *res = 
		new CWSIRpcCall<Arg, Rval>(f, ser->_wsi_id); 

	BindAPITriplet(res, ser, m); 
	return res; 
} 





// Flawor 2: Returns a pointer to an object. 

template <class Base, typename Arg, typename Rval> 
class CWSIMemRpcCall2: public IWsiConsumerBase { 
	Rval* (Base::*Fn)(Arg*); 
	Base *Bc; 

public: 
	CWSIMemRpcCall2(Rval* (Base::*f)(Arg*), Base *b, const std::string& wsi_id): 
		Bc(b), 
		Fn(f) { 
		_wsi_id = wsi_id; 
	} 

	void operator()(void* obj, LONG_PTR rpc_handle) { 
		static wsi_gate gate; 
		gate.lock(); 
		ReturnObject2((Bc->*Fn)((Arg*)obj), rpc_handle); 
		gate.unlock(); 
	} 
}; 

template <typename Arg, typename Rval> 
class CWSIRpcCall2: public IWsiConsumerBase { 
	Rval* (*Fn)(Arg*); 

public: 
	CWSIRpcCall2(Rval* (*f)(Arg*), const std::string& wsi_id): 
		Fn(f) { 
		_wsi_id = wsi_id; 
	} 

	void operator()(void* obj, LONG_PTR rpc_handle) { 
		static wsi_gate gate; 
		gate.lock(); 
		ReturnObject2((Fn)((Arg*)obj), rpc_handle); 
		gate.unlock(); 
	} 
}; 


template <class Base, typename Arg, typename Rval> 
CWSIMemRpcCall2<Base, Arg, Rval> * 
CreateRpcCallee(Rval* (Base::*f)(Arg*), Base *b, IWSIMarshaller *m = 0, IWsiSerializerBase *ser = 0) { 
	if(ser == 0) { 
		ser = &GetRootSerializer((Arg*)0); 
	} 
	CWSIMemRpcCall2<Base, Arg, Rval> *res = 
		new CWSIMemRpcCall2<Base, Arg, Rval>(f, b, ser->_wsi_id); 

	BindAPITriplet(res, ser, m); 
	return res; 
} 


template <typename Arg, typename Rval> 
CWSIRpcCall2<Arg, Rval> * 
CreateRpcCallee(Rval* (*f)(Arg*), IWSIMarshaller *m = 0, IWsiSerializerBase *ser = 0) { 
	if(ser == 0) { 
		ser = &GetRootSerializer((Arg*)0); 
	} 
	CWSIRpcCall2<Arg, Rval> *res = 
		new CWSIRpcCall2<Arg, Rval>(f, ser->_wsi_id); 

	BindAPITriplet(res, ser, m); 
	return res; 
} 



api_handle EXP _SetCallback(IWsiConsumerBase *api); 

int EXP _QueryObject(void *query, const std::string& wsi_id, int subscr_index, void *rval, IWsiSerializerBase *pserializer, void *rval2, IWsiSerializerBase *pserializer2, IWSIMarshaller **ppmarshaller); 



//----------------------------------------------------------------------------




// Templates that establish the actual interface begin here. 
// The following functions use IPCInterface to call IPCObject. 



template<class T> 
EXP 
int SendObject(const T& obj, int subscr_index = 0/*specific index (as opposed to PostObject)*/) { 
	int rc = 0; 
	IPCObjInterface *r = IPCObjectGetInterface(); 
	if(r && r->XSendObject) { 
		rc = r->SendObject(GetRootSerializer((T*)0)._wsi_id.c_str(), (PVOID)&obj, subscr_index, true/*i.e. synchronous*/); 
	} 
	return rc; 
} 

template<class T> 
EXP 
int PostObject(const T& obj, int subscr_index = -1/*any index subscribed*/) { 
	int rc = 0; 
	IPCObjInterface *r = IPCObjectGetInterface(); 
	if(r && r->XSendObject) { 
		rc = r->SendObject(GetRootSerializer((T*)0)._wsi_id.c_str(), (PVOID)&obj, subscr_index, false/*i.e. Asynchronous*/); 
	} 
	return rc; 
} 

template<class T> 
EXP 
api_handle SetCallback(void (*f)(T*)) { 
	return _SetCallback(CreateAPICallback(f)); 
} 

template<class B, typename T> 
EXP 
api_handle SetCallback(void (B::*f)(T*), B *b) { 
	return _SetCallback(CreateAPICallback(f, b)); 
} 


template<class T, typename R> 
EXP 
api_handle SetRpcCallee(R (*f)(T*)) { 
	return _SetCallback(CreateRpcCallee(f)); 
} 

template <class B, typename T, typename R> 
EXP 
api_handle SetRpcCallee(R (B::*f)(T*), B *b) { 
	return _SetCallback(CreateRpcCallee(f, b)); 
} 


template<class T, typename R> 
EXP 
api_handle SetRpcCallee(R* (*f)(T*)) { 
	return _SetCallback(CreateRpcCallee(f)); 
} 

template <class B, typename T, typename R> 
EXP 
api_handle SetRpcCallee(R* (B::*f)(T*), B *b) { 
	return _SetCallback(CreateRpcCallee(f, b)); 
} 


template<typename Q> 
EXP 
int QueryObject(Q& query, int subscr_index = 0) { 
	static IWsiSerializerBase *pserializer = GetRootSerializer((Q*)0).CreateEmpty();
	static IWSIMarshaller *pmarshaller(0); 

	return _QueryObject((PVOID)&query, GetRootSerializer((Q*)0)._wsi_id, subscr_index, &query, pserializer, 0, 0, &pmarshaller);
} 

template<typename Q, typename R> 
EXP 
int QueryObject(const Q& query, R& rval, int subscr_index = 0) { 
	static IWsiSerializerBase *pserializer = GetRootSerializer((R*)0).CreateEmpty();
	static IWSIMarshaller *pmarshaller(0); 

	return _QueryObject((PVOID)&query, GetRootSerializer((Q*)0)._wsi_id, subscr_index, &rval, pserializer, 0, 0, &pmarshaller);
} 

template<typename Q, typename R1, typename R2> 
EXP 
int QueryObject(const Q& query, R1& rval1, R2& rval2, int subscr_index = 0) { 
	static IWsiSerializerBase *pserializer1 = GetRootSerializer((R1*)0).CreateEmpty();
	static IWsiSerializerBase *pserializer2 = GetRootSerializer((R2*)0).CreateEmpty();
	static IWSIMarshaller *pmarshaller(0); 

	return _QueryObject((PVOID)&query, GetRootSerializer((Q*)0)._wsi_id, subscr_index, &rval1, pserializer1, &rval2, pserializer2, &pmarshaller);
} 


#endif //rpccalltemplatesH
