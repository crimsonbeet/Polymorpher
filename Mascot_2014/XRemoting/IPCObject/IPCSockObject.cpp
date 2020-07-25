// IPCSockObject.cpp : Defines the main functionality.
//

// Created by Alex Nikol Bourakov (ANB) around April/07

// 10/28/2008 - Notes on unfinished important additions.
// 1.
// The connect routine should be extended with a protection against malicious program.
// The simple connect becomes then the connect verification routine.
// It can be done as follows:
// When accept returns a client socket, the next step is to read a connect package from client.
// The read should be done as usual in blocking mode, but this time with a time out.
// If read does not complete, let's say in one second, then the client socket must be closed and the client's ip address
// should be remembered so as not to be accepted next time.
// The connect package should be used for authentication purposes.
// The client should send the current UTC date time +- 15 seconds, as well as its encripted version.
// The server decripts the encripted value and compares with the original value. They should match, otherwise
// the clients socket must be closed and remembered.
// The connect verification routine should be done in a separate thread.
// 2.
// All reads that are marked in the code with TIME-OUT-NEEDED comment, should not block forever but only for a specific time.

// 12/15/2008 - Additional notes on unfinished business.
// 1. Application's error handlers based on delegates should be added.
// Every error will be sent to an application through a handler.
// 2. A successful conection to a socket will also be messaged to an application through a handler.


#include "stdafx_ipcobject.h"
#include ".\..\Include\WSIParser.h"
#include "IPCObject.h"

#include "XRemoting.h"
#include "PollClient.h"

#if defined(_WIN32_WINDOWS)
#include <Mstcpip.h>
#endif


/*
linger opt;
opt.l_onoff = 1;
opt.l_linger = 1;
if(setsockopt(target, SOL_SOCKET, SO_LINGER, (PCHAR)&opt, sizeof(opt))) {
	LOGFailure("IPCSendMessage::setsockopt", "SO_LINGER");
}
*/

inline
u_short inet_port_fromstring(const std::string& addr/*999.999.999.999:99999*/) {
	int x;
	int port = 0;
	if(addr.size()) {
		sscanf(addr.c_str(), "%d.%d.%d.%d:%d", &x, &x, &x, &x, &port);
	}
	return htons(port);
}

//template<typename T>
in_addr EXPORTIPCINTERFACE inet_addr_fromstring(const std::string& addr/*999.999.999.999:99999*/, u_short *pport) {
	int x = 0;
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	sscanf(addr.c_str(), "%d.%d.%d.%d:%d", &a, &b, &c, &d, &x);
	in_addr_helper res;
	res.S_un.S_un_b.s_b1 = a;
	res.S_un.S_un_b.s_b2 = b;
	res.S_un.S_un_b.s_b3 = c;
	res.S_un.S_un_b.s_b4 = d;
	if(pport) {
		*pport = (u_short)htons(x);
	}
	return res.S_un.S_in_addr;
}

sockaddr_in EXPORTIPCINTERFACE sockaddr_in_fromstring(const std::string& addr/*999.999.999.999:99999*/) {
	sockaddr_in servaddr;
	memset(servaddr.sin_zero, 0, sizeof(servaddr.sin_zero));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = inet_addr_fromstring(addr, &servaddr.sin_port);

	return servaddr;
}

std::string/*999.999.999.999:99999*/ EXPORTIPCINTERFACE sockaddr_in_tostring(const sockaddr_in& addr) {
	int x = ntohs(addr.sin_port);
	in_addr_helper tmp(addr.sin_addr);
	int a = tmp.S_un.S_un_b.s_b1;
	int b = tmp.S_un.S_un_b.s_b2;
	int c = tmp.S_un.S_un_b.s_b3;
	int d = tmp.S_un.S_un_b.s_b4;

	std::ostringstream ostr;
	ostr << a << '.' << b << '.' << c << '.' << d;
	if(x) {
		ostr << ':' << x;
	}

	return ostr.str();
}

bool EXPORTIPCINTERFACE check_loopaddress(const sockaddr_in& addr) {
	in_addr_helper tmp(addr.sin_addr);
	return tmp.is_aloopback();
}

bool EXPORTIPCINTERFACE is_loopaddress(const std::string& addr/*999.999.999.999:99999*/) {
	return check_loopaddress(sockaddr_in_fromstring(addr));
}

// it returns a string which is a combination of input address and name
// format is 999.999.999.999:NAME or 999.999.999.999:PORT:NAME or 999.999.999.999:PORT if valid ip address.
std::string make_uniquename(const std::string& name, sockaddr_in saddr, int index) {
//	saddr.sin_port = 0;
	std::ostringstream ostr;
	ostr << index << ':' << sockaddr_in_tostring(saddr);
	if(name.size()) {
		ostr << ":" << name;
	}

	return ostr.str();
}

