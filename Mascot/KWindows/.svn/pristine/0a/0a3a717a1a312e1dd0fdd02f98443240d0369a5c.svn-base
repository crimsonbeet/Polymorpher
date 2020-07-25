// stdafx.cpp : source file that includes just the standard includes
// KWindows.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#define WSICLASSFACTORY_IMPL

#include "FrameWnd.h"

void PrintLastError(DWORD dwErr) { 
	if(dwErr == 0) { 
		dwErr = GetLastError(); 
	} 
	char szErr[4096];
	memset( szErr, 0, sizeof( szErr ) );

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, 0, szErr, sizeof(szErr), NULL);
	std::cout << "Win Error : " <<  szErr << std::endl; 
} 

