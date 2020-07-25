// stdafx.cpp : source file that includes just the standard includes
// IPCInterface.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

// Created by Alex Nikol Bourakov (ANB) around April/07

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "stdafx_interface.h"

#include "RPCTemplates.h"


PIPCObjInterface _g_wsiremoting = 0;

extern HWND _g_loghandler;
extern IXDelegate<const std::string> _g_loghandler_delegates;

BOOL EXP __stdcall OUTPUTLogMessage(const std::string& msg); 



void EXP __stdcall IPCObjectSetInterface(IPCObjInterface *obj) {
	if(_g_wsiremoting) {
		delete _g_wsiremoting;
	}
	_g_wsiremoting = obj;
}

BOOL __stdcall IPCLogEx(const std::string& msg, BOOL no_cout = FALSE) { 
	BOOL ok = TRUE; 
	if(_g_wsiremoting) { 
		ok = _g_wsiremoting->IPCLog(msg); 
	} 
	else 
	if(_g_loghandler) { 
		ok = OUTPUTLogMessage(msg); 
	} 
	else 
	if(_g_loghandler_delegates.Size() == 0 || !_g_loghandler_delegates(msg)) { 
		if(!no_cout) { 
			std::cout << msg << std::endl; 
		} 
	} 

	return ok; 
} 

BOOL EXP __stdcall IPCLog(const std::string& msg) {
	return IPCLogEx(msg, NO_DEFAULT_COUT);
}

void EXP __stdcall IPCDeleteAPIHandle(api_handle& handle) {
	if(handle._api) {
//		DestroyIWsiConsumerBase(reinterpret_cast<IWsiConsumerBase*>(handle._api));
		delete reinterpret_cast<IWsiConsumerBase*>(handle._api);
		handle._api = 0;
	}
}

api_handle EXP _SetCallback(IWsiConsumerBase *api) {
	api_handle handle;
	handle._api = 0;
	IPCObjInterface *r = IPCObjectGetInterface();
	if(r && r->XSetCallback) {
		if(r->SetCallback(api)) {
			handle._api = api;
			api = 0;
		}
	}
	if(api) {
//		DestroyIWsiConsumerBase(api);
		delete api;
	}

	return handle;
}

int EXP _QueryObject(void *query, const std::string& wsi_id, int subscr_index, void *rval, IWsiSerializerBase *pserializer, void *rval2, IWsiSerializerBase *pserializer2, IWSIMarshaller **ppmarshaller) {
	int rc = 0;
	IPCObjInterface *r = IPCObjectGetInterface();
	if(r && pserializer) {
		pserializer->lock();

		if(!(*ppmarshaller)) {
			(*ppmarshaller) = r->CreateIWSIMarshaller();
			if((*ppmarshaller)) {
				pserializer->register_with(*(*ppmarshaller));
				if(pserializer2) { 
					pserializer2->register_with(*(*ppmarshaller));
				}
			}
		}
		if(r->XSendObject) {
			pserializer->set_outer_ptr(rval);
			if(pserializer2) { 
				pserializer2->set_outer_ptr(rval2);
			}
			rc = r->SendObject(wsi_id.c_str(), query, subscr_index, true/*i.e. synchronous*/, *ppmarshaller);
		}

		pserializer->unlock();
	}

	return rc;
}