std::string XConnector::UniqueName(const std::string& name, const std::string& addr, int index) {
	return make_uniquename(name, sockaddr_in_fromstring(addr), index);
}

VOID IPCSetSockOptions(const std::string& logstrrefix, volatile SOCKET& target, DWORD in_buffer_size, DWORD out_buffer_size, DWORD nonblocking, BOOL nodelay) { 
	if(target != INVALID_SOCKET) { 
		if(in_buffer_size != 0) { 
			if(SOCKET_ERROR == setsockopt(target, SOL_SOCKET, SO_RCVBUF, (char*)&in_buffer_size, sizeof(in_buffer_size))) {
				if(_g_trace) {
					LOGFailure(logstrrefix + std::string("::setsockopt(SO_RCVBUF)"), errno);
				}
			}
		} 
		if(out_buffer_size != 0) { 
			if(SOCKET_ERROR == setsockopt(target, SOL_SOCKET, SO_SNDBUF, (char*)&out_buffer_size, sizeof(out_buffer_size))) {
				if(_g_trace) {
					LOGFailure(logstrrefix + std::string("::setsockopt(SO_SNDBUF)"), errno);
				}
			}
		} 
		if(SOCKET_ERROR == ioctlsocket(target, FIONBIO, caddr_t(&nonblocking))) {
			if(_g_trace) {
				LOGFailure(logstrrefix + std::string("::ioctlsocket"), errno);
			}
		}
		if(SOCKET_ERROR == setsockopt(target, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay))) {
			if(_g_trace) {
				LOGFailure(logstrrefix + std::string("::setsockopt(TCP_NODELAY)"), errno);
			}
		}
	} 
} 

