// IPCObject.cpp : Defines the entry point for the DLL application.
//

// Created by Alex Nikol Bourakov (ANB) around April/07

#include "stdafx_ipcobject.h"

#if defined(_WIN32_WINDOWS)

#pragma hdrstop
#if defined(__BORLANDC__)
#pragma argsused
#endif

extern "C" int _libmain(unsigned long reason)
{
	return 1;
}

HINSTANCE _g_module;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
			_g_module = (HINSTANCE)hModule;
#if defined(_MSC_VER)
			hModule = GetModuleHandle("IPCInterface.dll");
#elif defined(__BORLANDC__)
			hModule = GetModuleHandle("IPCInterfaceBorland.dll");
#endif
			if(hModule) {
				InitializeIPCInterface();
			}
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

	return TRUE;
}
#endif
