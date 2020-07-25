// Created by Alex Nikol Bourakov (ANB) around April/07
#pragma once

#ifndef ipcgateH
#define ipcgateH


#ifndef EXP
#define EXP IMPORTIPCINTERFACE
#endif


#if defined(_WIN32_WINDOWS)
#if defined(_M_IX86) && !defined(__BORLANDC__)
__int64 EXP MyQueryTickCounter(void);
#else
clock_t EXP MyQueryTickCounter();
#endif
#endif


__int64 EXP OSDayTimeInMilliseconds(); // time in milliseconds from 2010/01/01.


VOID EXP OSWait(PVOID = 0);
VOID EXP OSSleep(DWORD millisecs);
#ifdef _UCC
VOID EXP OSSetSignalValueReceiver(BOOL& signalvalue);
#endif

int EXP OSPipe(int fd[2]);
int EXP OSPipeclose(int fd[2]);

int EXP OSSemaphoreInit(sem_t& s);
int EXP OSSemaphoreWait(sem_t& s, BOOL bAlertable = FALSE);
int EXP OSSemaphoreDestroy(sem_t& s);
int EXP OSSemaphorePost(sem_t& s);

int EXP OSThreadCreate(pthread_t& hthread, return_t (__stdcall *AFunction)(PVOID), PVOID AParameter, DWORD *pIdThread = 0, DWORD priority = 128);
int EXP OSThreadJoin(pthread_t& hthread);
DWORD EXP OSThreadCurrent(); // returns thread_id, not a thread handle.

pthread_t EXP OSThreadHandle();
void EXP OSFreeThreadHandle(pthread_t& hthread);



struct TLSSecurityToken {
	int _accesslevel;
	long long int _expiration_time;
	TLSSecurityToken(): _accesslevel(0), _expiration_time(0){
	}
};


struct EXP TLSObject { 
	char _connectorname[18]; 
	int _index; 
	int _onreturn_shutdownsocket; 
	pthread_t _hthread;

	TLSSecurityToken *_stoken;

	void Init(); 
	void SetConnectorName(char *name, int size = -1); 

	PVOID GetRpcHandle(); // returns top entry of _rpchandles; can return zero. 
	bool PushRpcHandle(PVOID); // pushes into _rpchandles; can return false when _rpchandles is full. 
	PVOID PopRpcHandle(); // removes and returns top entry of _rpchandles; can return zero. 

private: 
	PVOID _rpchandles[16]; // stack of pointers to XRPCHandle that allow for calling the _marshaller of current callback call.
	int _rpchandlespos; // current stack position.
}; 

EXP TLSObject __stdcall GetTLSObject(); // based on GetTLSObject2(). 
VOID EXP __stdcall SetTLSObject(TLSObject& obj); // based on GetTLSObject2(). If GetTLSObject2() returns null, it does nothing. 

EXP TLSObject* __stdcall GetTLSObject2(); // can return null if set-function was not called before. 
VOID EXP __stdcall SetTLSObject2(TLSObject *obj); // just replaces the current pointer with the input parameter. It has to be used on top of the call-stack. It does not free the old memory, so pointer to auto variable can be used. 


return_t EXP __stdcall QueueWorkItem(return_t (__stdcall *afunction)(PVOID), PVOID aparameter = 0, PVOID athreadctl = 0);
void EXP __stdcall CleanupWorkItems();
void EXP __stdcall WaitforWorkItems();
int EXP __stdcall CountActiveWorkItems();
void EXP __stdcall SignalWorkItems(int iSignal = 9); 


#if !defined(_WIN32_WINDOWS)
int EXP GetLastError();
LONG EXP InterlockedIncrement(LONG volatile* Addend);
LONG EXP InterlockedDecrement(LONG volatile* Addend);
void EXP InitializeCriticalSection(CRITICAL_SECTION*);
void EXP EnterCriticalSection(CRITICAL_SECTION*);
void EXP LeaveCriticalSection(CRITICAL_SECTION*);
void EXP DeleteCriticalSection(CRITICAL_SECTION*);
#endif


std::string EXP FormatSystemError(DWORD dwErr);






HANDLE EXP OSRDONLYOpenFile(const std::string& filename);
HANDLE EXP OSWRONLYOpenFile(const std::string& filename, int append_mode = 0);
int EXP OSCloseFile(HANDLE handle);
int EXP OSDeleteFile(const std::string& filename); 