// It creates socket and connects to addr if socket is not created yet.
// Sends.
// Receives a confirmation ticket.
// It does not close socket on exit.
BOOL IPCSendMessage(const sockaddr_in *paddr, volatile SOCKET& target, IPCMsgheader& copydata, IWSIMarshaller *marsh = 0, XConnector *conn = 0) {
	BOOL rc = TRUE; 
	BOOL notconnected = FALSE; 
	int err = 0; 
	if(target == INVALID_SOCKET && paddr) { 
		target = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
		if(target != INVALID_SOCKET) { 
			DWORD in_buffer_size = copydata._in_buffer_size; 
			DWORD out_buffer_size = copydata._out_buffer_size; 
			DWORD nonblocking = 0;
			BOOL nodelay = 1; 

			IPCSetSockOptions("IPCSendMessage", target, in_buffer_size, out_buffer_size, nonblocking, nodelay); 

			if((copydata._tag & 0x100) == 0) { // No no-keepalive is set, so set keep alive feature. 
				#if defined(_WIN32_WINDOWS)
				tcp_keepalive keepalive;
				keepalive.onoff = 1;
				keepalive.keepalivetime = 7000; // timeout, in milliseconds, with no activity until the first keep-alive packet is sent.
				keepalive.keepaliveinterval = 1000; // interval, in milliseconds, between when successive keep-alive packets are sent if no acknowledgement is received.
				DWORD dwReturned;
				if(SOCKET_ERROR == WSAIoctl(target, SIO_KEEPALIVE_VALS, &keepalive, sizeof(keepalive), &dwReturned, 4, &dwReturned, 0, 0)) {
					if(_g_trace) {
						LOGFailure(std::string("IPCSendMessage::SIO_KEEPALIVE_VALS"), WSAGetLastError());
					}
				}
				#else
				long keepaliveconnection = -1;
				if(SOCKET_ERROR == setsockopt(target, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepaliveconnection, sizeof(keepaliveconnection))) {
					if(_g_trace) {
						LOGFailure(std::string("IPCSendMessage::setsockopt(SO_KEEPALIVE)"), errno);
					}
				}
				#endif
			} 
		}
		notconnected = TRUE;
	}
	if(target == INVALID_SOCKET) {
		err = errno;
		if(_g_trace) {
			LOGFailure(std::string("IPCSendMessage::socket"), err);
		}
		rc = FALSE;
	}
	else {
		//SOCKET sock = target;
		if(notconnected && paddr == 0) {
			rc = FALSE;
		}
		else
		if(notconnected && connect(target, (sockaddr*)paddr, sizeof(sockaddr_in)) < 0) {
			err = errno;
			if(_g_trace & 4) {
				LOGFailure(std::string("IPCSendMessage::connect") + ' ' + sockaddr_in_tostring(*paddr), err);
			}
			rc = FALSE;
		}
		else
		if(copydata._aux == IPCObj_Response || copydata._aux == IPCObj_InvalidHeader) {
		}
		else
		if(!copydata.ToHeader()) {
			if(_g_trace) {
				LOGFailure(std::string("IPCSendMessage::ToHeader"), std::string((char*)&copydata, sizeof(WSIHeader)));
			}
			rc = FALSE;
		}
		else {
			int headerlen = 0;
			int datalen = copydata._len;

			switch(copydata._aux) {
			case IPCObj_FepMessage:
			case IPCObj_DartMessage:
			case IPCObj_HttpMessage:
			case IPCObj_RawData:
				break;
			default: headerlen = sizeof(WSIHeader);
			}

			int buf2send_cnt = 0;

			WSABUF pack[2];
			if(headerlen) {
				pack[0].buf = (PCHAR)&copydata;
				pack[0].len = headerlen;
				pack[1].buf = (PCHAR)copydata._data;
				pack[1].len = datalen;

				buf2send_cnt = datalen? 2: 1;
			}
			else {
				pack[0].buf = (PCHAR)copydata._data;
				pack[0].len = datalen;

				buf2send_cnt = 1;
			}

			errno = 0;

			IPCReadTransaction *ptransaction = 0;

			if(copydata._ticketexpected == '1') {
				int suggested_trn = _g_ipctransactions.GetSuggestedTransactionNumber(copydata);
				int timeoutsecs = -1;
				if(conn && conn->_responsetimeoutsecs > 0) {
					timeoutsecs = conn->_responsetimeoutsecs;
				}
				ptransaction = _g_ipctransactions.AllocateTransaction(suggested_trn, target, timeoutsecs);
				if(ptransaction) {
					if(suggested_trn < 0 && !headerlen) { /*not Fep and no header, so transaction number is unknown on receive*/
						ptransaction->_transaction = suggested_trn;
					}
					else {
						copydata._trn = ptransaction->_transaction;
						copydata.TransactionToHeader();
					}
				}
			}

			DWORD bytes2send = headerlen + datalen;
			DWORD bytes_sent = 0;
			if((_g_trace & 2) && bytes2send > 0) {
				std::ostringstream ostr;
				ostr << "sending:" << std::string((char*)pack[0].buf, pack[0].len) << (buf2send_cnt > 1? std::string((char*)pack[1].buf, pack[1].len): std::string()) << std::endl;
				IPCLog(ostr.str());
			}
			if(bytes2send > 0) {
				XSendv(target, pack, buf2send_cnt, &bytes_sent, bytes2send);
			}
			if(bytes_sent != bytes2send) {
				err = errno;
				if(_g_trace) {
					std::ostringstream ostr;
					ostr << '(' << bytes_sent << '!' << '=' << bytes2send << ')';
					LOGFailure(std::string("IPCSendMessage::send"), ostr.str()/*(paddr? sockaddr_in_tostring(*paddr): std::string())*/);
				}
				rc = FALSE;
			}

			while(rc == TRUE && ptransaction) {
				std::string obj;
				IPCMsgheader rettkt;
				if(_g_trace & 2) {
					std::ostringstream ostr;
					ostr << "receiving...transaction" << ' ' << ptransaction->_transaction;
					IPCLog(ostr.str());
				}
				rc = ptransaction->Receive(rettkt, obj);
				if(_g_trace & 2) {
					std::ostringstream ostr;
					ostr << "received" << '(' << (rc? "Ok": "Nok") << ')' << std::string((char*)&rettkt, sizeof(WSIHeader)) << ' ' << obj;
					IPCLog(ostr.str());
				}

				if(rc && IPCMsgheader::IsIPCResponse(rettkt)) {

					_g_ipctransactions.FreeTransaction(*ptransaction);

					ptransaction = 0;

					if(rc && obj.size() && marsh) {
						switch(rettkt._format) { 
							case (int)'X': 
								(*marsh)(CXMLParser(), obj);
							break; 
							case (int)'J': 
								(*marsh)(CJSONParser(), obj);
							break; 
							case (int)'F': 
								(*marsh)(CFepParser(), obj);
							break;
							default: 
								(*marsh)(CWSIParser(), obj);
							break; 
						} 
					}
				}
				else
				if(rc && IPCMsgheader::IsIPCRequest(rettkt)) {
					XFunctionType<int, XMarshRequest*> delegGetRequest(XMarshRequestEx::GetRequest);
					if(conn && obj.size()) {
						rc = poll_client::marsh_request(target, 0, rettkt, obj, conn, delegGetRequest);
					}
					else
					if(obj.size()) {
						rc = FALSE;
					}
				}
				else {
					rc = FALSE;
				}
			}
			if(ptransaction) {
				_g_ipctransactions.FreeTransaction(*ptransaction);
			}
			ptransaction = 0;
		}
		if(rc == FALSE && target != INVALID_SOCKET) {
			if(conn) {
				if(_g_trace & 4) {
					LOGMessage(std::string("IPCSendMessage()"), (int)target, (int)SOCKET(target), std::string("shutting down subscriber by socket"));
				}
				if(conn->ShutdownSubscriber(SOCKET(target))) {
					target = INVALID_SOCKET;
				}
			}
			if(target != INVALID_SOCKET) {
				closesocket(target);
			}
			target = INVALID_SOCKET;
		}
	}

	copydata._tag = 0;
	copydata._tag |= err;
	return rc;
}




