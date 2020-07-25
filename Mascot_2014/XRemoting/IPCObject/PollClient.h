// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef pollclientH
#define pollclientH



std::string make_uniquename(const std::string& name, sockaddr_in saddr, int index);

size_t recvn(SOCKET sock, char *vptr, size_t nsize, int flags = 0, int timeout_secs = -1, bool one_step = false);

sockaddr_in sock_peeraddr(const SOCKET& sock);
sockaddr_in sock_myaddr(const SOCKET& sock);




struct WSIHeader {
	char _magicword[3]; // ==>
	char _headerversion; // 0, 1, 2, 3...
	char _msgtype; // 0, 1, 2, 3, 4, 5, 6, 7...
	char _ticketexpected; // 0 or 1
	char _transaction[3]; // position in _g_ipctransactions, -1 means search by socket number. 
	char _format; // 'W' - wsi, 'X' - xml, 'F' - fep, 'D' - dart, 'H' - http
	char _connectorname[17];
	char _datalen[9]; // left justified.

	WSIHeader() {
		strcpy(_magicword, "==>");
		memset(&_headerversion, ' ', &_datalen[9] - &_headerversion);
	}

	bool ISWSIMagicword() {
		return memcmp(_magicword, "==>", 3) == 0;
	}
};

struct IPCMsgheader: WSIHeader {
	__int32 _aux; 
	__int32 _tag; // On send - Masked values: 0xff - my version number. On connect: 0x100 - no keep alive flag. 
	__int32 _len; 
	__int32 _trn; 

	PCHAR _data; 

	size_t _in_buffer_size; // set the new receive buffer size on create socket.
	size_t _out_buffer_size; // set the new send buffer size on create socket.

	IPCMsgheader();
	IPCMsgheader(const std::string& name, __int32 aux, char format, int trn = -1, int version = XXVERSION);
	IPCMsgheader(const std::string& name, __int32 aux, char format, const std::string& msg, int trn = -1, int version = XXVERSION);

	void Init(const std::string& name, __int32 aux, char format, int trn, int version);

	bool ToHeader();
	bool FromHeader();

	bool TransactionToHeader(); 
	bool TransactionFromHeader(); 

	static bool IsIPCRequest(IPCMsgheader& header);
	static bool IsIPCResponse(IPCMsgheader& header);
};




struct IPCReadTransaction { 
	int _transaction; 
	SOCKET _sock; 
	std::string _response; 
	IPCMsgheader _header; 
	sem_t _event; 
	int _vacant; 

	__int64 _expiration_time; // time in milliseconds 

	BOOL Receive(IPCMsgheader& header, std::string& response); 
	BOOL PostResponse(const IPCMsgheader& header, std::string& response); 

	IPCReadTransaction(): _transaction(-1), _vacant(-1), _sock(INVALID_SOCKET), _expiration_time(0) { 
		memset(&_event, 0, sizeof(_event)); 
	} 
}; 


struct IPCTransactions { 
	wsi_gate _gate; 
#if defined(__BORLANDC__)
	static 
#endif
	IPCReadTransaction _transactions[40]; 

	LONG _active_transactions_cnt; 

	int GetSuggestedTransactionNumber(IPCMsgheader& header); 
	IPCReadTransaction * AllocateTransaction(int trn, SOCKET sock, int timeout_secs = -1);
	void FreeTransaction(IPCReadTransaction& transaction); 

	void CheckExpiredTransactions(); 

	void CloseSocketTransactions(SOCKET sock); 

	BOOL PostResponse(int trn, SOCKET sock, IPCMsgheader& header, std::string& response); 

	IPCTransactions(): _active_transactions_cnt(0) { 
	} 
}; 

extern IPCTransactions _g_ipctransactions; 




struct XMarshRequest {
	SOCKET _sock;
	int _index;
	XConnector *_pConn;
	IPCMsgheader _copydata;
	std::string _msg;
	volatile unsigned char _vacant;

	CWSIWriter _wsiwriter;
	CXMLWriter _xmlwriter;
	CJSONWriter _jsonwriter;
	CFepWriter _fepwriter;
	CHttpWriter _httpwriter;
	CRawWriter _rawwriter;

	std::deque<XMarshRequest*> _new_data;
	wsi_gate _new_data_gate; 

