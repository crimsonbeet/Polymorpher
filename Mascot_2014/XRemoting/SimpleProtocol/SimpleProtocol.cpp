// SimpleProtocol.cpp : Defines the entry point for the console application.
//

#include "stdafx_simpleprotocol.h"

#if defined(_MSC_VER)
#include "winsock2.h"
#include "Ws2tcpip.h"

#define caddr_t
#define MSG_NOSIGNAL 0

#else
#include <sys\types.h>
#include <sys\socket.h>
#include <sys\uio.h>
#include <UNIX\ioctl.h>
#include <sys\ioctl.h>
#include <netinet\in.h>
#include <netinet/tcp.h>
#include <sys\select.h>
#include <sched.h>

#define closesocket close
#define ioctlsocket ioctl
#define SD_SEND 1
#define SD_RECEIVE 0
#define SD_BOTH 2
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define MSG_NOSIGNAL 0
typedef int SOCKET;

#endif

#include ".\..\Include\IPCInterface.h"
#include "SimpleXClasses.h"

typedef XFunctionType<XIPCObjectException, int> XIPCObjectExceptionHandler;



BOOL g_bTerminated = FALSE;

BOOL g_wsiremoting_success = FALSE;

std::vector<api_handle> g_api_handles;

int Handle_XIPCObjectException(XIPCObjectException& excpt);
XIPCObjectExceptionHandler g_xipcexception_handler(Handle_XIPCObjectException);



SimpleProcotolConfiguration g_myconfiguration;




#if defined(_MSC_VER)

HANDLE g_hThread[4] = {0, 0, 0, 0};

VOID CALLBACK ApcWakeUp(ULONG_PTR dwParam) {
}

BOOL WINAPI HandleConsoleEvent(DWORD dwCtrlType) {
	BOOL rc = FALSE;
	switch (dwCtrlType) {
		case CTRL_CLOSE_EVENT:
			g_bTerminated = TRUE;
			for(int x = 0; x < ARRAY_NUM_ELEMENTS(g_hThread); ++x) {
				if(g_hThread[x] != 0) {
					QueueUserAPC(ApcWakeUp, g_hThread[x], (ULONG_PTR)0);
				}
			}
			rc = TRUE;
			break;
	}
	return rc;
}

#endif


int Handle_XIPCObjectException(XIPCObjectException& excpt) {
	switch(excpt._type) {
		case 1: // subscriber.
			std::cout << excpt._name << ':' << excpt._addr << ' ' << "has been" << ' ' << (excpt._is_connected? std::string("connected"): std::string("disconnected")) << std::endl;
		break;
		case 2: // producer.
		break;
	}
	return 0;
}




BOOL XStart() {
#if defined(_MSC_VER)
	std::string config = ReadTextFile("SimpleProtocolSecondary.txt");
#else
	std::string config = ReadTextFile("/h0/MPRO/SimpleProtocol.txt");
#endif
	if(config.size()) {
		PIPCObjInterface wsiremoting = IPCObjectGetInterface();
		if(wsiremoting) {
			ADD_WSINAMESPACE(XSimpleProtocol)
			g_wsiremoting_success = wsiremoting->Initialize(config.c_str());
		}
	}

	if(g_wsiremoting_success) {
		PIPCObjInterface wsiremoting = IPCObjectGetInterface();
		if(wsiremoting) {
			wsiremoting->AddExceptionDelegate(&g_xipcexception_handler);
			wsiremoting->JoinNetwork();
		}
	}

	return g_wsiremoting_success;
}

VOID XStop() {
	if(g_wsiremoting_success) {
		g_wsiremoting_success = 0;

		PIPCObjInterface wsiremoting = IPCObjectGetInterface();
		if(wsiremoting) {
			wsiremoting->LeaveNetwork();
			wsiremoting->RemoveExceptionDelegate(&g_xipcexception_handler);
		}
	}
}




void Process_SIMessage(SIMessage *obj);
void Process_SPMessage(SPMessage *obj);
SIMessage Return_SIMessage(SPMessage *obj);