// XRemoting socket functionality begins here.


void erase_clientsmap(sockclients_map& _map) {
	sockclients_map::iterator iter = _map.begin();
	while(iter != _map.end()) {
		if((*iter).second._sock != INVALID_SOCKET) {
			closesocket((*iter).second._sock);
		}
		if((*iter).second._handler) {
			(*iter).second._handler->OnStatusChange(IPCReq_NOTPROCESSED);
		}
		_map.erase(iter++);
	}
}



void add_client::operator() (const XSocket2Handler<XProducer>& handler) {
	SOCKET client_sock = handler._sock;
	FD_SET(client_sock, _pAllset);
	if((int)client_sock > _maxsock) {
		_maxsock = client_sock;
	}

	_clmap.insert(sockclients_map::value_type(client_sock, handler));
}

void add_client::operator() (PSocket2XProducer& pHandler) {
	(*this)(*pHandler);

	delete pHandler;
	pHandler = 0;
}


void poll_client::operator() (sockclients_map::value_type& client) {
	SOCKET& sock = client.second._sock;
	BOOL ok = TRUE;
	if(sock != INVALID_SOCKET && FD_ISSET(sock, _pRset)) { 
		XWorkItem *pItem = new XWorkItem(); 

		std::string& msg = *pItem->_pitem;
		IPCMsgheader header;

		BOOL isipcrequest = TRUE;

		if(client.second._raw_data) {
			ok = read_rawdata(sock, header, msg);
		}
		else {
			ok = read_ipcrequest(sock, header, msg, 0/*time-out secs*/);
		}

		if(ok && !header.IsIPCRequest(header)) {
			if(client.second._index > 0) {
				header._trn = client.second._index - 1;
			}
			if(!_g_ipctransactions.PostResponse(header._trn, sock, header, msg)) {
				if(_g_trace) {
					std::ostringstream ostr;
					ostr << "argument is not a request" << ':' << std::string((char*)&header, sizeof(WSIHeader)) << ' ' << msg;
					LOGFailure(std::string("poll_client::operator()"), ostr.str());
				}
				ok = FALSE;
			}
			isipcrequest = FALSE;
		}

		if(ok && isipcrequest) {
			if(header._aux == IPCObj_HttpMessage) { 
				client.second._timeout_threshold = OSDayTimeInMilliseconds() + 3000; // IE8 preserves old connections and creates new ones when "Refresh" is hit. 
			} 
			ok = marsh_request(sock, client.second._index, header, msg, _ctl._pConn, _ctl._delegGetRequest);
		}

		pItem->Release(); 
	}
	else 
	if(sock != INVALID_SOCKET) { 
		if(client.second._timeout_threshold > 0) { 
			if(client.second._timeout_threshold < OSDayTimeInMilliseconds()) { 
				ok = FALSE; 
			} 
		} 
	} 

	if(sock != INVALID_SOCKET && !ok) { 
		SOCKET sock_value = sock;
		sock = INVALID_SOCKET; // sock is a reference; thus the value in the sockclients_map is being reset.

		FD_CLR(sock_value, _pAllset);
//		10/20/2008 - Dynamic subscriber reuses existing connection. Try to delete it.
		if(!_ctl._pConn->DeleteSubscriber(sock_value, true)) {
			if(_g_trace & 4) {
				LOGMessage(std::string("poll_client::operator()"), (int)0, (int)sock_value, std::string("shutting down subscriber by socket"));
			}
			if(!_ctl._pConn->ShutdownSubscriber(sock_value)) { // ShutdownSubscriber calls RemoveSocket() that will shutdown gracefully.
				closesocket(sock_value);
			}
		}
	} 
}

// this works because the postfix++ operator modifies the iter variable and
// returns its previous value prior to the call of the erase function.
poll_client::~poll_client() {
	_maxsock = -1;
	sockclients_map::iterator iter = _clmap.begin();
	while(iter != _clmap.end()) {
		if((*iter).second._sock == INVALID_SOCKET) {
			if((*iter).second._handler) {
				(*iter).second._handler->OnStatusChange(IPCReq_CLOSED/*IPCReq_NOTPROCESSED*/);
			}
			_clmap.erase(iter++);
		}
		else {
			if(_maxsock < (int)(*iter).second._sock) {
				_maxsock = (*iter).second._sock;
			}
			++iter;
		}
	}
}