	XMarshRequest() {
		_index = 0;
		_sock = INVALID_SOCKET;
		_pConn = 0;
		_vacant = 1;
	}

	virtual void Release() {
		_vacant = 1;
	}
	virtual ~XMarshRequest() { 
	} 
};

struct XMarshRequestEx: public XMarshRequest {
	static XMarshRequestEx _storage[512]; 
	volatile static XMarshRequestEx *_head; 
	static wsi_gate _gate; 

	static XMarshRequestEx* InitializeStorage() { 
		for(size_t x = 0; x < (ARRAY_NUM_ELEMENTS(_storage) - 1); ++x) { 
			_storage[x]._next = &_storage[x + 1]; 
		} 
		_storage[ARRAY_NUM_ELEMENTS(_storage) - 1]._next = 0; 
		return &_storage[0]; 
	} 

	XMarshRequestEx *_next; 
	static XMarshRequest* GetRequest(int&) {
		XMarshRequestEx *prequest = 0; 
		_gate.lock(); 
		prequest = (XMarshRequestEx*)_head; 
		if(_head) { 
			_head = _head->_next; 
		} 
		_gate.unlock(); 
		return prequest; 
	}
	void Release() {
		_gate.lock(); 
		_next = (XMarshRequestEx*)_head; 
		_head = this; 
		_gate.unlock(); 
	}
	virtual ~XMarshRequestEx() { 
	} 
};

struct XMarshRequestsCtl { // Main usage: it is declared on the stack of XReadingHead::Monitor() (i.e. calls to it may be made from a single thread only). it manages requests specific to a Listener.
	std::vector<XMarshRequest> _requests;
	XConnector *_pConn;

	static XThreadCtl *_s_pipe_pctl; // it is used when it is desired to process in one thread all the requests from all listeners (in main() for instance).

	XMarshRequest *_recent_request; 

	XMemFunctionType<XMarshRequestsCtl, int, XMarshRequest*> _delegGetRequest;

	XMarshRequestsCtl(XConnector *conn, int max_pos = 16): _delegGetRequest(&XMarshRequestsCtl::GetRequest, (XMarshRequestsCtl*)0) {
		_pConn = conn;
		if(max_pos > 0) { 
			_requests.resize(max_pos);
		} 
		_delegGetRequest.Bc = this;
		_recent_request = 0; 
	}

	XMarshRequest* GetRequest(int& what/*0 - first available, 1 - most recent request*/);

	void WaitFor();
};


class poll_client {
	fd_set *_pAllset;
	int& _maxsock;
	sockclients_map& _clmap;

	fd_set *_pRset;

public:
	XMarshRequestsCtl& _ctl; // a reference to a XMarshRequestsCtl instance (reqs_ctl of XReadingHead::Monitor()).

	poll_client(XMarshRequestsCtl& ctl, sockclients_map& clmap, fd_set *allset, int& maxsock, fd_set *rset):
		_maxsock(maxsock), _pAllset(allset), _clmap(clmap), _pRset(rset), _ctl(ctl) {
	}

	~poll_client();

	void operator() (sockclients_map::value_type& client);

	static BOOL read_rawdata(SOCKET sock, IPCMsgheader& header, std::string& msg);
	static BOOL read_ipcrequest(SOCKET sock, IPCMsgheader& header, std::string& msg, int timeout_secs);
	static BOOL read_ipcdatastring(SOCKET sock, IPCMsgheader& header, std::string& msg);
	static BOOL marsh_request(SOCKET sock, int index, IPCMsgheader& header, std::string& msg, XConnector *conn, IXFunction<int, XMarshRequest*>& getRequest);
	static BOOL read_fepmessage(SOCKET sock, const std::string& header, std::string& msg, int nread);
	static BOOL read_dartmessage(SOCKET sock, const std::string& header, std::string& msg, int nread);
	static BOOL read_httpmessage(SOCKET sock, const std::string& header, std::string& msg, int nread);
};


class add_client {
	fd_set *_pAllset;
	int& _maxsock;
	sockclients_map& _clmap;

public:

	add_client(sockclients_map& clmap, fd_set *allset, int& maxsock):
		_maxsock(maxsock), _pAllset(allset), _clmap(clmap) {
	}

	void operator() (PSocket2XProducer& pHandler); // used in conjjunction with new clients procesing.
	void operator() (const XSocket2Handler<XProducer>& handler);
};

#endif //pollclientH
