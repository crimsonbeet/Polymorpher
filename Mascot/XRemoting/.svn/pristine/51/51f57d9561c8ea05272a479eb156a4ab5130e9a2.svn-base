// Created by Alex Nikol Bourakov (ANB) around April/07


#include "stdafx_ipcobject.h"

#include "IPCObject.h"
#include "XRemoting.h"
#include "PollClient.h"


/*
 * Static declarations for backed up XMarshRequest storage. 
 */
XMarshRequestEx XMarshRequestEx::_storage[512]; 
volatile XMarshRequestEx *XMarshRequestEx::_head = XMarshRequestEx::InitializeStorage(); 
wsi_gate XMarshRequestEx::_gate; 

XThreadCtl *XMarshRequestsCtl::_s_pipe_pctl;



/*
 * Receive "n" bytes from a socket.
 */
size_t recvn(SOCKET sock, char *vptr, size_t nsize, int flags, int timeout_secs, bool one_step) {
	size_t nleft = nsize;
	size_t nread = 0;

	timeval tv;

	tv.tv_sec = timeout_secs;
	tv.tv_usec = 0;

	char *ptr = vptr;

	while(nleft > 0) {
		DWORD navailable = 0;
		if(SOCKET_ERROR == ioctlsocket(sock, FIONREAD, caddr_t(&navailable))) {
			if(_g_trace & 8) {
				LOGFailure(std::string("recvn::ioctlsocket"), errno);
			}
		}

		if(navailable == 0 && timeout_secs >= 0) {
			fd_set ssock;
			FD_ZERO(&ssock);
			FD_SET(sock, &ssock);
			if(timeout_secs == 0 || select((int)(sock + 1), &ssock, NULL, NULL, &tv) <= 0) {
				if(_g_trace & 8) {
					if(timeout_secs != 0) { 
						LOGFailure(std::string("recvn::select"), errno);
					} 
				}
				break; // time-out
			}
			FD_CLR(sock, &ssock);

			if(tv.tv_sec >= 1) {
				if(tv.tv_sec == 1) {
					tv.tv_usec = 500000;
				}
				tv.tv_sec >>= 1;
			}
			else {
				tv.tv_usec >>= 1;
			}
		}

		while((int)(nread = recv(sock, ptr, (int)nleft, flags)) < 0) {
			if(timeout_secs < 0) { 
				if(errno == EWOULDBLOCK || errno == EAGAIN) { 
					OSWait(); 
					continue; 
				} 
			} 
			if(_g_trace) {
				LOGFailure(std::string("recvn"), errno);
			}
			return 0; // error
		}
		if(nread == 0) {
			break; // eof
		}

		nleft -= nread;
		ptr += nread;

		if(one_step) { 
			break; 
		} 
	}

	return (nsize - nleft);
}

sockaddr_in sock_peeraddr(const SOCKET& sock) {
	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	if(getpeername(sock, (sockaddr*)&addr, &addrlen) == SOCKET_ERROR) {
		memset(&addr, 0, sizeof(addr));
	}
	return addr;
}

sockaddr_in sock_myaddr(const SOCKET& sock) {
	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	if(getsockname(sock, (sockaddr*)&addr, &addrlen) == SOCKET_ERROR) {
		memset(&addr, 0, sizeof(addr));
	}
	else {
		if(in_addr_helper(addr.sin_addr).is_inaddr_any()) {
			addr.sin_addr = in_addr_helper::make_loopback();
		}
	}
	return addr;
}



BOOL __stdcall SendReturnObject(SOCKET sock, std::string& ret_obj) {
	BOOL ok = TRUE;

	WSABUF pack;
	pack.buf = (PCHAR)ret_obj.data();
	pack.len = (ULONG)ret_obj.size();

	DWORD bytes2send = pack.len;
	DWORD bytes_sent = 0;
	XSendv(sock, &pack, 1, &bytes_sent, bytes2send);
	if(bytes_sent != bytes2send) {
		std::ostringstream ostr; 
		ostr << "bytes_sent != bytes2send" << ':' << bytes_sent << ' ' << "!=" << ' ' << bytes2send; 
		LOGFailure(std::string("WINAPI Marshaller"), ostr.str());
		ok = FALSE;
	}

	if(_g_trace & 4) { 
		size_t length = 0; 
		for(size_t x = 0; x < ret_obj.size(); ++x) { 
			if(ret_obj[x] < 0x20 && ret_obj[x] != 0xD && ret_obj[x] != 0xA) { 
				break; 
			} 
			++length; 
		} 
		if(length < ret_obj.size()) { 
			IPCLog(ret_obj.substr(0, length)); 
		} 
		else { 
			IPCLog(ret_obj); 
		} 
	}

	return ok;
}


BOOL __stdcall SendReturnTicket(SOCKET sock, char format, std::string& ret_obj, int trn) {
	BOOL ok = TRUE;

	IPCMsgheader rettkt("", 0, format, ret_obj, trn);

	WSABUF pack[2];
	pack[0].buf = (PCHAR)&rettkt;
	pack[0].len = sizeof(WSIHeader);
	pack[1].buf = rettkt._data;
	pack[1].len = rettkt._len;

	DWORD bytes2send = pack[0].len + pack[1].len;
	DWORD bytes_sent = 0;
	XSendv(sock, pack, pack[1].len? 2: 1, &bytes_sent, bytes2send);
	if(bytes_sent != bytes2send) {
		LOGFailure(std::string("WINAPI Marshaller"), std::string("bytes_sent != bytes2send"));
		ok = FALSE;
	}

	return ok;
}