int EXP OSSEEKFileSize(HANDLE handle);

// inp_str should be properly sized.
// size of inp_str indicates how many bytes to read.
int EXP OSReadStringFromFile(HANDLE handle, std::string& inp_str);

// size of out_str indicates how many bytes to write.
int EXP OSWriteStringToFile(HANDLE handle, const std::string& out_str);
int EXP OSAppendStringToFile(HANDLE handle, const std::string& out_str);

std::string EXP ReadTextFile(const std::string& file_name);
std::string EXP GetFileNameExtension(const std::string& file_name);

int EXP ReadTextFile(const std::string& filename, std::string& inp_str);
int EXP WriteTextFile(const std::string& filename, const std::string& out_str);
int EXP AppendTextFile(const std::string& filename, const std::string& out_str);




#if defined(_WIN32_WINDOWS)
struct IWsiStorage {
	virtual void Synch() = 0;

	virtual void Init(const std::string& folder, const std::string& pattern, int maxsize = 4194304) = 0;

	virtual void Rewind() = 0;
	virtual bool Read(std::string& row) = 0;
	virtual bool Append(const std::string& row) = 0;

	virtual const std::string& GetHandle() = 0;
	virtual int GetCurrentPos() = 0;
	virtual void SetPosition(const std::string& filename, int position) = 0;
	virtual int FindHandlePosition(const std::string& filename) = 0;
};

typedef IWsiStorage *PIWsiStorage;
typedef IWsiStorage IWsiStorageClient;
typedef IWsiStorage *PIWsiStorageClient;

PIWsiStorage EXP CREATEIWsiStorageObject();
VOID EXP
DESTROYIWsiStorageObject(PIWsiStorage*);

PIWsiStorageClient EXP CREATEIWsiStorageClientObject();
VOID EXP
DESTROYIWsiStorageClientObject(PIWsiStorageClient*);
#endif





BOOL EXP XTrimString(std::string& s);
std::string EXP XTrimString(char *str, int nsize = -1); 
VOID EXP String_2Lower(std::string& s);
VOID EXP String_2Upper(std::string& s);


#ifdef _UCC
int _ftime(struct _timeb *tp);
#endif





struct api_handle {
	void *_api;
};

class XIPCObjectException {
public:
	BOOL _is_connected;
	int _type; // 0 - generic, 1 - subscriber, 2 - producer.
	int _index; 
	std::string _name;
	std::string _addr;
	int _error; // as returned by errno.

	XIPCObjectException() {
		_is_connected = FALSE;
		_type = 0;
		_error = 0;
	}
};

class XChangeableAddress {
public:
	std::string _name;
	std::string _addr;
	std::string _description;
	int _index; 
	bool operator == (const std::string& name) const {
		return _name == name;
	}
	bool operator != (const std::string& name) const {
		return _name != name;
	}
	bool operator == (const XChangeableAddress& other) const {
		return _name == other._name && _index == other._index;
	}
	bool operator != (const XChangeableAddress& other) const {
		return _name != other._name || _index != other._index;
	}
};

class XRemotingSettings {
public:
	std::vector<XChangeableAddress> _changeable_addresses;
	std::vector<std::string> _subscribers2ignore;
	std::vector<std::string> _listeners2ignore;
};




//#define WSI_GATE_USE_HOMEGROWN_SPINLOCK
//#define WSI_REENTGATE_USE_HOMEGROWN_SPINLOCK


class EXPORTIPCINTERFACE wsi_gate {
public:
	wsi_gate();
	wsi_gate(const wsi_gate&);
	wsi_gate& operator=(const wsi_gate&);

	~wsi_gate();

	void lock();
	void unlock();

private:
#if defined(WSI_GATE_USE_HOMEGROWN_SPINLOCK)
#if defined(_MSC_VER)
	_declspec(align(4))
#endif
	long int _cref; 
#else
	sem_t _gate; 
	unsigned char _alive;
#endif
	void Init();

public: 
	unsigned char _terminating;
};

class EXPORTIPCINTERFACE wsi_reenterantgate {
public:
	wsi_reenterantgate();
	wsi_reenterantgate(const wsi_reenterantgate&);
	wsi_reenterantgate& operator=(const wsi_reenterantgate&);

	~wsi_reenterantgate();

