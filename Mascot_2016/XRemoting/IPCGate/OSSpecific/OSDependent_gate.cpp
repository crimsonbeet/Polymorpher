// Created by Alex Nikol Bourakov (ANB) around April/07

#ifdef __BORLANDC__
#pragma warn -8057
#endif

#include "stdafx_gate.h"



#include ".\..\..\Include\ExportSTLMacro.h"




#ifdef _UCC
void IPCGateDllMain() {
}

#elif defined(__GCC__)
void  __attribute__((constructor)) IPCGateDllMain(){
}

#endif



#ifdef _UCC
int _ftime(struct _timeb *tp) {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	tp->time = tv.tv_sec;
	tp->millitm = (unsigned short)(tv.tv_usec / 1000);
	tp->dstflag = 0;
	tp->timezone = 0;

	return 0;
}

__int64 _atoi64(const char *str) {
	__int64 rc = 0;
	sscanf(str, "%Ld", &rc);
	return rc;
}

#endif





#if defined(_WIN32_WINDOWS)
#if defined(_M_IX86) && !defined(__BORLANDC__)
__int64 EXP MyQueryTickCounter(void) {
    _asm    _emit 0x0F
    _asm    _emit 0x31
}
#else
clock_t EXP MyQueryTickCounter() {
	return clock();
}
#endif
#endif

#if defined(__BORLANDC__)
#include <sys\timeb.h>
#define _ftime ftime
#define _timeb timeb
#endif

__int64 EXP OSDayTimeInMilliseconds() {
	_timeb tmnow;
	memset(&tmnow, 0, sizeof(tmnow));
	_ftime(&tmnow);

	__int64 secs = tmnow.time - 40 * (__int64)31556926;

	return (secs * 1000) + tmnow.millitm;
}



VOID EXP OSWait(PVOID do_sleep) {
#ifdef _UCC
	static long s_oswait_mode = 0; 
	switch(((++s_oswait_mode)&0x3)) { 
	case 0: 
		sched_yield();
		break; 
	case 1: 
		tsleep(2);
		break; 
	case 2: 
		tsleep(1);
		break; 
	} 

#elif defined(__GCC__)
	sched_yield();

#elif defined(_WIN32_WINDOWS)
	long long ticks = MyQueryTickCounter(); 
	if(do_sleep != NULL || (ticks % 11) == 0) {
		timeBeginPeriod(1);
		Sleep(1);
		timeEndPeriod(1);
	}
	else {
		SwitchToThread();
	}

#endif
}

#ifdef _UCC

BOOL *_g_signalvalue = 0;
BOOL _g_signalinitialized = 0;

extern "C" void SignalInterceptor(int aSignal) {
	if(_g_signalvalue != NULL) {
		if(aSignal == 9) {
			*_g_signalvalue = aSignal;
		}
		if(aSignal == 0x3300) {
			signal(0x3300, SignalInterceptor); 
		} 
	}
}

VOID EXP OSSetSignalValueReceiver(BOOL& signalvalue) {
	_g_signalvalue = &signalvalue;
	if(_g_signalvalue != NULL && !_g_signalinitialized) {
		intercept(SignalInterceptor);
		signal(0x3300, SignalInterceptor); 
	}
}
#endif


VOID EXP OSSleep(DWORD millisecs) {
#ifdef _UCC
	millisecs = std::floor((double)millisecs / 10.0 + 0.5); 
	if(millisecs == 0) { 
		millisecs = 1; 
	} 
	tsleep(millisecs);

#elif defined(__GCC__)
	sleep(millisecs);

#elif defined(_WIN32_WINDOWS)
	SleepEx(millisecs, TRUE);

#endif
}

int EXP OSPipe(int fd[2]) {
#ifdef _UCC
	return pipe((unsigned int*)fd);

#elif defined(__GCC__)
	return pipe(fd);

#elif defined(_WIN32_WINDOWS)
	return _pipe(fd, 8192, _O_TEXT);

#endif
}

