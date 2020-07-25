// stdafx.cpp : source file that includes just the standard includes
// IPCObject.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

// Created by Alex Nikol Bourakov (ANB) around April/07

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "stdafx_ipcobject.h"


#define WSICLASSFACTORY_IMPL


#include "ClassesXParse.h"
#include "ClassesXRemoting.h"



bool g_network_isactive;

int _g_trace = 0; 



class CWSICallbackNewXSubscriber: public IWsiConsumerBase {
	XSubscriber * (XConnector::*Fn)(XSubscriber*);
	XConnector *Bs;

public:
	CWSICallbackNewXSubscriber(XSubscriber* (XConnector::*f)(XSubscriber*), XConnector *base, const std::string& wsi_id):
		Fn(f), Bs(base) {
		_wsi_id = wsi_id;
	}

	void operator()(void* obj, LONG_PTR rpc_handle) {
		XSubscriber *subscr = (Bs->*Fn)((XSubscriber*)obj);
//		10/20/2008 - Server reuses existing connection instead of creating a new connection.
		if(subscr) {
			XRPCHandle *rpc = (XRPCHandle*)rpc_handle;
			if(rpc->_rpc_socket) {
				subscr->_writer._target._sock = *(SOCKET*)rpc->_rpc_socket;
				socklen_t addrlen = sizeof(sockaddr_in);
				getpeername(subscr->_writer._target._sock, (sockaddr*)&subscr->_writer._target._sockaddr, &addrlen);
				subscr->_addr = sockaddr_in_tostring(subscr->_writer._target._sockaddr);
			}
			if(subscr->_log_receiver) {
				if(!subscr->_static) {
					((XRemoting*)subscr->_host_ptr->_host_ptr)->_logserver.HookSubscriber(*subscr);
				}
			}
			subscr->FireException(1, subscr->_index, TRUE);
		}
	}
};

CWSICallbackNewXSubscriber *
CreateCallbackNewXSubscriber(XSubscriber* (XConnector::*f)(XSubscriber*), XConnector *b, IWSIMarshaller *m, IWsiSerializerBase *ser) {
	CWSICallbackNewXSubscriber *res = new CWSICallbackNewXSubscriber(f, b, ser->_wsi_id);
	if(res) {
		ser->register_with(*m);
		res->register_with(*m);
	}

	return res;
}





// RadiSys Ultra C++ requires some source file (normal, top-level, explicitly-compiled)
// that contains both the definition of the template entity
// and types required for the particular instantiation.
// Otherwise Ultra C++ complains about entities that are
// referenced as both explicit specializations and generated instantiations.

// The following functions were added with the purpose to overcome this issue.


IWsiConsumerBase * SetCallbackNewXSubscriber(XConnector *This) {
	IWsiSerializerBase *ws = This->_host_ptr->FindObj(XRemote::__wsiRootXSubscriber._wsi_id);
	return
		CreateCallbackNewXSubscriber(&XConnector::NewSubscriber, This, This->_marshaller, ws);
}

void DeSerializeXRemotingObject(XRemoting *remoobj, const std::string& config, IWsiSerializerBase **extra, const int extra_cnt) {
	IWSIMarshaller *pMarshaller = CreateIWSIMarshaller();
	if(pMarshaller) {
		IWSIMarshaller& marshaller = *pMarshaller;

		CWSIXRemoting sr(remoobj);
		sr.register_with(marshaller);

		XRemotingSettings remset;
		CWSIXRemotingSettings sremset(remset);
		sremset.register_with(marshaller);

		for(int x = 0; x < extra_cnt; ++x) {
			(extra[x])->register_with(marshaller);
		}

		marshaller(CWSIParser(), config);

		remoobj->Apply(remset);
	}
	DestroyIWSIMarshaller(pMarshaller);
}


void SerializeXRemotingObject(std::string& config, XRemoting *remoobj, IWsiSerializerBase **extra, const int extra_cnt) {
	ISTREAMWriter& writer = *CWSIParser().create_writer();
	writer.SetHumanReadableFormat(true);

	XRemotingSettings remset;
	remoobj->GetSettings(remset);

	std::string wsi;

	CWSIXRemoting sr(remoobj);
	sr(writer, config);

	CWSIXRemotingSettings srs(remset);
	srs(writer, wsi);
	config += wsi;

	for(int x = 0; x < extra_cnt; ++x) {
		(*(extra[x]))(writer, (extra[x])->get_inner_ptr(), wsi);
		config += wsi;
	}

	delete &writer;
}


