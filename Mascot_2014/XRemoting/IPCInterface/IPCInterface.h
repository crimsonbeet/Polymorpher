// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef ipcobjectinterfaceH
#define ipcobjectinterfaceH


// -- Software design is not a manufacturing no matter what management likes to think about it --
// -- Software design is a creative activity, craft, art or something along this line --


#ifndef EXP
#define EXP IMPORTIPCINTERFACE
#endif


void EXP __stdcall IPCDeleteAPIHandle(api_handle& handle);


struct IPCObjInterface {
	void *_remoobj;
	void *_threadpool;
	void *_redirectcout;

	int (*XAddCollection)(const IWsiCollection&, void*);
	IWsiSerializerBase* (*XFindSerializer)(const std::string&, void*);
	int (*XInitialize)(const char *configuration, void*, IWsiSerializerBase **extra, const int extra_cnt);
	int (*XSerialize)(std::string& configuration, void*, IWsiSerializerBase **extra, const int extra_cnt);
	int (*XJoinNetwork)(void*);
	int (*XLeaveNetwork)(void*);
	int (*XSendObject)(const char *id, void *obj, int subscr_index, bool synchronous/*false - asynchronous*/, void*, IWSIMarshaller *marsh);
	int (*XReturnObject)(const char *id, const void *obj, LONG_PTR rpc_handle, void*);
	int (*XSetCallback)(IWsiConsumerBase *api, void*);
	int (*XIPCLog)(const std::string& msg, void*);
	int (*XChangeAddress)(const std::string& name, const std::string& addr, int index, bool shutdown_if_same, void*);
	int (*XGetChangeableAddresses)(std::vector<std::string>& names, std::vector<std::string>& addresses, std::vector<std::string>& descriptions, std::vector<int>& indices, void*);
	int (*XGetSettings)(XRemotingSettings& remset, void*);
	int (*XDestroy)(void*);
	int (*XAddExceptionDelegate)(IXFunction<XIPCObjectException, int> *obj, void*);
	int (*XRemoveExceptionDelegate)(IXFunction<XIPCObjectException, int> *obj, void*);
	IWSIMarshaller* (*XCreateIWSIMarshaller)();
	int (*XSetTraceLevel)(int); 
	return_t (*XRunXMarshRequestPipe)(void*);
	int (*XSetSecurity)(int, void*);

	inline int AddCollection(const IWsiCollection& c) {
		return XAddCollection(c, _remoobj);
	}
	inline IWsiSerializerBase* FindSerializer(const std::string& id) {
		return XFindSerializer(id, _remoobj);
	}
	inline int Initialize(const char *c, IWsiSerializerBase **extra = 0, const int extra_cnt = 0) {
		return XInitialize(c, _remoobj, extra, extra_cnt);
	}
	inline int Serialize(std::string& c, IWsiSerializerBase **extra = 0, const int extra_cnt = 0) {
		return XSerialize(c, _remoobj, extra, extra_cnt);
	}
	inline int JoinNetwork() {
		return XJoinNetwork(_remoobj);
	}
	inline int LeaveNetwork() {
		return XLeaveNetwork(_remoobj);
	}
	inline int SendObject(const char *id, void *obj, int subscr_index, bool synchronous/*false - asynchronous*/, IWSIMarshaller *marsh = 0) {
		return XSendObject(id, obj, subscr_index, synchronous, _remoobj, marsh);
	}
	inline int ReturnObject(const char *id, const void *obj, LONG_PTR rpc_handle) {
		return XReturnObject(id, obj, rpc_handle, _remoobj);
	}
	inline int SetCallback(IWsiConsumerBase *api) {
		return XSetCallback(api, _remoobj);
	}
	inline int IPCLog(const std::string& msg) {
		return XIPCLog(msg, _remoobj);
	}
	inline int ChangeAddress(const std::string& name, const std::string& addr, int index = 0, bool shutdown_if_same = false) {
		return XChangeAddress(name, addr, index, shutdown_if_same, _remoobj);
	}
	inline int GetChangeableAddresses(std::vector<std::string>& names, std::vector<std::string>& addresses, std::vector<std::string>& descriptions, std::vector<int>& indices) {
		return XGetChangeableAddresses(names, addresses, descriptions, indices, _remoobj);
	}
	inline int GetSettings(XRemotingSettings& remset) {
		return XGetSettings(remset, _remoobj);
	}
	inline int Destroy() {
		return XDestroy(_remoobj);
	}
	inline int AddExceptionDelegate(IXFunction<XIPCObjectException, int> *obj) {
		return XAddExceptionDelegate(obj, _remoobj);
	}
	inline int RemoveExceptionDelegate(IXFunction<XIPCObjectException, int> *obj) {
		return XRemoveExceptionDelegate(obj, _remoobj);
	}
	inline IWSIMarshaller * CreateIWSIMarshaller() {
		return XCreateIWSIMarshaller();
	}
	inline int SetTraceLevel(int trace_level) {
		return XSetTraceLevel(trace_level);
	}
	inline return_t RunXMarshRequestPipe() { // Once been called from one thread, must be continued to be called from same thread. 
		return XRunXMarshRequestPipe(_remoobj); // Returns when being signalled; can be reentered afterward. 
	}
	inline int SetSecurity(int security_is_enabled) {
		return XSetSecurity(security_is_enabled, _remoobj);
	}
};

typedef IPCObjInterface *PIPCObjInterface;



PIPCObjInterface EXP __stdcall IPCObjectGetInterface();



return_t EXP __stdcall RedirectStandardOutput();
return_t EXP __stdcall RestoreStandardOutput();

void EXP __stdcall IPCSetLogHandler(HWND); //do pass HWND == 0 (or invalid file handler for non Windows) in order to remove handler. 
void EXP __stdcall IPCSetLogDelegate(int (*)(const std::string&)); //the delegate must not use IPCLog in any case and also std::cout when redirect is active. 
void EXP __stdcall IPCRemoveLogDelegate(int (*)(const std::string&)); 
BOOL EXP __stdcall IPCLog(const std::string& msg);

#define ADD_WSINAMESPACE(Namespace) IPCObjectGetInterface()->AddCollection(Namespace::CWsiRootCollection());


#endif //ipcobjectinterfaceH