void XReadingHead::AddSocket(SSocketCtl& sock, int index) {
	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	_allsocketsgate.lock();
	//sock.LOCKER.lock(); // causes deadlock
	if(sock._sock != INVALID_SOCKET) {
		if(getsockname(sock._sock, (sockaddr*)&addr, &addrlen) == SOCKET_ERROR) {
			memset(&addr, 0, sizeof(addr));
		}
		add_client(_clientsmap, &_allsockets, _maxsock)(XSocket2Handler<XProducer>(sock._sock, addr, index, sock._raw_data));
	}
	//sock.LOCKER.unlock();
	_allsocketsgate.unlock();
}

bool XReadingHead::RemoveSocket(SSocketCtl& sock) {
	bool rc = false; 
	_allsocketsgate.lock();
	//sock.LOCKER.lock(); // causes deadlock
	if(sock._sock != INVALID_SOCKET) {
		sockclients_map::iterator iter = _clientsmap.find((SOCKET)sock._sock);
		if(iter != _clientsmap.end()) {
			(*iter).second._sock = INVALID_SOCKET; 

			shutdown((SOCKET)sock._sock, SD_BOTH); // XReadingHead::Monitor() handles graceful shutdown through poll_client.
			sock.Reset(); // no closesocket() will be done by XWritingHead since sock is being set to INVALID_SOCKET.
			rc = true; 
		}
	}
	//sock.LOCKER.unlock();
	_allsocketsgate.unlock();
	return rc; 
}




SOCKET XReadingHead::Listen() {
	XListener *Listener = (XListener*)_host_ptr;

	volatile SOCKET& handler = _handler._sock;
	sockaddr_in& addr = _handler._sockaddr;

	handler = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(handler == INVALID_SOCKET) {
		if(_g_trace) {
			LOGFailure(std::string("Listen()::socket"), errno);
		}
		return INVALID_SOCKET;
	}

	BOOL reuseaddr = TRUE;
	if(setsockopt(handler, SOL_SOCKET, SO_REUSEADDR, (PCHAR)&reuseaddr, sizeof(reuseaddr))) {
		if(_g_trace) {
			LOGFailure(std::string("Listen()::setsockopt"), std::string("SO_REUSEADDR"));
		}
	}
#if !defined(_MSC_VER) && !defined(__BORLANDC__)
	if(setsockopt(handler, SOL_SOCKET, SO_REUSEPORT, (PCHAR)&reuseaddr, sizeof(reuseaddr))) {
		if(_g_trace) {
			LOGFailure(std::string("Listen()::setsockopt"), std::string("SO_REUSEPORT"));
		}
	}
#endif

	addr = sockaddr_in_fromstring(Listener->_addr);

	if(bind(handler, (sockaddr*)&addr, sizeof(addr)) < 0) {
		if(_g_trace) {
			LOGFailure(std::string("Listen()::bind"), Listener->_addr);
		}
		closesocket(handler);
		handler = INVALID_SOCKET;
	}
	else
	if(listen(handler, 10) < 0) {
		closesocket(handler);
		handler = INVALID_SOCKET;
	}

	return handler;
}






return_t __stdcall POLLProducers(PVOID pv) {
	SPollProducersCtl *ctl = (SPollProducersCtl*)pv;
	ctl->_connector->PollProducers(ctl->_new_clients);
	ctl->_busy = FALSE;

	return 0;
}




