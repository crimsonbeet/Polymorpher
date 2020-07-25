// KWindows.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "controls.h"  

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) { 
	INITCOMMONCONTROLSEX icex;
	//This is where we create the rebar control
	// First, we initialize the common controls
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_COOL_CLASSES | ICC_USEREX_CLASSES;	// load the rebar and combo
	InitCommonControlsEx(&icex);

    return TRUE;
}