return_t __stdcall MarshRequestHandler(LPVOID lpParameter) {
	XMarshRequest *request = (XMarshRequest *)lpParameter;
	if(!request) {
		return (return_t)TRUE;
	}

	CWSIWriter& _wsiwriter = request->_wsiwriter;
	CXMLWriter& _xmlwriter = request->_xmlwriter;
	CJSONWriter& _jsonwriter = request->_jsonwriter;
	CFepWriter& _fepwriter = request->_fepwriter;
	CHttpWriter& _httpwriter = request->_httpwriter;
	CRawWriter& _rawwriter = request->_rawwriter;

	SOCKET& sock = request->_sock;
	XConnector *conn = request->_pConn;
	IPCMsgheader& header = request->_copydata;
	std::string& msg = request->_msg;

	ISTREAMWriter& writer = header._format == 'F'? (ISTREAMWriter&)_fepwriter: header._format == 'X'? (ISTREAMWriter&)_xmlwriter: header._format == 'J'? (ISTREAMWriter&)_jsonwriter: header._format == 'H'? (ISTREAMWriter&)_httpwriter: header._format == 'R'? (ISTREAMWriter&)_rawwriter: (ISTREAMWriter&)_wsiwriter;

	XRPCHandle rpc(writer);
	rpc._marshaller = (LONG_PTR)conn->_marshaller;
	rpc._remoting_object = (LONG_PTR)conn->_host_ptr;
	rpc._writer.SetVersion(header._tag & 0xff);

	std::string ret_obj;

	if(header._ticketexpected == '1') {
		rpc._rpc_outstring = (LONG_PTR)&ret_obj;
	}

	rpc._rpc_socket = (LONG_PTR)&sock;


	TLSObject *tlsobj = GetTLSObject2();
	tlsobj->SetConnectorName(header._connectorname, sizeof(header._connectorname));
	tlsobj->_index = request->_index;
	tlsobj->_onreturn_shutdownsocket = 0;

	if(conn->_security_is_enabled) {
#if _MSC_VER > 1700
		std::unordered_map<std::string, XSubscriber*>::iterator it = conn->FindMappedSubscriber(sock);
#else
		std::map<std::string, XSubscriber*>::iterator it = conn->FindMappedSubscriber(sock);
#endif
		if(it != conn->_subscribers.end()) {
			tlsobj->_stoken = &(*it).second->_stoken;
		}
		else {
			tlsobj->_stoken = 0;
		}
	}
	else {
		tlsobj->_stoken = 0;
	}

	tlsobj->PushRpcHandle(&rpc); 

	if(msg.size()) {
		if(_g_trace & 8) {
			std::ostringstream ostr; 
			ostr << "MarshRequestHandler" << ':' << msg;
			IPCLog(ostr.str()); 
		}
		ISTREAMReader *parser = 0;
		switch((int)header._format) {
			case (int)'F': parser = &conn->_fepparser; break;
			case (int)'X': parser = &conn->_xmlparser; break;
			case (int)'J': parser = &conn->_jsonparser; break;
			case (int)'D': parser = &conn->_dartparser; break;
			case (int)'H': parser = &conn->_httpparser; break;
			case (int)'R': parser = &conn->_rawparser; break;
			default: parser = &conn->_wsiparser;
			break;
		}
		(*(conn->_marshaller))(*parser, msg, (LONG_PTR)&rpc, conn? conn->_use_clones: FALSE);
	}

	tlsobj->PopRpcHandle(); 

	BOOL ok = TRUE;
	if(header._ticketexpected == '1') {
		switch(header._aux) { 
		case IPCObj_DartMessage: 
		case IPCObj_CRFData:
			break; 
		case IPCObj_FepMessage: 
		case IPCObj_HttpMessage: 
		case IPCObj_RawData: 
			if(ret_obj.size()) {
				ok = SendReturnObject(sock, ret_obj);
			}
			break; 
		default: 
			ok = SendReturnTicket(sock, header._format, ret_obj, header._trn);
			break;
		} 
	}

	if(ok) { 
		if(tlsobj->_onreturn_shutdownsocket) { 
			shutdown(sock, SD_SEND); 
		} 
	} 

	bool apc_done = false; 

	while(!apc_done) { 
		XMarshRequest *apc_request = 0; 

		request->_new_data_gate.lock(); 
		if(request->_new_data.empty()) { 
			request->Release();
			apc_done = true; 
		} 
		else { 
			apc_request = request->_new_data.front(); 
			request->_new_data.pop_front();
		} 
		request->_new_data_gate.unlock(); 

		if(apc_request) { 
			static size_t s_next_threshold = 16; 
			if(_g_trace && request->_new_data.size()) { 
				if(_g_trace && request->_new_data.size() > s_next_threshold) { 
					std::ostringstream ostr;
					ostr << "MarshRequestHandler() apc_request queue length" << ' ' << request->_new_data.size();
					IPCLog(ostr.str()); 
	
					s_next_threshold <<= 1; 
				}
			} 
			else 
			if(s_next_threshold >= 32) { 
				s_next_threshold >>= 1; 
			} 
			MarshRequestHandler(apc_request); 
		} 
	} 


	return (return_t)ok;
}


XMarshRequest* XMarshRequestsCtl::GetRequest(int& what/*0 - first available, 1 - most recent*/) { // no concurrent calls may be made. 
	if(what == 1) { 
		return _recent_request; 
	} 
	XMarshRequest *req = 0;
	std::vector<XMarshRequest>::iterator it = _requests.begin();
	while(req == 0 && it != _requests.end()) {
		if(it->_vacant) {
			req = &(*it);
			req->_vacant = 0;

			_recent_request = req; 
		}
		else {
			++it;
		}
	}

	return req;
}