RPCResponse FEPSend_StartCommunication(RPCRequest_StartCommunication *obj);
RPCResponse FEPSend_EnableDisable(RPCRequest_EnableDisableTool *obj);

return_t __stdcall SimulateEnableDisable(LPVOID);
return_t __stdcall FepEnableDisable(LPVOID); 









FPMessageStartAcknowledge FPStartCommunication(FPMessageStartCommunication *obj) { 
	std::cout << "FPMessageStartCommunication" << std::endl; 
	return FPMessageStartAcknowledge(); 
} 

FPMessageCommandAccepted FPEnableTool(FPMessageEnableTool *obj) { 
	std::cout << "FPMessageEnableTool" << std::endl; 
	return FPMessageCommandAccepted(); 
} 

FPMessageCommandAccepted FPDisableTool(FPMessageDisableTool *obj) { 
	std::cout << "FPMessageDisableTool" << std::endl; 
	return FPMessageCommandAccepted(); 
} 



return_t __stdcall FepEnableDisable(LPVOID) {
	#if defined(_MSC_VER)
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_hThread[2], 0, FALSE, DUPLICATE_SAME_ACCESS);

	int statuses[32]; // 1 - started, 2 - enabled, 3 - disabled
	memset(statuses, 0, sizeof(statuses));

	while(!g_bTerminated) {
		for(int x = 0; x < 10 && !g_bTerminated; ++x) {
			FPMessageStartAcknowledge start_ack; 
			FPMessageCommandAccepted cmd_ack; 

			int nspindle = x + 1;

			if(statuses[x] == 0) {
				if(!QueryObject(FPMessageStartCommunication(nspindle), start_ack)) {
					std::cout << "FPMessageStartCommunication not sent." << std::endl;
				}
				else {
					statuses[x] = 1;
				}
			}
			else
			if(statuses[x] == 3) { 
				if(!QueryObject(FPMessageEnableTool(nspindle), cmd_ack)) {
					std::cout << "FPMessageEnableTool not sent." << std::endl;
				}
				else { 
					statuses[x] = 2; 
				} 
			}
			else
			if(statuses[x] != 3) { 
				if(!QueryObject(FPMessageDisableTool(nspindle), cmd_ack)) {
					std::cout << "FPMessageDisableTool not sent." << std::endl;
				}
				else { 
					statuses[x] = 3; 
				} 
			}

			//OSSleep(100);
		}
		OSSleep(2000);
	}

	CloseHandle(g_hThread[2]);
	#endif
	return 0;
} 











void Process_SDartPacket(SDartPacket *obj) { 
	std::cout << "SDartPacket" << std::endl; 
} 

void Process_SDartData(SDartData *obj) { 
	std::cout << "SDartData" << std::endl; 
} 



return_t __stdcall DartSend(LPVOID) {
	#if defined(_MSC_VER)
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_hThread[3], 0, FALSE, DUPLICATE_SAME_ACCESS);

	while(!g_bTerminated) { 
		if(!PostObject(SDartConnect())) { 
			std::cout << "SDartConnect not sent" << std::endl; 
		} 
		if(!PostObject(SDartPacket())) { 
			std::cout << "SDartPacket not sent" << std::endl; 
		} 
		OSSleep(200);
		if(!PostObject(SDartData())) { 
			std::cout << "SDartData not sent" << std::endl; 
		} 
		OSSleep(2000);
	}

	CloseHandle(g_hThread[3]);
	#endif
	return 0;
} 





in_addr inet_addr_fromstring(const std::string& addr/*999.999.999.999:99999*/, u_short *pport = 0);
BOOL InitializeIPCInterface();





#if defined(_MSC_VER)
int _tmain(int argc, _TCHAR* argv[]) {
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_hThread[0], 0, FALSE, DUPLICATE_SAME_ACCESS);

	SetConsoleCtrlHandler(HandleConsoleEvent, TRUE);

	XConfigure(g_myconfiguration, ReadTextFile("MyConfig.txt"));

#else

