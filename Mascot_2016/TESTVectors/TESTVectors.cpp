#pragma comment( user, "Created by Alex Nickol Bourakov on 2009/02/12")
#include "stdafx.h"
#include <shlobj.h>

#include "./classesToplevel.h" 

#include "FrameMain.h"


extern "C" __declspec(dllexport)
void __stdcall EncriptString(char *str, char *encripted);
extern "C" __declspec(dllexport)
void __stdcall DecriptString(char *str, char *decripted);


XFrameMain *_g_main_frame; 


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { 

	char str[] = "abcd test string 1234567";
	char encripted[128];
	char decripted[128];

	EncriptString(str, encripted);
	DecriptString(encripted, decripted);

	srand((unsigned)time(NULL));

	char Buffer[512]; 

	std::vector<std::string> arguments = argumentsFromCommandline(); 
	if(arguments.size() > 0) { 
		SetCurrentDirectoryA(arguments[0].c_str()); 
	} 
	GetCurrentDirectory(512, Buffer); 

	PCHAR pszPath = (PCHAR)malloc(2048); 
	if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_DEFAULT, pszPath))) { 
		_g_program_files = pszPath; 
		_g_boot_drive = _g_program_files.substr(0, _g_program_files.find(':') + 2); // should be like C:\ 
	} 
	free(pszPath); 

	std::string config = ReadTextFile("XPrototype.txt"); 

	int wsiremoting_success = 0; 
	if(config.size()) { 
		PIPCObjInterface wsiremoting = IPCObjectGetInterface(); 
		if(wsiremoting) { 
			wsiremoting_success = wsiremoting->Initialize(config.c_str()); 
		} 
	} 

	if(wsiremoting_success) { 
		PIPCObjInterface wsiremoting = IPCObjectGetInterface(); 
		if(wsiremoting) { 
			wsiremoting->JoinNetwork(); 
		} 
	} 

	XFrameMain mywin(hInstance, LoadIcon((HINSTANCE)hInstance, MAKEINTRESOURCE(IDI_Q))); 
	mywin.Instantiate(); 


	_g_main_frame = &mywin; 

	if(_g_other_frame) { 
		mywin.MessageLoop(); 
	} 



//	Let it settle. 
	Sleep(500); 



	if(wsiremoting_success) { 
		PIPCObjInterface wsiremoting = IPCObjectGetInterface(); 
		if(wsiremoting) { 
			wsiremoting->LeaveNetwork(); 
		} 
	} 

	WaitforWorkItems(); 

	return 0; 
} 