void XMarshRequestsCtl::WaitFor() {
	BOOL ok = FALSE;
	while(!ok) {
		ok = TRUE;
		std::vector<XMarshRequest>::iterator it = _requests.begin();
		while(ok && it != _requests.end()) {
			if(!it->_vacant) {
				ok = FALSE;
			}
			else {
				++it;
			}
		}
		if(!ok) {
			OSWait();
		}
	}
}


BOOL poll_client::read_dartmessage(SOCKET sock, const std::string& header, std::string& msg, int nread) {
	__int64 time_end = OSDayTimeInMilliseconds() + 4000; 

	BOOL is_dart = TRUE;
	size_t length = std::max(nread, 521);
	msg.resize(length);
	memcpy((PCHAR)msg.data(), (PCHAR)header.data(), nread);

	do {
		if((size_t)nread == msg.size()) {
			msg.resize(msg.size() * 2);
		}
		size_t z = msg.size() - nread; 
		size_t x = recvn(sock, (PCHAR)msg.data() + nread, std::min(521, (int)z), 0, 1/*secs time-out*/, true/*one_step*/);
		if(x > 0) { 
			nread += (int)x; 
		} 
		else { 
			break; 
		} 
		if(msg.data()[nread - 1] < 0x20) { 
			break; 
		} 

	} while(time_end > OSDayTimeInMilliseconds()); 

	msg.resize(nread); 
	if(nread < 6 || msg.data()[nread - 1] >= 0x20) { 
		is_dart = FALSE; 
	} 
	return is_dart; 
}

BOOL poll_client::read_vcrfmessage(SOCKET sock, const std::string& header, std::string& msg, int nread) {
	BOOL ok = TRUE;
	int len = nread == 6? atoi(header.c_str() + 1): 0;
	if(len > 0) {
		msg.reserve(len + 1);
		msg.resize(len);
		if(len != (int)msg.size() || len != (int)recvn(sock, (PCHAR)msg.data(), msg.size(), 0, 1/*secs time-out*/)) {
			LOGFailure(std::string("poll_client::read_vcrfmessage"), len, "Length");
			ok = FALSE;
		}
	}
	else {
		msg.resize(0);
	}
	return ok;
}

BOOL poll_client::read_fepmessage(SOCKET sock, const std::string& header, std::string& msg, int nread) {
	int length = 0;
	BOOL is_fep = TRUE;
	for(size_t x = 0; x < 4 && is_fep; ++x) {
		if(!isdigit(header[x])) {
			is_fep = FALSE;
		}
	}
	if(is_fep) {
		sscanf(header.data(), "%04d", &length);
		if(length >= 20 && length < 2048) {
			++length;
		}
		else {
			is_fep = FALSE;
		}
	}
	if(is_fep) {
		msg.reserve(length + 1);
		msg.resize(length);
		if(length <= nread) {
			memcpy((PCHAR)msg.data(), (PCHAR)header.data(), length);
			nread -= length;
			if(nread) {
				std::string next_header = header.substr(length, nread);
				std::string next_msg;
				if(read_fepmessage(sock, next_header, next_msg, (int)next_header.size())) {
					msg.resize(length + next_msg.size());
					memcpy((PCHAR)msg.data() + length, (PCHAR)next_msg.data(), next_msg.size());
				}
				else {
					LOGFailure(std::string("read_next_fepmessage failure"), next_header + next_msg);
					is_fep = FALSE;
				}
			}
		}
		else {
			memcpy((PCHAR)msg.data(), (PCHAR)header.data(), nread);
			length -= nread;
			nread = (int)recvn(sock, (PCHAR)msg.data() + nread, length, 0, 1/*secs time-out*/);
			if(nread != length) {
				is_fep = FALSE;
			}
		}
	}
	return is_fep;
}

BOOL poll_client::read_httpmessage(SOCKET sock, const std::string& header, std::string& msg, int nread) {
	BOOL is_http = TRUE; 
	if(memcmp(header.data(), "GET ", 4) == 0) { 
	} 
	else 
	if(memcmp(header.data(), "POST", 4) == 0) { 
	} 
	else 
	if(memcmp(header.data(), "OPTI", 4) == 0) { 
	} 
	else 
	if(memcmp(header.data(), "HEAD", 4) == 0) { 
	} 
	else 
	if(memcmp(header.data(), "PUT ", 4) == 0) { 
	} 
	else 
	if(memcmp(header.data(), "TRAC", 4) == 0) { 
	} 
	else 
	if(memcmp(header.data(), "DELE", 4) == 0) { 
	} 
	else { 
		is_http = FALSE; 
	} 

	if(is_http) { 
		// 1. Read start line and headers. It has to come in one chunk, no delay. 
		int length = 1024 + nread; 
		msg.reserve(length + 1);
		msg.resize(length); 
		memcpy((PCHAR)msg.data(), header.c_str(), nread); 
		size_t n; 
		while((n = recvn(sock, (PCHAR)msg.data() + nread, 1024, 0, 0/*secs time-out*/)) == 1024) { 
			nread += (int)n; 
			length = 1024 + nread; 
			msg.reserve(length + 1); 
			msg.resize(length); 
		} 
		nread += (int)n; 
		length = 0; 
		// 2. Search an empty line. 
		int x; 
		int y; 
		for(x = 4, y = 0; x < nread && y < 2; ++x) { 
			if(msg[x] == 0xA) { 
				if(++y == 2) { 
				} 
				else 
				if((x + 15) < nread && memcmp(&msg[x + 1], "Content-Length", 14) == 0) { 
					x += 15; 
					while(x < nread && msg[x] >= 0x20 && !isdigit(msg[x])) ++x; 
					if(isdigit(msg[x])) { 
						length = atoi(&msg[x]); 
					} 
					else { 
						--x; 
					} 
				} 
			} 
			else 
			if(msg[x] == 0xD) { 
			} 
			else { 
				y = 0; 
			} 
		} 

		if(y != 2) { 
			is_http = FALSE; 
		} 

		if(is_http && x <= nread) { 
			length -= nread - x; 
		} 
		if(is_http && length > 0) { 
			msg.reserve(nread + length + 1); 
			msg.resize(nread + length); 
			while(is_http && length > 0) {
				size_t chunk = length > 8192? 8192: length;
				if((n = recvn(sock, (PCHAR)msg.data() + nread, chunk, 0, 2/*secs time-out*/)) == (size_t)chunk) {
					nread += (int)n;
					length -= (int)n;
				}
				else {
					is_http = FALSE;
				}
			} 
		} 
	} 
	return is_http;
}