return_t CALLBACK XReadingHead::Monitor(PVOID pv) {
	XReadingHead *This =
		(XReadingHead*)pv;
	XListener *Listener =
		(XListener*)This->_host_ptr;
	XConnector *Connector =
		(XConnector*)Listener->_host_ptr;

	if(Listener->_addr.size() && This->Listen() == INVALID_SOCKET) {
		This->_threadshutdown = TRUE;
		return 0;
	}

	DWORD my_threadid = This->_thread = OSThreadCurrent();

	TLSObject tls_obj; 
	tls_obj.Init(); 
	tls_obj._hthread = (pthread_t)my_threadid; 
	SetTLSObject2(&tls_obj); 


	if(_g_trace) {
		LOGMessage(std::string("XReadingHead::Monitor"), Connector->_name, This->_thread, std::string("thread id"));
	}

	XMarshRequestsCtl reqs_ctl(Connector, Listener->_max_parallel_requests);
	SPollProducersCtl prod_ctl(Connector);

	volatile SOCKET& handler = This->_handler._sock;

	fd_set& allset = This->_allsockets;
	int& maxsock = This->_maxsock;
	sockclients_map& clmap = This->_clientsmap;

	if(This->_handler._sock != INVALID_SOCKET) {
		FD_SET(This->_handler._sock, &allset);
	}

	This->_isrunning = TRUE;

	bool ok = true;

	while(!This->_threadshutdown && !Connector->_started) {
		OSSleep(50);
	}

	if(This->_threadshutdown) {
		ok = false;
	}

	int nsteps = 0;

	int seconds2wait = 0;

	const int const_microseconds2wait_upper = 200000; 
	const int const_microseconds2wait_lower = 30000; 

	int microseconds2wait_upper = Listener->_is_time_critical? const_microseconds2wait_lower: const_microseconds2wait_upper; 
	int microseconds2wait = microseconds2wait_upper;
	int nerrors = 0;
	while(ok) {
		This->_allsocketsgate.lock(my_threadid);
		if(maxsock < (int)handler) {
			maxsock = handler;
		}
		fd_set rset = allset; // rset gets modified each time around.
		This->_allsocketsgate.unlock();

		timeval tv;
		tv.tv_sec = seconds2wait;
		tv.tv_usec = microseconds2wait;

		if((microseconds2wait >>= 1) < const_microseconds2wait_lower) {
			microseconds2wait = microseconds2wait_upper;
		}

		int n = 0;
		if(maxsock >= 0) {
			n = select(maxsock + 1, &rset, NULL, NULL, &tv);
		}
		else {
			OSSleep(200);
		}
		if(n >= 0) {
			nerrors = 0;
		}

		This->_allsocketsgate.lock(my_threadid);
		if(n < 0) {
			if(_g_trace & 8) {
				LOGFailure(std::string("XReadingHead::select"), errno);
			}
			if(++nerrors > 10) {
				ok = false;
			}
			else {
				std::for_each(clmap.begin(), clmap.end(), poll_client(reqs_ctl, clmap, &allset, maxsock, &rset));
			}
		}
		else
		if(n == 0) {
			if(!prod_ctl._busy) {
				_g_ipctransactions.CheckExpiredTransactions();

				if(prod_ctl._new_clients.size()) {
					std::for_each(prod_ctl._new_clients.begin(), prod_ctl._new_clients.end(), add_client(clmap, &allset, maxsock));
					prod_ctl._new_clients.resize(0);
				}
				if(Connector->_producers.size() && Connector->CheckProducers()) {
					prod_ctl._busy = TRUE;
					QueueWorkItem(POLLProducers, (PVOID)&prod_ctl);
				}
			}
		}
		else
		if(FD_ISSET(handler, &rset)) {
//			accept new client request.
			sockaddr_in addr;
			socklen_t addrlen = sizeof(addr);
			SOCKET client_sock = accept(handler, (sockaddr*)&addr, &addrlen);
			if(client_sock == INVALID_SOCKET) {
				LOGFailure(std::string("XReadingHead::accept"), errno);
			}
			else {
				DWORD nonblocking = 0;
				BOOL nodelay = 1;
				int index = 0;
				int raw_data = 0;
				int in_buffer_size = 0; 
				int out_buffer_size = 0; 

				XSubscriber *subscr = Connector->NewClient(sockaddr_in_tostring(addr));
				if(subscr) {
					subscr->_writer._target._sock = client_sock;
					subscr->_writer._target._sockaddr = addr;

					index = subscr->_index;
					raw_data = subscr->_writer._target._raw_data;

					if(subscr->_is_nonblocking) { 
						nonblocking = 1; 
					} 
					#if defined(_UCC) 
					if(check_loopaddress(addr)) { // in order to avoid numerous disconnects when subscriber receives very little cpu-time. 
						nonblocking = 0; 
					} 
					#endif
					if(subscr->_in_buffer_size_kbytes > 0) { 
						in_buffer_size = subscr->_in_buffer_size_kbytes * 1024; 
					} 
					if(subscr->_out_buffer_size_kbytes > 0) { 
						out_buffer_size = subscr->_out_buffer_size_kbytes * 1024; 
					} 
				}

				IPCSetSockOptions("XReadingHead", client_sock, in_buffer_size, out_buffer_size, nonblocking, nodelay); 

				add_client(clmap, &allset, maxsock)(XSocket2Handler<XProducer>(client_sock, addr, index, raw_data));
				if(subscr) {
					subscr->_is_suspended = 0;
					subscr->FireException(1, index, TRUE);
				}
			}
		}
		else {
			if(++nsteps & 1) {
				std::for_each(clmap.begin(), clmap.end(), poll_client(reqs_ctl, clmap, &allset, maxsock, &rset));
			}
			else {
				poll_client func(reqs_ctl, clmap, &allset, maxsock, &rset); 
				sockclients_map::iterator it = clmap.end(); 
				for(size_t x = clmap.size(); x > 0; --x) { 
					--it; 
					func(*it); 
				} 
				//std::for_each(clmap.rbegin(), clmap.rend(), poll_client(reqs_ctl, clmap, &allset, maxsock, &rset));
			}
		}
		This->_allsocketsgate.unlock();

		if(Listener->_max_parallel_requests == 1) { 
			if(Listener->_apc_queue_depth > (int)(clmap.size() * 2)) { 
				Listener->_apc_queue_depth = 0; 
				OSSleep(20); 
			} 
		} 

		if(This->_threadshutdown) {
			break;
		}
	}

	This->_isrunning = FALSE;

	erase_clientsmap(clmap);
	reqs_ctl.WaitFor();
	prod_ctl.WaitFor();

	SetTLSObject2(NULL); 

	if(_g_trace) {
		LOGMessage(std::string("XReadingHead::Monitor"), Connector->_name, This->_thread, std::string("stopped"));
	}
	return 0;
}