	void lock();
	void lock(DWORD threadid);
	void unlock();

private:
	DWORD _cthreadid;

#if defined(WSI_REENTGATE_USE_HOMEGROWN_SPINLOCK)
#if defined(_MSC_VER)
	_declspec(align(4))
#endif
	long int _cref; 
#else
	sem_t _gate; 
	unsigned char _alive;
#endif

	void Init();

public:
	unsigned char _terminating;
	unsigned char _threadcref;

};

struct EXPORTIPCINTERFACE CRefCounter {
#if defined(_MSC_VER)
	_declspec(align(4))
#endif
	long int _cref;

	CRefCounter();
	virtual ~CRefCounter();

	long int AddRef();
	long int Release();
};





/*
 * Some helper constructs follow next.
 */


// A helper macro to find out a number of elements in array.

template <int N> struct array_size_struct {
	unsigned char c[N];
};

template <class T, int N>
	array_size_struct<N> const& static_array_size(T (&)[N]);

#define ARRAY_NUM_ELEMENTS(ar) sizeof(static_array_size(ar).c)



// A helper supporting a greedy approach to memory allocation (object is never freed).
// Not to be created on stack. 

template<typename T> 
struct cached_work_objects: public wsi_gate { 
	struct snext_item { 
		T _item; 
		snext_item *_pnext; 
		snext_item(): _pnext(0) { 
		} 
	}; 
	T* get_object() { 
		T *pitem; 
		lock(); 
		if(_phead == 0) { 
			pitem = &((new snext_item())->_item); 
		} 
		else { 
			snext_item *p = _phead; 
			_phead = p->_pnext; 
			pitem = &p->_item; 
		}
		unlock(); 

		return pitem;
	} 

	void return_object(T* item) { 
		lock(); 
		snext_item *p = (snext_item*)item; 
		p->_pnext = _phead; 
		_phead = p; 
		unlock(); 
	} 

	cached_work_objects(): _phead(0) { 
	} 

	snext_item *_phead; 
}; 




// A helper to pass a member function(one argument) to FOR_EACH algorithm.
// It is also a base for delegates: see below.

template <typename Arg, typename Rval>
class IXFunction: public std::unary_function<Arg, Rval> {
public:
	virtual Rval operator()(Arg& obj) = 0;
	virtual bool operator == (const IXFunction<Arg, Rval>& func) const = 0;
	virtual ~IXFunction() {}
	void *Bc;
};



// Two classes take advantage of IXFunction interface:
// 1. Ones that wrap regular function pointer: XFunctionType.
// 2. Ones that wrap class member function: XMemFunctionType.


template <typename Arg, typename Rval>
class XFunctionType: public IXFunction<Arg, Rval> {
	Rval (*Fn)(Arg&);

public:
	XFunctionType(Rval (*f)(Arg&)): Fn(f) {
		this->Bc = 0;
	}

	~XFunctionType() {}

	Rval operator()(Arg& obj) {
		return (Fn)(obj);
	}

	bool operator == (const IXFunction<Arg, Rval>& func) const {
		if(func.Bc == 0) {
			const XFunctionType<Arg, Rval>& other = static_cast<const XFunctionType<Arg, Rval>& >(func);
			return other.Fn == this->Fn;
		}
		return false;
	}
};




template <class Base, typename Arg, typename Rval>
class XMemFunctionType: public IXFunction<Arg, Rval> {
	Rval (Base::*Fn)(Arg&);

public:
	XMemFunctionType(Rval (Base::*f)(Arg&), Base *b): Fn(f) {
		this->Bc = b;
	}

	~XMemFunctionType() {}

	Rval operator()(Arg& obj) {
		return (((Base*)(this->Bc))->*Fn)(obj);
	}

	bool operator == (const IXFunction<Arg, Rval>& func) const {
		if(func.Bc == this->Bc) {
			const XMemFunctionType<Base, Arg, Rval>& other = static_cast<const XMemFunctionType<Base, Arg, Rval>& >(func);
			return other.Fn == this->Fn;
		}
		return false;
	}
};






// helpers to establish XMemFunctionType

// Generate XMemFunctionType on stack.
template <class Base, typename Arg, typename Rval>
XMemFunctionType<Base, Arg, Rval>
XMemFunction(Rval (Base::*f)(Arg&), Base *b) {
	return XMemFunctionType<Base, Arg, Rval>(f, b);
}