BOOL poll_client::read_ipcdatastring(SOCKET sock, IPCMsgheader& header, std::string& msg) {
	BOOL ok = TRUE;
	int len = header._len;
	msg.resize(0);
//	read data buffer from client.
	if(len > 0 && len < (4194304 * 20)) { // 4Mb * 20
		msg.reserve(len + 1);
		msg.resize(len);
		//if(len != (int)msg.size() || len != (int)recvn(sock, (PCHAR)msg.data(), msg.size(), 0, 8/*secs time-out*/ + (len >> 18))) {
		if(len != (int)msg.size() || len != (int)recvn(sock, (PCHAR)msg.data(), msg.size(), 0, -1/*secs time-out*/)) {
			LOGFailure(std::string("poll_client::read_ipcdatastring"), len, "Length");
			ok = FALSE;
		}
	}
	else
	if(len != 0) {
		if(_g_trace) {
			std::ostringstream ostr;
			ostr << "read_ipcdatastring() is requested to read strange-object of length" << ' ' << len << ';' << sockaddr_in_tostring(sock_peeraddr(sock));
			IPCLog(ostr.str()); 
		}
		ok = FALSE;
	}
	return ok;
}

inline BOOL IsDartMagicCharacter(char x) { 
	switch((int)x) { 
		case (int)'S': 
		case (int)'U': 
		case (int)'R': 
		case (int)'T': 
		case (int)'O': 
		case (int)'P': 
		case (int)'D': 
		case (int)'a': 
		case (int)'p': 
		case (int)'d': 
			return TRUE; 
	} 
	return FALSE; 
} 

BOOL poll_client::read_ipcrequest(SOCKET sock, IPCMsgheader& header, std::string& msg, int timeout_secs) {
	BOOL ok = TRUE;
	BOOL is_fep = FALSE;
	BOOL is_dart = FALSE;
	BOOL is_vcrf = FALSE;
	BOOL is_http = FALSE;
//	read argument buffer from client.
	msg.resize(0);
	int nread = (int)recvn(sock, (PCHAR)&header, 4/*bytes*/, 0, timeout_secs);
	if(nread == 1 && IsDartMagicCharacter(header._magicword[0])) { // 2011-05-02 Dart
		is_dart = TRUE;
		ok = FALSE;
	}
	else
	if(nread > 1 && header._magicword[1] == ',' && IsDartMagicCharacter(header._magicword[0])) { // 2011-05-02 Dart
		is_dart = TRUE;
		ok = FALSE;
	}
	else
	if(nread != 4) {
		return FALSE;
	}
	else
	if(header._magicword[0] == '*' && isdigit(header._magicword[1])) { // 2014-05-20 VCRF Vision CRF
		ok = FALSE;
		if((nread += (int)recvn(sock, ((PCHAR)&header) + 4, 2, 0, 0/*secs time-out*/)) != 6) { // header *XXXXXX
			if(_g_trace) {
				LOGFailure(std::string("poll_client::recvn wrong argument"), std::string((PCHAR)&header, nread));
			}
		}
		else {
			is_vcrf = TRUE;
		}
	}
	else
	if(isdigit(header._magicword[0])) { // 2011-04-28 Fep
		is_fep = TRUE;
		ok = FALSE;
	}
	else
	if(isalpha(header._magicword[0])) { // 2011-05-17 Http
		is_http = TRUE;
		ok = FALSE;
	}
	else
	if(!header.ISWSIMagicword()) {
		ok = FALSE;
	}
	else
	if((nread += (int)recvn(sock, ((PCHAR)&header) + 4, sizeof(WSIHeader) - 4, 0, 0/*secs time-out*/)) != (int)sizeof(WSIHeader)) {
		if(_g_trace) {
			LOGFailure(std::string("poll_client::recvn wrong argument"), std::string((PCHAR)&header, sizeof(WSIHeader)));
		}
		ok = FALSE;
	}
	else
	if(!header.FromHeader()) {
		ok = FALSE;
	}

	if(ok) {
		ok = read_ipcdatastring(sock, header, msg);
	}
	else
	if(is_fep) {
		if(!read_fepmessage(sock, std::string(header._magicword, nread), msg, nread)) {
			if(_g_trace) {
				LOGFailure(std::string("read_fepmessage failure"), msg);
			}
			msg.resize(0);
		}
		else {
			memset(&header, '?', sizeof(WSIHeader));
			memcpy(&header, &msg[4], 4);

			header._aux = IPCObj_FepMessage;
			header._ticketexpected = '1';
			header._format = 'F';
			header._len = (int)msg.size();
			header._data = (PCHAR)msg.data(); 
			header._tag = 0;

			header._trn = -1; 
			if(header._data[8] > ' ') { 
				sscanf(&header._data[8], "%02d", &header._trn);
			} 
			if(header._trn >= 0 && header._trn < 33) { 
				header._trn -= 1;
			} 

			ok = TRUE;
		}
	}
	else
	if(is_dart) {
		if(!read_dartmessage(sock, std::string(header._magicword, nread), msg, nread)) {
			if(_g_trace) {
				LOGFailure(std::string("read_dartmessage failure"), msg);
			}
			msg.resize(0);
		}
		else {
			memset(&header, '?', sizeof(WSIHeader));

			header._aux = IPCObj_DartMessage;
			header._trn = -1; 
			header._ticketexpected = '0';
			header._format = 'D';
			header._len = (int)msg.size();
			header._data = (PCHAR)msg.data(); 
			header._tag = 0;

			ok = TRUE;
		}
	}
	else
	if(is_http) {
		if(!read_httpmessage(sock, std::string(header._magicword, nread), msg, nread)) {
			if(_g_trace) {
				LOGFailure(std::string("read_httpmessage failure"), msg);
			}
			msg.resize(0);
		}
		else { 
			memset(&header, '?', sizeof(WSIHeader)); 
			memcpy(&header, &msg[0], 4); 

			header._aux = IPCObj_HttpMessage; 
			header._trn = -1; 
			header._ticketexpected = '1'; 
			header._format = 'H'; 
			header._len = (int)msg.size(); 
			header._data = (PCHAR)msg.data(); 
			header._tag = 0; 

			ok = TRUE; 
		} 
	} 
	else
	if(is_vcrf) {
		if(!read_vcrfmessage(sock, std::string(header._magicword, nread), msg, nread)) {
			if(_g_trace) {
				LOGFailure(std::string("read_vcrfmessage failure"), msg);
			}
			msg.resize(0);
		}
		else {
			memset(&header, '?', sizeof(WSIHeader));
			memcpy(&header, "VCRF", 4);

			header._aux = IPCObj_CRFData;
			header._trn = -1;
			header._ticketexpected = '0';
			header._format = 'W';
			header._len = (int)msg.size();
			header._data = (PCHAR)msg.data();
			header._tag = 0;

			ok = TRUE;
		}
	}

	return ok; 
} 