// Main Interface functions begin here.

bool XWritingHead::IsValid() {
	return _target._sock != INVALID_SOCKET;
}

int IPCRequestType(XWritingHead *writing_head) {
	int rt = IPCObj_ReceiveObj;
	switch(writing_head->_host_ptr->_writer_type) {
	case (int)'F': rt = IPCObj_FepMessage; break;
	case (int)'D': rt = IPCObj_DartMessage; break;
	case (int)'H': rt = IPCObj_HttpMessage; break;
	case (int)'R': rt = IPCObj_RawData; break;
	}
	return rt;
}

inline bool IPCRequestIsTransactional(XWritingHead *writing_head) {
	switch(writing_head->_host_ptr->_writer_type) {
	case (int)'D':
	case (int)'R':
		return false;
	}
	return true;
}

bool XWritingHead::Connect(std::string* my_addr_out) {
	bool rc = false;

	_target.LOCKER.lock();

	if(!IsValid() && _host_ptr->_addr.size()) {
		if(_target._sockaddr.sin_family != AF_INET) {
			_target._sockaddr = sockaddr_in_fromstring(_host_ptr->_addr);
		}

		IPCMsgheader copydata(Connector()->_name, IsDefaultWriter()? IPCObj_Connect: IPCObj_InvalidHeader, (char)_host_ptr->_writer_type);

		if(IsLocalAddress() || _host_ptr->_no_keepalive) { 
			copydata._tag |= 0x100; 
		} 
		if(_host_ptr->_in_buffer_size_kbytes > 0) { 
			copydata._in_buffer_size = _host_ptr->_in_buffer_size_kbytes * 1024; 
		} 
		if(_host_ptr->_out_buffer_size_kbytes > 0) { 
			copydata._out_buffer_size = _host_ptr->_out_buffer_size_kbytes * 1024; 
		} 

		copydata._ticketexpected = '0';

		if(IPCSendMessage(&_target._sockaddr, _target._sock, copydata, NULL, NULL) == TRUE) {
			if(my_addr_out) {
				*my_addr_out = sockaddr_in_tostring(sock_myaddr((SOCKET)_target._sock));
			}

			if(_host_ptr->_static) {
				Connector()->_listener._reader.AddSocket(_target, _host_ptr->_index);
			}
			rc = true;
		}
		if(_host_ptr->_static) {
			_host_ptr->FireException(1, _host_ptr->_index, rc? TRUE: FALSE);
		}
	}

	_target.LOCKER.unlock();

	return rc;
}


struct XEXTWorkItem {
	XWritingHead *_writing_head;

	XWorkItem *_item;

	XEXTWorkItem(XWritingHead *writing_head, XWorkItem *item): _writing_head(writing_head), _item(item) {
		_item->AddRef();
		_writing_head->_target.AddRef();
	}

	~XEXTWorkItem() {
		if(_writing_head) {
			_writing_head->_target.Release();
			_writing_head = 0;
		}
		if(_item) {
			_item->Release();
			_item = 0;
		}
	}
};


return_t __stdcall XWritingHead::PostFunction(LPVOID lpParameter) {
	return_t rc = 0;
	XEXTWorkItem *work_item =
		(XEXTWorkItem*)lpParameter;
	if(work_item) {
		XWritingHead *writing_head = work_item->_writing_head;
		SSocketCtl *ctl = &writing_head->_target;

		if(!writing_head->IsValid() && writing_head->Connector()->_started) {
			writing_head->Connect(NULL);
		}
		if(writing_head->IsValid() && writing_head->Connector()->_started) {
			IPCMsgheader copydata("POST", IPCRequestType(writing_head), (char)writing_head->_host_ptr->_writer_type, *work_item->_item->_pitem);

			if(FALSE == IPCSendMessage(NULL, ctl->_sock, copydata, NULL, writing_head->Connector())) {
				if(_g_trace) {
					LOGFailure(std::string("XWritingHead::PostFunction"), work_item->_writing_head->_host_ptr->_addr/* + ":" + *work_item->_item->_pitem*/);
				}
				work_item->_item->_status = IPCReq_NOTPROCESSED;
				ctl->_error = 0;
				ctl->_error |= copydata._tag;
			}
			else {
				rc = (return_t)1;
				work_item->_item->_status = IPCReq_SENT;
			}
		}

		InterlockedDecrement(&writing_head->Connector()->_activerequests_count);
		delete work_item;
	}

	return rc;
}

