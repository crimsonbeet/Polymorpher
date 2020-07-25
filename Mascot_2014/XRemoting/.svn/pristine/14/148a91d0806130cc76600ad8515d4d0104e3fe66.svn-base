// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef ipc_xmediaH
#define ipc_xmediaH



class XSubscriber;
class XConnector;
class XListener;
class XProducer;
class XRemoting;
class XWritingHead;
class XReadingHead;
class XLoggingHead;



struct SSocketCtl: CRefCounter {
	sockaddr_in _sockaddr;
	SOCKET volatile _sock;
	wsi_reenterantgate LOCKER;

	int _error;
	int _raw_data; 

	SSocketCtl& operator=(const SSocketCtl&); 

	SSocketCtl(); 

	void Reset(); 
	static bool Close(SSocketCtl& ctl); 
};



template <typename T> struct XSocket2Handler {
	sockaddr_in _peeraddr;
	SOCKET _sock;
	T *_handler;

	int _index; 
	int _raw_data; 

	__int64 _timeout_threshold; 

	XSocket2Handler() {
#ifdef _WIN32_WINDOWS
		_peeraddr.sin_addr.S_un.S_addr = 0;
#else
		_peeraddr.sin_addr.s_addr = 0;
#endif
		_sock = INVALID_SOCKET;
		_handler = 0; 
		_index = 0; 
		_raw_data = 0; 
		_timeout_threshold = 0; 
	}

	XSocket2Handler(SOCKET sock, const sockaddr_in& peeraddr, int index = 0, int raw_data = 0) {
		_index = index; 
		_raw_data = raw_data; 
		_peeraddr = peeraddr;
		_sock = sock;
		_handler = 0;
		_timeout_threshold = 0; 
	}
};

typedef XSocket2Handler<XProducer> *PSocket2XProducer;

typedef std::map<SOCKET, XSocket2Handler<XProducer> > sockclients_map;


struct SPollProducersCtl {
	XConnector *_connector;
	BOOL _busy;

	std::vector<XSocket2Handler<XProducer> * > _new_clients;

	SPollProducersCtl(XConnector *connector) {
		_connector = connector;
		_busy = FALSE;
	}

	void WaitFor() {
		while(_busy) {
			OSWait();
		}
	}

};



struct XWorkItem: CRefCounter {
	std::string *_pitem;
	const char *_format_name;
	int _status;
	int _version;

	XWorkItem *_pNext; 

	static int s_nstaticCntFreedEntries; 
	static XWorkItem* s_pstaticFreedEntries; 
	static int s_nstaticCntAllocatedEntries; 
	static wsi_gate s_gate; 

	void*operator new(size_t Size); 
	void operator delete(void *p); 

	XWorkItem(bool allocate_pitem = true);
	~XWorkItem();
};


class XReadingHead {
	fd_set _allsockets;
	int _maxsock;
	sockclients_map _clientsmap;
	wsi_reenterantgate _allsocketsgate;

public:
	pthread_t _hmonitor;
	static return_t CALLBACK Monitor(PVOID pv);
	DWORD _thread;
	BOOL _threadshutdown;
	BOOL _isrunning;

	SOCKET Listen();
	SSocketCtl _handler;

	void AddSocket(SSocketCtl&, int index = 0);
	bool RemoveSocket(SSocketCtl&);

	XListener *_host_ptr;

	void Initialize(XListener *host_ptr);
	void Start();
	void Startthread();
	void Shutdown();
	void Shutdownthread();

	XConnector * Connector();
};


class XWritingHead {
public:
	static return_t __stdcall PostFunction(LPVOID lpParameter);

	SSocketCtl _target;
	bool _keep_alive; // when true, the XReadingHead should have this socket in its list, so Monitor()::select() will process a request. 

	std::string BuildWellFormedAddress(const std::string& addr);

	bool SendXWorkItem(XWorkItem *item, IWSIMarshaller *marsh);
	bool PostXWorkItem(XWorkItem *item);

	XSubscriber *_host_ptr;

	void Initialize(XSubscriber *host_ptr);
	bool Connect(std::string* my_addr_out = 0);
	bool IsValid();
	bool IsLocalAddress();
	bool IsDefaultWriter(); 

	bool Shutdown();

	XSocket2Handler<XProducer> * TearOffAHandler(XProducer *handler);

	XConnector * Connector();
	XListener * Listener();
	XReadingHead * Reader();

	bool _initialized;
};


class XLoggingHead {
	wsi_gate _locker;
	SOCKET volatile _sock;
public:
	XLoggingHead() {
		_sock = INVALID_SOCKET;
	}
	void Set(SOCKET sock) {
		_locker.lock();
		_sock = sock;
		_locker.unlock();
	}
	SOCKET Get() {
		return _sock;
	}
	bool IsValid() {
		return _sock != INVALID_SOCKET && _sock != NULL;
	}
	void Reset() {
		_locker.lock();
		_sock = INVALID_SOCKET;
		_locker.unlock();
	}
};


#endif //ipc_xmediaH