BOOL poll_client::read_rawdata(SOCKET sock, IPCMsgheader& header, std::string& msg) {
	__int64 time_end = OSDayTimeInMilliseconds() + 100; 
	size_t nread = 0; 

	msg.resize(128);
	do {
		if(nread == msg.size()) {
			msg.resize(msg.size() * 2);
		}
		size_t x = recvn(sock, (PCHAR)msg.data() + nread, (int)msg.size() - nread, 0, 0/*secs time-out*/, true/*one_step*/);
		if(x > 0) { 
			nread += x; 
		} 
		else { 
			break; 
		} 

	} while(time_end > OSDayTimeInMilliseconds()); 

	msg.resize(nread); 

	memset(&header, '?', sizeof(WSIHeader));

	header._aux = IPCObj_RawData;
	header._trn = -1; 
	header._ticketexpected = '1';
	header._format = 'R';
	header._len = (int)msg.size();
	header._data = (PCHAR)msg.data(); 
	header._tag = 0;

	return nread > 0; 
} 

BOOL poll_client::marsh_request(SOCKET sock, int index, IPCMsgheader& header, std::string& msg, XConnector *conn, IXFunction<int, XMarshRequest*>& getRequest) {
	std::string ret_obj;
	BOOL ok = TRUE;
	if(header._aux == IPCObj_LogMessage) {
		std::string log_msg = XTrimString(header._connectorname, sizeof(header._connectorname)) + ": " + msg; 
		if(conn && conn->_host_ptr) { 
			int t = -1; 
			TLSObject *tlsobj = GetTLSObject2();
			if(tlsobj) { 
				t = tlsobj->_index; 
				sockaddr_in peer = sock_peeraddr(sock);
				if(check_loopaddress(peer)) { 
					tlsobj->_index = peer.sin_port;
				} 
				else { 
					tlsobj->_index = -1; 
				} 
			} 
			conn->_host_ptr->IPCLog(log_msg); 
			if(tlsobj) { 
				tlsobj->_index = t;
			} 
		} 
		else { 
			OUTPUTLogMessage(log_msg);
		} 
	}
	else
	if(header._aux == IPCObj_Connect) {
//		10/20/2008 - Server reuses existing connection instead of creating a new connection.
//		04/27/2009 - Send version object on connect.
		std::ostringstream ostr;
		ostr << ' ' << "><" << ' ' << "XVersion" << ' ' << "Version" << '=' << XXVERSION << ' ';
		ret_obj = ostr.str();
	}

	switch(header._aux) {
		case IPCObj_ReceiveObj:
		case IPCObj_FepMessage:
		case IPCObj_DartMessage:
		case IPCObj_CRFData:
		case IPCObj_HttpMessage:
		case IPCObj_RawData: {
			XMarshRequest localrequest;
			int request_type;
			BOOL do_local;

			XMarshRequest *pRequest = 0;
			do {
				do_local = FALSE;

				if(XMarshRequestsCtl::_s_pipe_pctl && !XMarshRequestsCtl::_s_pipe_pctl->_vacant) {
					request_type = 1;
					pRequest = (XMarshRequest*)XMarshRequestsCtl::_s_pipe_pctl->_aparameter;
				}
				else {
					request_type = 0;
					pRequest = getRequest(request_type);
				}

				if(!pRequest) {
					pRequest = getRequest(request_type = request_type == 1? 0: 1);
				}
				if(!pRequest) {
					request_type = 0;
					pRequest = &localrequest;
					do_local = TRUE;
				}
				if(request_type == 1) {
					XMarshRequest *pTemp = pRequest;
					pTemp->_new_data_gate.lock();
					if(!pRequest->_vacant) {
						XMarshRequest *apc_request = XMarshRequestEx::GetRequest(request_type);
						if(apc_request) {
							apc_request->_index = index;
							apc_request->_sock = sock;
							apc_request->_pConn = conn;
							apc_request->_copydata = header;
							apc_request->_msg.swap(msg);
							pRequest->_new_data.push_back(apc_request);
							if(conn) {
								conn->_listener._apc_queue_depth = (int)pRequest->_new_data.size();
							}
						}
						else
						if(conn->_listener._max_parallel_requests > 1) {
							request_type = 0;
							pRequest = &localrequest;
							do_local = TRUE;
						}
						else {
							pRequest = 0;
							OSWait();
						} 
					} 
					else { 
						pRequest = 0; 
					} 
					pTemp->_new_data_gate.unlock();
				} 
				if(request_type == 0) {
					pRequest->_index = index;
					pRequest->_sock = sock;
					pRequest->_pConn = conn;
					pRequest->_copydata = header;
					pRequest->_msg.swap(msg);

					if(!do_local) {
						if(!QueueWorkItem(MarshRequestHandler, (PVOID)pRequest, (PVOID)XMarshRequestsCtl::_s_pipe_pctl)) {
							if(XMarshRequestsCtl::_s_pipe_pctl) {
								pRequest->_msg.swap(msg);
								pRequest->Release();
								pRequest = 0;
							}
							else
							if(!QueueWorkItem(MarshRequestHandler, (PVOID)pRequest)) {
								do_local = TRUE;
							}
						}
					}
				}
			} while(pRequest == 0);

			if(request_type == 0) {
				if(do_local) {
					MarshRequestHandler(pRequest);
				}
			}
		}
		break;
		default:
			if(header._ticketexpected == '1') {
				ok = SendReturnTicket(sock, header._format, ret_obj, header._trn);
			}
		break;
	}

	return ok;
}