bool XWritingHead::IsLocalAddress() {
	bool rc = false;
	if(_host_ptr->_addr.size()) {
		if(_target._sockaddr.sin_family != AF_INET) {
			_target._sockaddr = sockaddr_in_fromstring(_host_ptr->_addr);
		}
		rc = check_loopaddress(_target._sockaddr);
	}
	return rc;
}


bool XWritingHead::PostXWorkItem(XWorkItem *item) {
	bool rc = false;

	item->_status = IPCReq_PROCESSING;

	if(!IsValid() && _host_ptr->_addr.size()) {
		InterlockedIncrement(&Connector()->_activerequests_count);

		XEXTWorkItem *work_item = new XEXTWorkItem(this, item);
		if(!QueueWorkItem(PostFunction, (PVOID)work_item)) {
			if(_g_trace) {
				LOGFailure(std::string("XWritingHead::PostXWorkItem(QueueWorkItem)"), _host_ptr->_addr);
			}
			delete work_item;
			InterlockedDecrement(&Connector()->_activerequests_count);
		}
		else {
			rc = true;
		}
	}
	else
	if(IsValid() && _host_ptr->_addr.size()) {
		IPCMsgheader copydata(Connector()->_name, IPCRequestType(this), (char)_host_ptr->_writer_type, *item->_pitem);

		if(FALSE == IPCSendMessage(NULL, _target._sock, copydata, NULL, Connector())) {
			if(_g_trace) {
				LOGFailure(std::string("XWritingHead::PostXWorkItem"), _host_ptr->_addr);
			}
			item->_status = IPCReq_NOTPROCESSED;
			_target._error = 0;
			_target._error |= copydata._tag;
		}
		else {
			rc = true;
			item->_status = IPCReq_SENT;
		}
	}

	return rc;
}

bool XWritingHead::SendXWorkItem(XWorkItem *item, IWSIMarshaller *marsh) {
	bool rc = false;

	if(IsValid() && _host_ptr->_addr.size()) {
		IPCMsgheader copydata(Connector()->_name, IPCRequestType(this), (char)_host_ptr->_writer_type, *item->_pitem);

		if(_host_ptr->_index > 0) {
			copydata._trn = _host_ptr->_index - 1;
		}

		copydata._ticketexpected = IPCRequestIsTransactional(this)? '1': '0';

		if(FALSE == IPCSendMessage(NULL, _target._sock, copydata, marsh, Connector())) {
			if(_g_trace) {
				LOGMessage(std::string("XWritingHead::SendXWorkItem"), *item->_pitem, _host_ptr->_addr, std::string("failed for"));
			}
			item->_status = IPCReq_NOTPROCESSED;
			_target._error = 0;
			_target._error |= copydata._tag;
		}
		else {
			rc = true;
			item->_status = IPCReq_SENT;
		}

	}

	return rc;
}

std::string XWritingHead::BuildWellFormedAddress(const std::string& addr) {
	sockaddr_in saddr = sockaddr_in_fromstring(addr);
	if(saddr.sin_port == 0) {
		saddr.sin_port = inet_port_fromstring(_host_ptr->_addr);
	}

	return sockaddr_in_tostring(saddr);
}




// Log support begins here.


bool XLogServer::SendLogMessage(const std::string& msg, int origin_port) {
	bool ok = true;
	bool msg_sent = false;
	if(_logger.IsValid() && (origin_port == -1 || origin_port != _my_port)) {
		IPCMsgheader copydata(_name, IPCObj_LogMessage, 'W', msg);
		copydata.ToHeader();

		int len = sizeof(WSIHeader);
		int datalen = copydata._len;
		WSABUF pack[2];
		pack[0].buf = (PCHAR)&copydata;
		pack[0].len = len;
		pack[1].buf = (PCHAR)copydata._data;
		pack[1].len = datalen;

		DWORD bytes2send = len + datalen;
		DWORD bytes_sent = 0;
		XSendv(_logger.Get(), pack, datalen? 2: 1, &bytes_sent, bytes2send);
		if(bytes_sent == bytes2send) {
			msg_sent = true;
		}
		else {
			_logger.Reset();
		}
	}
	if(!msg_sent) {
		ok = OUTPUTLogMessage(msg) == TRUE;
	}

	return ok;
}

void XLogServer::HookSubscriber(XSubscriber& subscriber) {
	SOCKET sock = subscriber._writer._target._sock;
	if(sock != INVALID_SOCKET && sock != NULL) {
		if(!_logger.IsValid()) {
			_name = subscriber._name;
			_logger.Set(sock);
			_my_port = sock_myaddr(_logger.Get()).sin_port; 
		}
	}
}

void XLogServer::Reset() {
	_my_port = 0; 
	_name.resize(0); 
	_logger.Reset();
}

XLogServer::XLogServer(): _my_port(0) {
}

XLogServer::~XLogServer() {
}