int main(int argc, char **argv) {
	InitializeIPCInterface();

	XConfigure(g_myconfiguration, ReadTextFile("/h0/MPRO/MyConfig.txt"));

#endif

	XStart();

	g_api_handles.push_back(SetCallback(Process_SIMessage));
	g_api_handles.push_back(SetCallback(Process_SPMessage));


	g_api_handles.push_back(SetCallback(Process_SDartPacket));
	g_api_handles.push_back(SetCallback(Process_SDartData));


	g_api_handles.push_back(SetRpcCallee(FEPSend_EnableDisable));
	g_api_handles.push_back(SetRpcCallee(FEPSend_StartCommunication));


	g_api_handles.push_back(SetRpcCallee(FPStartCommunication));
	g_api_handles.push_back(SetRpcCallee(FPEnableTool));
	g_api_handles.push_back(SetRpcCallee(FPDisableTool));



#if defined(_MSC_VER)
	//QueueWorkItem(SimulateEnableDisable);
	QueueWorkItem(FepEnableDisable);
	QueueWorkItem(DartSend);
#endif


	std::string logmsg;

	while(!g_bTerminated) {
#if defined(_UCC)
		SPMessage pmessage;
		if(PostObject(pmessage)) {
			logmsg = "p-message sent.";
		}
		else {
			logmsg = "p-message not sent.";
		}
		if(!IPCLog(logmsg)) {
			std::cout << logmsg << std::endl;
		}
#endif
		OSSleep(1000);
	}



	std::for_each(g_api_handles.begin(), g_api_handles.end(), IPCDeleteAPIHandle);

	XStop();

#if defined(_MSC_VER)
	SetConsoleCtrlHandler(HandleConsoleEvent, FALSE);

	CloseHandle(g_hThread[0]);
#endif


	WaitforWorkItems();
	return 0;
}












void Process_SIMessage(SIMessage *obj) {
	TLSObject tlsobj = GetTLSObject();
	//std::cout << "i-message received from " << tlsobj._connectorname << std::endl;
	std::ostringstream ostr;
	ostr << "i-message " << tlsobj._connectorname;
	if(!IPCLog(ostr.str())) {
		std::cout << ostr.str() << std::endl;
	}
}

void Process_SPMessage(SPMessage *obj) {
	if(!PostObject(SIMessage())) {
		std::cout << "i-message has not been sent." << std::endl;
	}
}

SIMessage Return_SIMessage(SPMessage *obj) {
	return SIMessage();
}







BOOL SendReceiveString(SOCKET target, const std::string& message, RPCResponse& response) {
	BOOL ok = TRUE;

	char buf[128];
	if(send(target, (PCHAR)message.c_str(), message.size(), 0) < 0) {
		ok = FALSE;
	}
	else
	if(recv(target, buf, sizeof(buf) - 1, 0) < 0) {
		ok = FALSE;
	}
	else {
		response._response = buf;
		response._ok = 1;
	}

	return ok;
}