return_t XRemoting::RunXMarshRequestPipe() {
	if(XMarshRequestsCtl::_s_pipe_pctl == 0) {
		XMarshRequestsCtl::_s_pipe_pctl = new XThreadCtl();
	}
	return RunAFunction(*XMarshRequestsCtl::_s_pipe_pctl);
}




IPCMsgheader::IPCMsgheader(): WSIHeader() {
	_aux = IPCObj_InvalidHeader;
	_tag = 0;
	_len = 0;
	_trn = 0; 

	_data = 0; 

	_in_buffer_size = 0; 
	_out_buffer_size = 0; 
}

IPCMsgheader::IPCMsgheader(const std::string& name, __int32 aux, char format, int trn, int version): WSIHeader() {
	Init(name, aux, format, trn, version);
	ToHeader();
}

IPCMsgheader::IPCMsgheader(const std::string& name, __int32 aux, char format, const std::string& msg, int trn, int version): WSIHeader() {
	Init(name, aux, format, trn, version);
	if(msg.size()) {
		_data = (PCHAR)msg.c_str();
		_len = (int)msg.size();
	}
	ToHeader();
}

void IPCMsgheader::Init(const std::string& name, __int32 aux, char format, int trn, int version) {
	_tag = 0;
	_len = 0;
	_data = 0;

	_in_buffer_size = 0; 
	_out_buffer_size = 0; 

	_aux = aux;

	_tag = version & 0xff;

	_trn = trn;

	_format = format == 'V'? 'W': format;

	size_t nlen = name.size();
	if(nlen > sizeof(_connectorname)) {
		nlen = sizeof(_connectorname);
	}
	if(nlen > 1) {
		memcpy(_connectorname, name.c_str(), nlen);
	}
	if(nlen < sizeof(_connectorname)) {
		_connectorname[nlen] = ' ';
	}
}

bool IPCMsgheader::ToHeader() {
	bool ok = true;
	switch(_aux) {
		case IPCObj_Response: _msgtype = '0'; break;
		case IPCObj_ReceiveObj: _msgtype = '1'; break;
		case IPCObj_LogMessage: _msgtype = '2'; break;
		case IPCObj_Connect: _msgtype = '3'; break;
		case IPCObj_InvalidHeader: _msgtype = '4'; break;
		case IPCObj_FepMessage: _msgtype = '5'; break;
		case IPCObj_DartMessage: _msgtype = '6'; break;
		case IPCObj_HttpMessage: _msgtype = '7'; break;
		case IPCObj_RawData: _msgtype = '8'; break;
		case IPCObj_CRFData: _msgtype = '9'; break;
		default: ok = false; _msgtype = '?';
	}

	char buf[16];
	int bufsize = 1;

	if((bufsize = sprintf(buf, "%d", (int)_len)) > (int)sizeof(_datalen)) {
		ok = false;
		memset(_datalen, '?', sizeof(_datalen)); 
	}
	else { 
		memcpy(_datalen, buf, bufsize);
	} 
	if(bufsize < (int)sizeof(_datalen)) {
		_datalen[bufsize] = ' ';
	}


	if((_tag & 0xf) > 9) {
		_headerversion = '?'; 
		ok = false;
	}
	else { 
		_headerversion = (char)((_tag & 0xf) + '0');
	} 


	if((bufsize = sprintf(buf, "%d", _trn)) > (int)sizeof(_transaction)) {
		ok = false;
		memset(_transaction, '?', sizeof(_transaction)); 
	}
	else { 
		memcpy(_transaction, buf, bufsize);
	} 
	if(bufsize < (int)sizeof(_transaction)) {
		_transaction[bufsize] = ' ';
	}

	return ok;
}