// Generate XMemFunctionType in memory.
template <class Base, typename Arg, typename Rval>
XMemFunctionType<Base, Arg, Rval> *
AllocXMemFunction(Rval (Base::*f)(Arg&), Base *b) {
	return new XMemFunctionType<Base, Arg, Rval>(f, b);
}



// A helper that compares class instances.
// A class is supposed to overload operator ==.
// It is used with an algorithm that searches a Delegate in a container.

template <typename T, typename V=T>
struct IXEqual {
	const V& _val;
	IXEqual(const V& val): _val(val) {
	}
	bool operator () (const T& obj) {
		return obj == _val;
	}
	bool operator () (const T *ptr) {
		return *ptr == _val;
	}
};



// callback Delegates.


template <typename Arg, typename Rval = int >
class IXDelegate: public wsi_reenterantgate {
	typedef IXFunction<Arg, Rval>* delegatetype_ptr;

	std::vector<delegatetype_ptr> _delegates;

public:
	size_t Size() {
		return _delegates.size();
	}

	void Add(IXFunction<Arg, Rval> *new_delegate) {
		this->lock();
		if(new_delegate && std::count(_delegates.begin(), _delegates.end(), new_delegate) == 0) {
			_delegates.push_back(new_delegate);
		}
		this->unlock();
	}
	void Remove(IXFunction<Arg, Rval> *old_delegate) {
		this->lock();
		_delegates.erase(std::remove_if(_delegates.begin(), _delegates.end(), IXEqual<delegatetype_ptr>(old_delegate)), _delegates.end());
		this->unlock();
	}
	void Delete(const IXFunction<Arg, Rval>& delegate) {
		this->lock();
		std::vector<delegatetype_ptr>::iterator iter = std::remove_if(_delegates.begin(), _delegates.end(), IXEqual<IXFunction<Arg, Rval> >(delegate));
		std::vector<delegatetype_ptr>::iterator it = iter;
		while(it != _delegates.end()) {
			delete *it;
			++it;
		}
		_delegates.erase(iter, _delegates.end());
		this->unlock();
	}

	template <class Base>
	void Add(Rval (Base::*f)(Arg&), Base *b) {
		IXFunction<Arg, Rval> *pdelegate = AllocXMemFunction(f, b);
		Add(pdelegate);
	}
	template <class Base>
	void Remove(Rval (Base::*f)(Arg&), Base *b) {
		XMemFunctionType<Base, Arg, Rval> rdelegate(f, b);
		Delete(rdelegate);
	}

	void Add(Rval (*f)(Arg&)) {
		IXFunction<Arg, Rval> *pdelegate = new XFunctionType<Arg, Rval>(f);
		Add(pdelegate);
	}
	void Remove(Rval (*f)(Arg&)) {
		XFunctionType<Arg, Rval> rdelegate(f);
		Delete(rdelegate);
	} 

	bool operator()(Arg &arg) { // returns true if delegate was available.
		bool rc = false;
		this->lock();
		for(int x = 0; (size_t)x < _delegates.size(); ++x) {
			size_t s = _delegates.size();

			(*(_delegates[x]))(arg); // Do call delegate. It is allowed to remove some delegates. Or add some. But not add and remove.

			if(s > _delegates.size()) { // Adjust current pointer if Remove was called. Can work only if the current and processed were removed.
				x -= (int)(s - _delegates.size());
				if(x < -1) {
					x = -1;
				}
			}
			rc = true;
		}
		this->unlock();
		return rc;
	}
};


// A little helper to initialize elements of a container via FOR_EACH algorithm.

#if !defined(_UCC)
template<typename M, M S> void _set_value(M& obj) {
	obj = S;
}
#endif

template<typename M>
struct _set_object_value {
	const M& _val;
	_set_object_value(const M& val): _val(val) {
	}
	void operator()(M& arg) {
		arg = _val;
	}
};


template<typename T>
T range_rand(T upper_boundary) {
//	r is a random floating point value in the range [0,1) {including 0, not including 1}
	double r = ((double)rand() / ((double)(RAND_MAX) + 1.0));
//	returns a random number of type T in the range [0,M) {including 0, not including M}
//	M - upper_boundary
	return (T)(r * upper_boundary);
}



#endif //ipcgateH

