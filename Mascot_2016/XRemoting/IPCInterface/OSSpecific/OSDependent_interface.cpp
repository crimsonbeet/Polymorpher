// Created by Alex Nikol Bourakov (ANB) around April/07

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "stdafx_interface.h"


extern PIPCObjInterface _g_wsiremoting;
extern BOOL _g_captstdcout_active;

wsi_gate _g_loghandler_gate;


#if defined(_WIN32_WINDOWS)
HWND _g_loghandler = 0;

#else
HWND _g_loghandler = -1;

#endif

IXDelegate<const std::string> _g_loghandler_delegates;


#ifdef _UCC
void IPCInterfaceDllMain() {
}


#elif defined(__GCC__)
pid_t _g_processid = -1;
void __attribute__((constructor)) IPCInterfaceDllMain() {
	_g_processid = getpid();
}

#endif



void EXP __stdcall IPCSetLogHandler(HWND handler) {
	_g_loghandler_gate.lock();
	_g_loghandler = handler;
	_g_loghandler_gate.unlock();
#if defined(_WIN32_WINDOWS)
	BOOL redirect_stdout = handler != 0;
#else
	BOOL redirect_stdout = handler != -1;
#endif
	if(redirect_stdout) {
		RedirectStandardOutput();
	}
	else {
		RestoreStandardOutput();
	}
}

void EXP __stdcall IPCSetLogDelegate(int (*obj)(const std::string&)) { 
	_g_loghandler_delegates.Add(obj); 
} 

void EXP __stdcall IPCRemoveLogDelegate(int (*obj)(const std::string&)) { 
	_g_loghandler_delegates.Remove(obj); 
} 


PIPCObjInterface EXP __stdcall IPCObjectGetInterface() {
#ifdef _UCC
	if(!_g_wsiremoting) {
		LOGFailure(std::string("IPCObjectGetInterface"), std::string("was not initialized yet."));
	}

#elif defined(__GCC__)
	if(!_g_wsiremoting) {
		const char ipcobject[] = "./libipcobject.so";
		if(dlopen(ipcobject, RTLD_NOW) == NULL) {
			LOGFailure(std::string("dlopen"), dlerror());
		}
	}

#elif defined(_WIN32_WINDOWS)
	if(!_g_wsiremoting) {
		HMODULE hModule = GetModuleHandle(_T("IPCInterface.dll"));
		TCHAR dllPath[_MAX_PATH];
		GetModuleFileName(hModule, dllPath, _MAX_PATH); 
		for(size_t j = strlen(dllPath) - 1; j > 0; --j) { 
			if(dllPath[j] == (TCHAR)'\\') { 
				break; 
			} 
			dllPath[j] = 0; 
		} 
#if defined(_MSC_VER)
		strcat(dllPath, "IPCObject.dll"); 
#elif defined(__BORLANDC__)
		strcat(dllPath, "IPCObjectBorland.dll"); 
#endif
		LoadLibrary(dllPath);
	}

#endif

	return _g_wsiremoting;
}



BOOL EXP __stdcall OUTPUTLogMessage(const std::string& msg) {
	BOOL rc = TRUE;

	bool is_delegated = false; 
	if(_g_loghandler_delegates.Size()) { 
		is_delegated = _g_loghandler_delegates(msg); 
	} 

	_g_loghandler_gate.lock();

#if defined(_WIN32_WINDOWS)
	if(_g_loghandler) {
		COPYDATASTRUCT copydata;
		copydata.dwData = 0;
		copydata.cbData = (DWORD)msg.size() + 1;
		copydata.lpData = (PVOID)msg.c_str();

		LRESULT done = 0;
		while(!done && _g_loghandler) {
			SetLastError(0);
			done = SendMessage(_g_loghandler, WM_COPYDATA, (WPARAM)0, (LPARAM) & copydata);
			if(GetLastError()) {
				rc = FALSE;
				break;
			}
		}
	}
	else 
	if(!is_delegated) {
#if defined(__BORLANDC__)
		OutputDebugStringA(msg.c_str());
#else
		if(NO_DEFAULT_COUT == FALSE && !_g_captstdcout_active) {
			std::cout << msg << std::endl;
		}
#endif
	}

#else
	if(_g_loghandler != -1) {
		DWORD dw = write(_g_loghandler, msg.data(), msg.size());
		write(_g_loghandler, "\r\n", 2);
		if(msg.size() != dw) {
			rc = FALSE;
		}
	}
	else 
	if(!is_delegated && NO_DEFAULT_COUT == FALSE && !_g_captstdcout_active) {
		std::cout << msg << std::endl;
	}

#endif

	_g_loghandler_gate.unlock();

	return rc;
}