bool IPCMsgheader::FromHeader() {
	bool ok = ISWSIMagicword();

	switch((int)_msgtype) {
		case (int)'0': _aux = IPCObj_Response; break;
		case (int)'1': _aux = IPCObj_ReceiveObj; break;
		case (int)'2': _aux = IPCObj_LogMessage; break;
		case (int)'3': _aux = IPCObj_Connect; break;
		case (int)'4': _aux = IPCObj_InvalidHeader; break;
		case (int)'5': _aux = IPCObj_FepMessage; break;
		case (int)'6': _aux = IPCObj_DartMessage; break;
		case (int)'7': _aux = IPCObj_HttpMessage; break;
		case (int)'8': _aux = IPCObj_RawData; break;
		case (int)'9': _aux = IPCObj_CRFData; break;
		default: ok = false;
	}

	_tag = (int)(_headerversion - '0');

	sscanf(_transaction, "%d", &_trn);
	sscanf(_datalen, "%d", &_len);

	return ok;
}

bool IPCMsgheader::TransactionToHeader() { 
	bool ok = true; 

	char buf[16];
	int bufsize = 1;

	if((bufsize = sprintf(buf, "%d", _trn)) > (int)sizeof(_transaction)) {
		ok = false;
		memset(_transaction, '?', sizeof(_transaction)); 
	}
	else { 
		memcpy(_transaction, buf, bufsize);
	} 

	return ok;
} 

bool IPCMsgheader::TransactionFromHeader() { 
	bool ok = true; 

	_trn = -1; 

	if(sscanf(_transaction, "%d", &_trn) != 1) { 
		ok = false; 
	} 

	return ok; 
} 


bool IPCMsgheader::IsIPCRequest(IPCMsgheader& header) {
	switch(header._aux) {
		case IPCObj_ReceiveObj:
		case IPCObj_LogMessage:
		case IPCObj_Connect:
		case IPCObj_DartMessage:
		case IPCObj_CRFData:
		case IPCObj_RawData:
			return true;
	}
	if(header._aux == IPCObj_FepMessage && isdigit(header._magicword[0])) {
		int mid = atoi(header._magicword);
		switch(mid) {
		case 1: case 3: case 40: case 42: case 43:
			return true;
		}
	}
	if(header._aux == IPCObj_HttpMessage && isalpha(header._magicword[0])) {
		if(memcmp(header._magicword, "HTTP", 4) != 0) { 
			header._ticketexpected = '1'; 
			return true; 
		} 
	}
	return false;
}

bool IPCMsgheader::IsIPCResponse(IPCMsgheader& header) {
	switch(header._aux) {
		case IPCObj_Response:
			return true;
	}
	if(header._aux == IPCObj_FepMessage && isdigit(header._magicword[0])) {
		int mid = atoi(header._magicword);
		switch(mid) {
		case 2: case 4: case 5: case 41:
			return true;
		}
	}
	if(header._aux == IPCObj_HttpMessage && isalpha(header._magicword[0])) {
		if(memcmp(header._magicword, "HTTP", 4) == 0) { 
			header._ticketexpected = '0'; 
			return true; 
		} 
	}
	return false;
}




IPCTransactions _g_ipctransactions; 
#if defined(__BORLANDC__)
IPCReadTransaction IPCTransactions::_transactions[40]; 
#endif


int IPCTransactions::GetSuggestedTransactionNumber(IPCMsgheader& header) { 
	int trn = header._trn; 

	switch(header._aux) { 
	case IPCObj_DartMessage: return -1; 
	case IPCObj_CRFData: return -1;
	case IPCObj_FepMessage: 
		if(trn < 0 && header._data && header._len > 10) { 
			sscanf(&header._data[8], "%02d", &trn);
			if(trn >= 0 && trn < 33) { 
				trn -= 1;
			} 
		} 
	} 
	return trn; 
} 

void IPCTransactions::FreeTransaction(IPCReadTransaction& transaction) { 
	transaction._transaction = -1; 
	transaction._sock = INVALID_SOCKET; 
	transaction._response.resize(0); 
	transaction._vacant = 1; 

	InterlockedDecrement(&_active_transactions_cnt); 
} 

