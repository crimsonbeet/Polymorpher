// stdafx.cpp : source file that includes just the standard includes
// SimpleProtocol.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx_simpleprotocol.h"

#define WSICLASSFACTORY_IMPL

#include "SimpleXClasses.h"





#if defined(_MSC_VER)

#include <psapi.h>

int EnumerateMyProcesses() {
	DWORD aProcesses[2048];
	DWORD cbNeeded;

	TCHAR szMyProcessName[MAX_PATH];
	HANDLE hProcess = GetCurrentProcess();
	HMODULE hMod;
	if(!EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
		return 0;
	}
	GetModuleBaseName(hProcess, hMod, szMyProcessName, sizeof(szMyProcessName)/sizeof(TCHAR));
	CloseHandle(hProcess);

	int ncount = 0;
	if(EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded) ) {
		int cProcesses = cbNeeded / sizeof(DWORD);
		for(int j = 0; j < cProcesses; ++j) {
			TCHAR szProcessName[MAX_PATH];

			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|PROCESS_SET_QUOTA, FALSE, aProcesses[j]);
			if(NULL != hProcess) {
				HMODULE hMod;
				if(EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
					GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
					if(_tcsicmp(szProcessName, szMyProcessName) == 0) {
						++ncount;
					}
				}
				CloseHandle(hProcess);
			}
		}
	}
	return ncount;
}
#endif
