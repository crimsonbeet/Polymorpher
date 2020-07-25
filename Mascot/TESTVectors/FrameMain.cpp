
#include "stdafx.h"
#include "./classesToplevel.h"


#include "FrameMain.h"


const std::string _g_arrow_images[2] = { 
	IMG_ARROW_LEFT, 
	IMG_ARROW_RIGHT 
}; 

const std::string _g_arrow_images_disabled[2] = { 
	IMG_ARROW_LEFT_D, 
	IMG_ARROW_RIGHT_D 
}; 

const std::string _g_arrow_images_hot[2] = { 
	IMG_ARROW_LEFT_H, 
	IMG_ARROW_RIGHT_H 
}; 


KFrameHistory *_g_frame_history; 
XFrameOther *_g_other_frame; 



XFrameMain::XFrameMain(HINSTANCE hInstance, HICON hIcon): KFrame(hInstance, hIcon) { 
} 

void XFrameMain::Instantiate() {
	Configure(ReadTextFile("TESTVectorsFrameMain.txt")); 
	Create(_hinst, NULL); 

	int nstep = 1; 
	while(nstep) switch(nstep) { 
		case 1: // check if rebar can be created. 
			nstep = _veccanvas.size()? 2: 0/*end*/; 
			break; 
		case 2: // create rebar. 
		{ 
			KCanvas& canvas = _veccanvas[0]; 

			SCreateRebar cr_rebar; 
			cr_rebar._idrebar = 102; 
			cr_rebar._hwndparent = canvas._hwnd; 
//			cr_rebar._piml = &_iml; 

			nstep = KWinCreateRebar(cr_rebar, &_prebar)? 3: 0; 
			if(!nstep) { 
				std::cout << "XFrameMain->can not create rebar\r\n"; 
			} 
		} 
		break; 
		case 3: // create back-forth toolbar. 
		{ 
			nstep = ActivateToolBar(*(_prebar.get()), 103, _g_arrow_images, _g_arrow_images_hot, _g_arrow_images_disabled, ARRAY_NUM_ELEMENTS(_g_arrow_images))? 4: 0; 
		} 
		break; 
		case 4: // create frame-history based on rebar. 
		{ 
			_g_frame_history = new KFrameHistory(_prebar, _toolbar); // it takes ownership of _prebar. 
			nstep = 5; 
		} 
		break; 
		case 5: // create other frame. 
		if(_veccanvas.size() > 1) { 
			_g_other_frame = new XFrameOther(_hinst, _veccanvas[1]._hwnd); 
			_g_other_frame->SetTopMost(); 

			_g_frame_history->InsertHistory(_g_other_frame); 

			IPCSetLogHandler(_g_other_frame->_hwnd); 
		} 
		nstep = 0; 
		break; 
	} 

	_delegates->_onmessage_delegates.Add(&XFrameMain::OnMessage, this); 
	_delegates->_ondraw_delegates.Add(&XFrameMain::OnPaint, this); 

	ShowWindow(); 
} 

void XFrameMain::ProcessToolBarCommand(SReBar& rebar) { 
	if(rebar._lparam == (LPARAM)_toolbar->_hwndtoolbar) { 
		switch(rebar._wparam) { 
			case 103/*_toolbar->_idfirstbutton*/: 
				_g_frame_history->StepBackHistory(); 
			break; 
			case 104/*_toolbar->_idfirstbutton + 1*/: 
				_g_frame_history->StepForthHistory(); 
			break; 
		} 
	} 
} 

void XFrameMain::OnMessage(SOnMessageParams& params) { 
//	static bool is_sizing = false; 
	switch(params._umsg) { 
		case WM_CLOSE: 
		case WM_DESTROY: 
			IPCSetLogHandler(0); 
		break; 
		case WM_SIZE: 
		if(IsIconic(_hwnd)) { 
		} 
		else 
		if(IsSizing()) { 
		} 
		else { 
			RECT clrect; 
			if(_veccanvas.size() > 0) { 
				GetClientRect(_veccanvas[0]._hwnd, &clrect); 
				RECT winrect; 
				GetWindowRect(_g_frame_history->GetRebarPtr()->_hwndrebar, &winrect); 
				LONG rebar_height = winrect.bottom - winrect.top; 
				if(rebar_height < clrect.bottom) { 
					clrect.bottom = rebar_height; // Do not change the height of rebar. 
				} 
				MoveWindow(_g_frame_history->GetRebarPtr()->_hwndrebar, 0, 0, clrect.right, clrect.bottom, TRUE); 
			} 
			if(_veccanvas.size() > 1) { 
				GetClientRect(_veccanvas[1]._hwnd, &clrect); 
				MoveWindow(_g_other_frame->_hwnd, 0, 0, clrect.right, clrect.bottom, TRUE); 
			} 
		} 
		break; 
	} 
} 

void XFrameMain::OnPaint(SOnDrawParams& params) { 
	_g_frame_history->ShowCurrent(); 
	_delegates->_ondraw_delegates.Remove(_painthandler); 
//	delete _painthandler; 
	_painthandler = 0; 
} 