RPCResponse FEPSend_StartCommunication(RPCRequest_StartCommunication *obj) {
	RPCResponse response;

	int nspindle = obj->_spindle;

	std::cout << "RPCRequest_StartCommunication for " << g_myconfiguration._ipaddress << ' ' << "spindle=" << nspindle << ' ';

	std::ostringstream ostr;
	ostr << "Hi." << XTrimString(GetTLSObject2()->_connectorname, 18) << '!' << ':';

	if(nspindle > 0 && nspindle <= 32) {
		int nport = g_myconfiguration._ports[nspindle - 1];
		if(g_myconfiguration._ipaddress.size() == 0) {
			ostr << "ip-address not specified";
		}
		else
		if(nport <= 0) {
			ostr << "port not specified for spindle " << nspindle;
		}
		else {
			SOCKET target = g_myconfiguration._sockets[nspindle - 1];
			if(target == INVALID_SOCKET) {
				target = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if(target != INVALID_SOCKET) {
					DWORD nonblocking = 0;
					ioctlsocket(target, FIONBIO, caddr_t(&nonblocking));

					sockaddr_in addr;
					memset(&addr, 0, sizeof(addr));
					addr.sin_port = htons(nport);
					addr.sin_family = AF_INET;

					addr.sin_addr = inet_addr_fromstring(g_myconfiguration._ipaddress);

					if(connect(target, (sockaddr*)&addr, sizeof(addr)) < 0) {
						ostr << "can not connect to " << g_myconfiguration._ipaddress << ':' << nport << ':' << FormatSystemError(GetLastError());
						closesocket(target);
						target = INVALID_SOCKET;
					}
					else {
						ostr << "connected" << ' ';
					}
				}
				else {
					ostr << "can not create socket" << ':' << FormatSystemError(GetLastError());
				}
			}
			if(target != INVALID_SOCKET) {
				if(!SendReceiveString(target, OPMessageStartCommunication(), response)) {
					ostr << "failed to send/receive to " << g_myconfiguration._ipaddress << ':' << nport << ':' << FormatSystemError(GetLastError());
					closesocket(target);
					target = INVALID_SOCKET;
				}
				else {
					ostr << "sent" << ' ';
				}
			}
			g_myconfiguration._sockets[nspindle - 1] = target;
		}
	}
	else {
		ostr << "bad spindle number " << nspindle << std::endl;
	}

	if(!response._ok) {
		response._response = ostr.str();
	}

	std::cout << "done ok=" << response._ok << std::endl;

	response._test_double = 2.7;

	return response;
}


RPCResponse FEPSend_EnableDisable(RPCRequest_EnableDisableTool *obj) {
	RPCResponse response;

	std::cout << "RPCRequest enable=" << obj->_enable << ':' << "spindle=" << obj->_spindle;

	std::ostringstream ostr;
	ostr << "Hi." << XTrimString(GetTLSObject2()->_connectorname, 18) << '!' << ' ';

	int nspindle = obj->_spindle;

	std::string message;
	if(obj->_enable) {
		message = OPMessageEnableTool();
	}
	else {
		message = OPMessageDisableTool();
	}

	if(nspindle > 0 && nspindle <= 32) {
		SOCKET target = g_myconfiguration._sockets[nspindle - 1];
		if(target == INVALID_SOCKET) {
			ostr << "communication not started for spindle " << nspindle;
		}
		else
		if(!SendReceiveString(target, message, response)) {
			ostr << "failed to send/receive for spindle " << nspindle << ':' << FormatSystemError(GetLastError());
			closesocket(target);
			target = INVALID_SOCKET;
		}
		g_myconfiguration._sockets[nspindle - 1] = target;
	}
	else {
		ostr << "bad spindle number " << nspindle << std::endl;
	}

	if(!response._ok) {
		response._response = ostr.str();
	}

	std::cout << "done ok=" << response._ok << std::endl;

	response._test_double = 1.5;

	return response;
}


return_t __stdcall SimulateRPCEnableDisable(LPVOID) {
	#if defined(_MSC_VER)
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_hThread[1], 0, FALSE, DUPLICATE_SAME_ACCESS);

	int statuses[32]; // 1 - started, 2 - enabled, 3 - disabled
	memset(statuses, 0, sizeof(statuses));

	while(!g_bTerminated) {
		for(int x = 0; x < 10 && !g_bTerminated; ++x) {
			RPCResponse response;

			int nspindle = x + 1;

			if(statuses[x] == 0) {
				if(!QueryObject(RPCRequest_StartCommunication(nspindle), response)) {
					std::cout << "RPCRequest_StartCommunication not sent." << std::endl;
				}
				else {
					statuses[x] = 1;
				}
			}
			else
			if(!QueryObject(RPCRequest_EnableDisableTool(nspindle, statuses[x] == 3), response)) {
				std::cout << "RPCRequest_EnableDisableTool not sent." << std::endl;
			}
			else {
				statuses[x] = statuses[x] == 3? 2: 3;
			}

			if(response._response.size()) {
				std::cout << response._response << ' ' << response._test_double << std::endl;
			}
			if(!response._ok) {
				statuses[x] = 0;
			}

			//OSSleep(100);
		}
		OSSleep(2000);
	}

	CloseHandle(g_hThread[1]);
	#endif
	return 0;
}