IPCReadTransaction * IPCTransactions::AllocateTransaction(int trn, SOCKET sock, int timeout_secs) { 
	int trn_saved = trn; 

	if(trn >= (int)ARRAY_NUM_ELEMENTS(_transactions)) { 
		return NULL; 
	} 

	IPCReadTransaction *ptransaction = NULL; 

	bool done = false; 
	while(!done) { 
		trn = trn_saved; 

		if(trn > -1) { 
			while(!_transactions[trn]._vacant) { 
				OSWait(); 
			} 
			ptransaction = &_transactions[trn]; 
		} 
		else { 
			while(ptransaction == 0) { 
				for(size_t x = ARRAY_NUM_ELEMENTS(_transactions); x > 0 && !ptransaction; --x) { 
					if(_transactions[x - 1]._vacant) { 
						ptransaction = &_transactions[x - 1]; 
						trn = (int)(x - 1); 
					} 
				} 
				if(!ptransaction) { 
					OSWait(); 
				} 
			} 
		} 

		_gate.lock(); 

		if(ptransaction->_vacant) { 
			if(ptransaction->_vacant < 0) { 
				OSSemaphoreInit(ptransaction->_event);
			} 
			ptransaction->_transaction = trn; 
			ptransaction->_sock = sock; 

			ptransaction->_expiration_time = OSDayTimeInMilliseconds() + (__int64)(timeout_secs >= 0? timeout_secs * 1000: 60000); 

			ptransaction->_vacant = 0; 

			done = true; 
		} 

		if(done) { 
			InterlockedIncrement(&_active_transactions_cnt); 
		} 

		_gate.unlock(); 
	} 

	return ptransaction; 
} 

BOOL IPCTransactions::PostResponse(int trn, SOCKET sock, IPCMsgheader& header, std::string& response) { 
	if(trn > -1 && trn >= (int)ARRAY_NUM_ELEMENTS(_transactions)) { 
		return FALSE; 
	} 

	_gate.lock(); 

	BOOL rc = FALSE; 

	IPCReadTransaction *ptransaction = NULL; 

	if(trn < 0) { 
		for(size_t x = ARRAY_NUM_ELEMENTS(_transactions); x > 0; --x) { 
			if(_transactions[x - 1]._vacant == 0) { 
				IPCReadTransaction& transaction = _transactions[x - 1]; 
				if(transaction._transaction < 0 && transaction._sock == sock) { 
					ptransaction = &transaction; 
				} 
			} 
		} 
	} 
	else { 
		IPCReadTransaction& transaction = _transactions[trn]; 
		if(transaction._vacant == 0 && transaction._transaction == trn && transaction._sock == sock) { 
			ptransaction = &transaction; 
		} 
	} 

	if(ptransaction) { 
		rc = ptransaction->PostResponse(header, response); 
	} 

	_gate.unlock(); 

	return rc; 
} 

void IPCTransactions::CheckExpiredTransactions() { 
	if(_active_transactions_cnt) { 
		__int64 time_now = OSDayTimeInMilliseconds(); 

		for(size_t x = 0; x < ARRAY_NUM_ELEMENTS(_transactions); ++x) { 
			if(_transactions[x]._vacant == 0) { 
				_gate.lock(); 

				IPCReadTransaction& transaction = _transactions[x]; 
				if(transaction._vacant == 0 && (int)(time_now - transaction._expiration_time) > 0) { 
					std::string response; 
					transaction.PostResponse(IPCMsgheader(), response); 
				} 

				_gate.unlock(); 
			} 
		} 
	} 
} 

void IPCTransactions::CloseSocketTransactions(SOCKET sock) { 
	if(_active_transactions_cnt && sock != INVALID_SOCKET) { 

		for(size_t x = 0; x < ARRAY_NUM_ELEMENTS(_transactions); ++x) { 
			if(_transactions[x]._vacant == 0) { 
				_gate.lock(); 

				IPCReadTransaction& transaction = _transactions[x]; 
				if(transaction._vacant == 0 && transaction._sock == sock) { 
					std::string response; 
					transaction.PostResponse(IPCMsgheader(), response); 
				} 

				_gate.unlock(); 
			} 
		} 
	} 
} 




BOOL IPCReadTransaction::Receive(IPCMsgheader& header, std::string& response) { 
	OSSemaphoreWait(_event);

	header = _header; 
	response.swap(_response); 

	return TRUE; 
} 

BOOL IPCReadTransaction::PostResponse(const IPCMsgheader& header, std::string& response) { 
	BOOL rc = FALSE; 

	_header = header; 
	_response.swap(response); 
	
	if(_g_trace & 4) {
		LOGMessage(std::string("IPCReadTransaction::PostResponse"), this->_transaction, (int)this->_sock, std::string("posting..."));
	}

	if(OSSemaphorePost(_event) < 0) { 
		_response.swap(response); 
	} 
	else { 
		rc = TRUE; 
	} 

	if(_g_trace & 4) {
		LOGMessage(std::string("IPCReadTransaction::PostResponse"), this->_transaction, (int)rc, std::string("posted"));
	}
	return rc; 
} 






SSocketCtl::SSocketCtl() {
	_error = 0; 
	_raw_data = 0; 
	_sock = INVALID_SOCKET;
	_sockaddr.sin_family = AF_UNSPEC;
}

SSocketCtl& SSocketCtl::operator=(const SSocketCtl& other) { 
	_sockaddr = other._sockaddr;
	_sock = other._sock;
	_error = other._error;
	_raw_data = other._raw_data; 
	return *this; 
} 

void SSocketCtl::Reset() {
	_g_ipctransactions.CloseSocketTransactions(_sock);
	_sock = INVALID_SOCKET;
	_sockaddr.sin_family = AF_UNSPEC;
}

bool SSocketCtl::Close(SSocketCtl& ctl) { 
	bool rc = false; 
	if(ctl._sock != INVALID_SOCKET) {
		ctl.LOCKER.lock();
		if(ctl._sock != INVALID_SOCKET) {
			closesocket(ctl._sock);
			ctl.Reset();
			rc = true; 
		} 
		ctl.LOCKER.unlock();
	}
	return rc; 
}


