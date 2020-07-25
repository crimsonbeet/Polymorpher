// Mascot.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "./classesToplevel.h"

#include "FrameMain.h"


DWORD WINAPI dosmain_inbatch(LPVOID); 
DWORD WINAPI test_preload(LPVOID);

int _tmain(int argc, _TCHAR* argv[]) {

	XFrameMain mywin((HINSTANCE)GetModuleHandle(0)); 
	mywin.Instantiate(); 


	_g_main_frame = &mywin; 

	if(_g_drawing_frame) { 
		SCommandLineArguments args; 
		args.N = argc; 
		memcpy(args.A, argv, sizeof(char*) * argc); 

		//QueueWorkItem(dosmain_inbatch, (LPVOID)&args);
		QueueWorkItem(test_preload);

		mywin.MessageLoop(); 
	} 

//	Let it settle. 
	Sleep(500); 

	_g_drawing_frame = 0; 

	WaitforWorkItems(); 
	return 0;
}

