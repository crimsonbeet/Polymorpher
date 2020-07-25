// Created by Alex Nikol Bourakov (ANB) around April/07

#include "stdafx_ipcobject.h"
#include "IPCObject.h"

#include "XRemoting.h"


// XRemoting functions that support IPCInterface.



int AddCollection(const IWsiCollection& collection, void *remoobj) {
	if(remoobj) {
		((XRemoting*)remoobj)->AddCollection(collection);
	}
	return 1;
}

IWsiSerializerBase* FindSerializer(const std::string& id, void *remoobj) {
	if(remoobj) {
		return ((XRemoting*)remoobj)->FindObj(id);
	}
	return NULL;
}



//int Initialize(const char *configuration, PIWsiCollection collection[], size_t nItems) {
//	std::for_each(collection, collection + nItems, _g_remoobj->AddCollection);

int Initialize(const char *configuration, void *obj, IWsiSerializerBase **extra, const int extra_cnt) {
	XRemoting *remoobj = (XRemoting*)obj;
	if(remoobj) {

		DeSerializeXRemotingObject(remoobj, configuration, extra, extra_cnt);

		remoobj->Configure();
	}
	return remoobj != 0;
}


int Serialize(std::string& configuration, void *obj, IWsiSerializerBase **extra, const int extra_cnt) {
	XRemoting *remoobj = (XRemoting*)obj;
	if(remoobj) {
		SerializeXRemotingObject(configuration, remoobj, extra, extra_cnt);
	}
	return remoobj != 0;
}


int JoinNetwork(void *remoobj) {
	int rc = 0;
	if(remoobj) {
		((XRemoting*)remoobj)->Start();
		rc = 1;
	}

	g_network_isactive = true;

	return rc;
}

int LeaveNetwork(void *remoobj) {
	int rc = 0;
	if(remoobj) {
		((XRemoting*)remoobj)->Shutdown();
		rc = 1;
	}

	g_network_isactive = false;

	return rc;
}

int SendObject(const char *id, void *obj, int subscr_index, bool mode, void *remoobj, IWSIMarshaller *marsh) {
	int rc = 0;
	if(remoobj && g_network_isactive) {
		rc = ((XRemoting*)remoobj)->SendObject(id, obj, subscr_index, mode, marsh);
	}
	return rc;
}

int ReturnObject(const char *id, const void *obj, LONG_PTR rpc_handle, void *remoobj) {
	int rc = 0;
	if(remoobj) {
		rc = ((XRemoting*)remoobj)->ReturnObject(id, obj, rpc_handle);
	}
	return rc;
}

int SetCallback(IWsiConsumerBase *api, void *remoobj) {
	int rc = 0;
	if(remoobj) {
		rc = ((XRemoting*)remoobj)->SetCallback(api);
	}
	return rc;
}

int AddExceptionDelegate(IXFunction<XIPCObjectException, int> *obj, void *remoobj) {
	int rc = 0;
	if(remoobj) {
		rc = ((XRemoting*)remoobj)->AddExceptionDelegate(obj);
	}
	return rc;
}

int RemoveExceptionDelegate(IXFunction<XIPCObjectException, int> *obj, void *remoobj) {
	int rc = 0;
	if(remoobj) {
		rc = ((XRemoting*)remoobj)->RemoveExceptionDelegate(obj);
	}
	return rc;
}


int IPCObjectLog(const std::string& msg, void *remoobj) {
	int rc = 1;
	if(remoobj) {
		rc = ((XRemoting*)remoobj)->_logserver.SendLogMessage(msg);
	}
	else {
		std::cout << msg;
	}

	return rc;
}

int ChangeAddress(const std::string& name, const std::string& addr, int index, bool shutdown_if_same, void *remoobj) {
	int rc = 0;
	if(remoobj) {
		rc = ((XRemoting*)remoobj)->ChangeAddress(name, addr, index, shutdown_if_same);
	}

	return rc;
}

int GetChangeableAddresses(std::vector<std::string>& names, std::vector<std::string>& addresses, std::vector<std::string>& descriptions, std::vector<int>& indices, void *remoobj) {
	int rc = 0;
	if(remoobj) {
		rc = ((XRemoting*)remoobj)->GetChangeableAddresses(names, addresses, descriptions, indices);
	}

	return rc;
}

int GetSettings(XRemotingSettings& remset, void *remoobj) {
	int rc = 0;
	if(remoobj) {
		((XRemoting*)remoobj)->GetSettings(remset);
		rc = 1;
	}

	return rc;
}

int IPCObjectDestroy(void *remoobj) {
	int rc = 1;
	if(remoobj) {
		delete (XRemoting*)remoobj;
		IPCObjectSetInterface(NULL);
	}
	return rc;
}


int SetTraceLevel(int trace_level) { 
	int old_trace = _g_trace; 
	_g_trace = trace_level; 
	return old_trace; 
} 

return_t RunXMarshRequestPipe(void *remoobj) {
	 return_t rc = 0;
	if(remoobj) {
		rc = ((XRemoting*)remoobj)->RunXMarshRequestPipe();
	}

	return rc;
}

int SetSecurity(int security_is_enabled, void *remoobj) {
	int rc = 0;
	if(remoobj) {
		rc = ((XRemoting*)remoobj)->SetSecurity(security_is_enabled);
	}
	return rc;
}


BOOL InitializeIPCInterface() {
	if(!InitializeSOCKInterface()) {
		LOGFailure(std::string("InitializeIPCInterface"), errno);
		return FALSE;
	}

//	local to the process
	IPCObjInterface *ipcinterface = new IPCObjInterface();
	ipcinterface->_remoobj = new XRemoting();

	ipcinterface->XAddCollection = AddCollection;
	ipcinterface->XFindSerializer = FindSerializer;
	ipcinterface->XInitialize = Initialize;
	ipcinterface->XSerialize = Serialize;
	ipcinterface->XJoinNetwork = JoinNetwork;
	ipcinterface->XLeaveNetwork = LeaveNetwork;
	ipcinterface->XSendObject = SendObject;
	ipcinterface->XReturnObject = ReturnObject;
	ipcinterface->XSetCallback = SetCallback;
	ipcinterface->XIPCLog = IPCObjectLog;
	ipcinterface->XChangeAddress = ChangeAddress;
	ipcinterface->XGetChangeableAddresses = GetChangeableAddresses;
	ipcinterface->XDestroy = IPCObjectDestroy;
	ipcinterface->XAddExceptionDelegate = AddExceptionDelegate;
	ipcinterface->XRemoveExceptionDelegate = RemoveExceptionDelegate;
	ipcinterface->XGetSettings = GetSettings;
	ipcinterface->XCreateIWSIMarshaller = CreateIWSIMarshaller;
	ipcinterface->XSetTraceLevel = SetTraceLevel; 
	ipcinterface->XRunXMarshRequestPipe = RunXMarshRequestPipe;
	ipcinterface->XSetSecurity = SetSecurity;


	IPCObjectSetInterface(ipcinterface);
	return TRUE;
}