int EXP OSPipeclose(int fd[2]) {
	int rc = 0;
#ifdef _UCC
	if(close(fd[0]) < 0) {
		rc = -1;
	}
	if(close(fd[1]) < 0) {
		rc = -1;
	}
#else
	if(_close(fd[0]) < 0) {
		rc = -1;
	}
	if(_close(fd[1]) < 0) {
		rc = -1;
	}
#endif
	return rc;
}


std::string EXP FormatSystemError(DWORD dwErr) {
#ifdef _UCC
	return std::string((const char*)strerror(dwErr));

#elif defined(__GCC__)
	return std::string((const char*)strerror(dwErr));

#elif defined(_WIN32_WINDOWS)
	std::string out;
	LPVOID lpMsgBuf;
	DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	if(FormatMessageA(dwFlags, 0, dwErr, 0, (LPSTR)&lpMsgBuf, 0, 0)) {
		out = (const char*)lpMsgBuf;
		LocalFree(lpMsgBuf);
	}
	return out;

#endif
}


#if !defined(_WIN32_WINDOWS)
int EXP GetLastError() {
	return errno;
}
#endif





// Some compilers believe that
// pointers to C++ functions have a different type than pointers to C functions.
// The solution is to declare a typedef and use casting.
#if !defined(_WIN32_WINDOWS)
extern "C" typedef return_t (*AFPtr)(PVOID);
#endif


int EXP OSThreadCreate(pthread_t& hthread, return_t (__stdcall *AFunction)(PVOID), PVOID AParameter, DWORD *pIdThread, DWORD priority) {
	int rc = -1;

#ifdef _UCC
	static pthread_attr_t attr;
	static int attr_ok;
	if(!attr_ok) {
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, 16384);
		attr_ok = 1;
	}
	rc = pthread_create(&hthread, &attr, AFPtr(AFunction), AParameter); 
	if(rc == 0 && priority != 0) { 
		_pthread_setpr(hthread, priority); 
	} 

#elif defined(__GCC__)
	rc = pthread_create(&hthread, NULL, AFPtr(AFunction), AParameter);

#elif defined(_WIN32_WINDOWS)
	hthread = CreateThread(0, 0, AFunction, AParameter, 0, pIdThread);
	if(hthread && priority != 0) {
		SetThreadPriority(hthread, priority == 128? THREAD_PRIORITY_NORMAL: priority > 128? THREAD_PRIORITY_HIGHEST: THREAD_PRIORITY_LOWEST); 
		rc = 0;
	}

#endif

	return rc;
}

DWORD EXP OSThreadCurrent() {
#ifdef _UCC
	return getpid(); 

#elif defined(__GCC__)
	return (DWORD)pthread_self();

#elif defined(_WIN32_WINDOWS)
	return (DWORD)GetCurrentThreadId();

#endif
}

pthread_t EXP OSThreadHandle() {
#ifdef _UCC
	return pthread_self();

#elif defined(__GCC__)
	return pthread_self();

#elif defined(_WIN32_WINDOWS)
	pthread_t th;
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &th, 0, FALSE, DUPLICATE_SAME_ACCESS);
	return th;

#endif
}

void EXP OSFreeThreadHandle(pthread_t& hthread) {
#if defined(_WIN32_WINDOWS)
	CloseHandle(hthread);
	hthread = 0;
#endif
}

int EXP OSThreadJoin(pthread_t& hthread) {
#ifdef _UCC
	int rc = 0;
	pthread_join(hthread, NULL);

#elif defined(__GCC__)
	int rc = 0;
	pthread_join(hthread, NULL);

#elif defined(_WIN32_WINDOWS)
	int rc = 0;
	WaitForSingleObject(hthread, INFINITE);
	CloseHandle(hthread);

#endif

	return rc;
}





int EXP OSSemaphoreInit(sem_t& s) {
#ifdef _UCC
	int rc = 0;
	_os_sema_init(&s);
	_os_sema_p(&s);

#elif defined(__GCC__)
	int rc = 0;
	sem_init(&s, 0, 0);

#elif defined(_WIN32_WINDOWS)
	int rc = 0;
	s = CreateEvent(0, 0, 0, 0);

#endif

	return rc;
}


int EXP OSSemaphoreWait(sem_t& s, BOOL bAlertable) {
	int rc = 0;

#ifdef _UCC
	errno = 0;
	while(_os_sema_p(&s) != 0) {
		if(bAlertable) {
			rc = -1;
			break;
		}
		errno = 0;
	}

#elif defined(__GCC__)
	errno = 0;
	while(sem_wait(&s) < 0 && errno == EINTR) {
		if(bAlertable) {
			rc = -1;
			break;
		}
		errno = 0;
	}

#elif defined(_WIN32_WINDOWS)
	if(WAIT_OBJECT_0 != WaitForSingleObjectEx(s, INFINITE, bAlertable)) {
		rc = -1;
	}

#endif

	return rc;
}

int EXP OSSemaphoreDestroy(sem_t& s) {
#ifdef _UCC
	int rc = 0;
	_os_sema_term(&s);

#elif defined(__GCC__)
	int rc = 0;
	sem_destroy(&s);

#elif defined(_WIN32_WINDOWS)
	int rc = 0;
	CloseHandle(s);

#endif

	return rc;
}

int EXP OSSemaphorePost(sem_t& s) {
#ifdef _UCC
	int rc = 0;
	errno = 0;
	while(_os_sema_v(&s) != 0) {
		errno = 0;
	}

#elif defined(__GCC__)
	int rc = 0;
	sem_post(&s);

#elif defined(_WIN32_WINDOWS)
	int rc = 0;
	SetEvent(s);

#endif

	return rc;
}



#if !defined(_WIN32_WINDOWS)
#ifdef _UCC
long int add_atom(long int volatile *ptr, long int add_value) {
	long int value = 0;
	_asm("
%3
	lwarx %0,0,%1
	add %0,%0,%2
	dcbt 0,%1
	stwcx. %0,0,%1
	bne- %3
	",
	__reg_gen(__obj_modify(value)), __reg_gen(ptr), __reg_gen(add_value),__label(), __reg_nofunccall());
	return value;
}
LONG EXP InterlockedIncrement(LONG volatile* Addend) {
	return add_atom(Addend, 1);
}
LONG EXP InterlockedDecrement(LONG volatile* Addend) {
	return add_atom(Addend, -1);
}
#else
LONG EXP InterlockedIncrement(LONG volatile* Addend) {
	return ++(*Addend);
}
LONG EXP InterlockedDecrement(LONG volatile* Addend) {
	return --(*Addend);
}
#endif
void EXP InitializeCriticalSection(CRITICAL_SECTION *hmutex) {
	pthread_mutex_init(hmutex, NULL);
}
void EXP EnterCriticalSection(CRITICAL_SECTION *hmutex) {
	pthread_mutex_lock(hmutex);
}
void EXP LeaveCriticalSection(CRITICAL_SECTION *hmutex) {
	pthread_mutex_unlock(hmutex);
}
void EXP DeleteCriticalSection(CRITICAL_SECTION *hmutex) {
	pthread_mutex_destroy(hmutex);
}

#endif




HANDLE EXP OSRDONLYOpenFile(const std::string& filename) {
	HANDLE handle = (HANDLE)-1;

#ifdef _UCC
	handle = open(filename.c_str(), O_RDONLY);

#elif defined(__GCC__)
	handle = open(filename.c_str(), O_RDONLY, S_IRWXU);

#elif defined(_WIN32_WINDOWS)
	handle = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

#endif

	return handle;
}


HANDLE EXP OSWRONLYOpenFile(const std::string& filename, int append_mode) {
	HANDLE handle = (HANDLE)-1;

#ifdef _UCC
	if(append_mode) { 
		handle = open(filename.c_str(), S_IREAD|S_IWRITE);
	} 
	if(handle == (HANDLE)-1) { 
		handle = creat(filename.c_str(), FAM_WRITE|FAM_READ); 
	} 

#elif defined(__GCC__)
	handle = open(filename.c_str(), O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

#elif defined(_WIN32_WINDOWS)
	handle = CreateFileA(filename.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL/*|FILE_FLAG_WRITE_THROUGH*/, NULL);

#endif

	return handle;
}


int EXP OSSEEKFileSize(HANDLE handle) {
	int len = 0;

#ifdef _UCC
	len = lseek(handle, 0, SEEK_END);
	if(len > 0) {
		lseek(handle, 0, SEEK_SET);
	}

#elif defined(__GCC__)
	len = lseek(handle, 0, SEEK_END);
	if(len > 0) {
		lseek(handle, 0, SEEK_SET);
	}

#elif defined(_WIN32_WINDOWS)
	DWORD lDistLow = GetFileSize(handle, 0);
	len = (int)lDistLow;

#endif

	return len;
}

// inp_str should be properly sized.
// size of inp_str indicates how many bytes to read.
int EXP OSReadStringFromFile(HANDLE handle, std::string& inp_str) {
	int len = 0;

#ifdef _UCC
	len = read(handle, (char*)inp_str.data(), inp_str.size());

#elif defined(__GCC__)
	len = read(handle, (void*)inp_str.data(), inp_str.size());

#elif defined(_WIN32_WINDOWS)
	DWORD dw;
	ReadFile(handle, (void*)inp_str.data(), (DWORD)inp_str.size(), &dw, 0);
	len = (int)dw;

#endif
	if(len != (int)inp_str.size()) {
		if(len > 0 && len < (int)inp_str.size()) {
			inp_str.resize((int)len);
		}
		else {
			inp_str.resize(0);
			len = 0;
		}
	}

	return len;
}


int EXP OSWriteStringToFile(HANDLE handle, const std::string& out_str) {
	int rc = -1;
	int len = 0;

#ifdef _UCC
	len = write(handle, out_str.c_str(), out_str.size());

#elif defined(__GCC__)
	len = write(handle, out_str.c_str(), out_str.size());

#elif defined(_WIN32_WINDOWS)
	DWORD dw;
	if(!WriteFile(handle, out_str.c_str(), (DWORD)out_str.size(), &dw, NULL)) { 
		std::cout << FormatSystemError(GetLastError()) << std::endl; 
	} 
	else { 
		SetEndOfFile(handle);
	} 
	len = (int)dw;

#endif

	if(len == (int)out_str.size()) {
		rc = len;
	}

	return rc;
}



int EXP OSAppendStringToFile(HANDLE handle, const std::string& out_str) {
	int rc = -1;
	int len = 0;

#ifdef _UCC
	lseek(handle, 0, SEEK_END);
	len = write(handle, out_str.c_str(), out_str.size());

#elif defined(__GCC__)
	lseek(handle, 0, SEEK_END);
	len = write(handle, out_str.c_str(), out_str.size());

#elif defined(_WIN32_WINDOWS)
	DWORD dw;
	SetFilePointer(handle, 0, NULL, FILE_END);
	WriteFile(handle, out_str.c_str(), (DWORD)out_str.size(), &dw, NULL);
	len = (int)dw;

#endif

	if(len == (int)out_str.size()) {
		rc = len;
	}

	return rc;
}



int EXP OSCloseFile(HANDLE handle) {
	int rc = -1;

#ifdef _UCC
	rc = close(handle);

#elif defined(__GCC__)
	rc = close(handle);

#elif defined(_WIN32_WINDOWS)
	rc = CloseHandle(handle);

#endif

	return rc;
}



int EXP OSDeleteFile(const std::string& filename) {
	int rc = -1;

#ifdef _UCC
	rc = remove(filename.c_str()) == 0? 1: 0;

#elif defined(__GCC__)
	rc = remove(filename.c_str()) == 0? 1: 0;

#elif defined(_WIN32_WINDOWS)
	rc = DeleteFile(filename.c_str()); 

#endif

	return rc;
}



