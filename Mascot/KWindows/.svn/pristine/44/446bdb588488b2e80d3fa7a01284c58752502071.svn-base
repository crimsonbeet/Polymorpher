// Adopted from sources of Yuan, Feng 
// Created by Alex Bourakov. 

#include "stdafx.h"
#include "FrameWnd.h"


#include "IPCInterface.h" 
#include "WSIParser.h" 



KFrame::KFrame(HINSTANCE hInstance, HICON hIcon) { 
	_hinst	= hInstance; 
	_hicon = hIcon; 

	_pCanvas = NULL; 
	_pStatuswin = NULL; 
	_pLogwin = NULL; 
	_pHyperlist = NULL; 

	_width = 0; 
	_height = 0; 

	_haccel = NULL; 

	_messagehandler = NULL; 

	_toolbar = NULL; 
	_toolbar_delegates = FALSE; 
}


KFrame::~KFrame() {
}

void KFrame::Configure(const std::string& config) { 
	XConfigure(*this, config); 
} 



void KFrame::CreateKWindow(KWindow& item) { 
	RECT rect; 
	GetClientRect(_hwnd, &rect); 
	int bottom = rect.bottom; 
	if(_pStatuswin) { 
		RECT statrect; 
		GetWindowRect(_pStatuswin->_hwnd, &statrect); 
		bottom = statrect.top; 
	} 
	if(_pLogwin) { 
		RECT logrect; 
		GetWindowRect(_pLogwin->_hwnd, &logrect); 
		bottom = logrect.top; 
	} 

	if(item._ctl.bottom == 0) { 
		item._ctl.bottom = (int)((double)bottom / (double)rect.bottom * 1000 + 0.5); 
	} 
	if(item._ctl.right == 0) { 
		item._ctl.right = 1000; 
	} 

	item.Create(_hinst, _hwnd); 
} 

bool KFrame::CreateMDIChild(KCanvas *pCanvas) { 
	if(!pCanvas) { 
		return false; 
	} 
	bool ok = false; 
	std::vector<KCanvas>::iterator it = _veccanvas.begin(); 
	while(!ok && it != _veccanvas.end()) { 
		if((*it)._data_type == pCanvas->_data_type) { 
			ok = (*it).CreateMDIChild(pCanvas); 
		} 
		++it; 
	} 

	if(!ok) { 
		delete pCanvas; 
	} 

	return ok == true; 
} 


void KFrame::ResizeKWindow(KWindow& kwin) { 
	kwin.Resize(_hwnd, _width, _height); 
} 

struct SCollectKWinctl { 
	std::vector<KWinctl*>& _locations; 

	SCollectKWinctl(std::vector<KWinctl*>& locations): _locations(locations) { 
	} 

	void operator()(KWindow& item) { 
		_locations.push_back(&item._ctl); 
	} 
}; 

struct SAlignKWinctl { 
	std::vector<KWinctl*>& _locations; 

	SAlignKWinctl(std::vector<KWinctl*>& locations): _locations(locations) { 
	} 

	void operator()(KWindow& item) { 
		KWinctl& ctl = item._ctl; 
		std::vector<KWinctl*>::iterator iter = _locations.begin(); 
		while(iter != _locations.end()) { 
			if((*iter)->y > ctl.y && (*iter)->y < ctl.bottom) { 
				(*iter)->y = ctl.bottom; 
			} 
			++iter; 
		} 
	} 
}; 

void KFrame::AlignChildRectangles() { 
	std::vector<KWinctl*> locations; 

	for_each(_veccanvas.begin(), _veccanvas.end(), SCollectKWinctl(locations)); 
	for_each(_vechyperlist.begin(), _vechyperlist.end(), SCollectKWinctl(locations)); 

	for_each(_veccanvas.begin(), _veccanvas.end(), SAlignKWinctl(locations)); 
	for_each(_vechyperlist.begin(), _vechyperlist.end(), SAlignKWinctl(locations)); 
} 


void KFrame::Create(HINSTANCE hInst, HWND hParent) { 
	_hinst = hInst; 

	if(_hwnd) { 
		return; 
	} 

	if(_ctl.classname.size() == 0) { 
		if(_data_type.size()) { 
			_ctl.classname = _data_type; 
		} 
		else { 
			_ctl.classname = "KFrame"; 
		} 
	} 

	if(!_messagehandler) { 
		_messagehandler = AllocXMemFunction(&KFrame::OnMessage, this); 
	} 

	if(hParent == NULL) { 
		CreateEx(WS_EX_WINDOWEDGE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, NULL, _hinst); 
	} 
	else { 
		CreateEx(NULL, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS/* | WS_THICKFRAME*//* | WS_BORDER*/, hParent, _hinst); 
	} 

	if(_delegates) { 
		_delegates->_onmessage_delegates.Add(_messagehandler); 
	} 

	// status window is at the bottom of client area
	if(_vecstatuswin.size()) { 
		_pStatuswin = &_vecstatuswin[0]; 
	} 
	if(_pStatuswin) { 
		_pStatuswin->Create(_hinst, _hwnd); 
	}
	
	if(_veclogwin.size()) { 
		_pLogwin = &_veclogwin[0]; 
	} 
	if(_pLogwin) { 
		_pLogwin->Create(_hinst, _hwnd); 
	} 

	if(_veccanvas.size()) { 
		for_each(_veccanvas.begin(), _veccanvas.end(), XMemFunction(&KFrame::CreateKWindow, this)); 
		_pCanvas = &_veccanvas[0]; 
	} 

	if(_vechyperlist.size()) { 
		for_each(_vechyperlist.begin(), _vechyperlist.end(), XMemFunction(&KFrame::CreateKWindow, this)); 
		_pHyperlist = &_vechyperlist[0]; 
	} 
} 


BOOL KFrame::OnSize(int width, int height) { 
	_width = width; 
	_height = height; 

	AlignChildRectangles(); 
	
	if(_pStatuswin) { 
		_pStatuswin->Resize(_hwnd, width, height); 
	} 
	
	if(_pLogwin) { 
		_pLogwin->Resize(_hwnd, width, height); 
	} 

	if(_vechyperlist.size()) { 
		for_each(_vechyperlist.begin(), _vechyperlist.end(), XMemFunction(&KFrame::ResizeKWindow, this)); 
	} 

	if(_veccanvas.size()) { 
		for_each(_veccanvas.begin(), _veccanvas.end(), XMemFunction(&KFrame::ResizeKWindow, this)); 
	} 

	return TRUE;
}

BOOL KFrame::OnCopyData(LPARAM lparam) { 
	if(_pLogwin) { 
		PCOPYDATASTRUCT copydata = (PCOPYDATASTRUCT)lparam; 
		std::istringstream istring((PCHAR)(copydata->lpData)); 
		std::string message; 
		std::getline(istring, message); 
		if(message.size()) { 
			struct _timeb t; 
			_ftime(&t); 
			std::istringstream istrTime(ctime(&t.time)); 
			std::string sWeekDay, sMonth;
			int iDay, iHour, iMinutes, iSeconds, iYear; 
			char cSColmn; 
			istrTime >> sWeekDay >> sMonth >> iDay >> iHour >> cSColmn >> iMinutes >> cSColmn >> iSeconds >> iYear; 
			std::ostringstream ostr; 
			ostr.fill('0'); 
			ostr << sMonth << ' ' << std::setw(2) << iDay << ' ' << std::setw(2) << iHour << ':' << std::setw(2) << iMinutes << ':' << std::setw(2) << iSeconds << '_' << std::setw(3) << (int)t.millitm << ' '; 
			_pLogwin->Log("%s\t", ostr.str().c_str()); 

			while(message.size()) { 
				_pLogwin->Log("%s\r\n", message.c_str()); 
				message.resize(0); 
				std::getline(istring, message); 
			} 
		} 
	} 
	return TRUE;
} 

BOOL KFrame::OnUserMessage(WPARAM wparam, LPARAM lparam) { 
	BOOL ok = FALSE; 
	switch(wparam) { 
		case WM_NCDESTROY: // destroy MDI child. 
			for(size_t x = 0; !ok && x < _veccanvas.size(); ++x) { 
				if(_veccanvas[x].RemoveMDIChild((HWND)lparam)) { 
					ok = TRUE; 
				} 
			}
			break; 
		case WM_MDIACTIVATE: // MDI child got activated. 
			SendMessage((HWND)lparam, WM_NCACTIVATE, TRUE, (LPARAM)NULL); 
			ok = TRUE; 
			break; 
	} 
	return ok; 
} 


KHyperlist * KFrame::FindHListObject(const std::string& data_type) { 
	KHyperlist *obj = 0; 
	std::vector<KHyperlist>::iterator iter = _vechyperlist.begin(); 
	while(obj == 0 && iter != _vechyperlist.end()) { 
		if(std::find((*iter)._data_type.begin(), (*iter)._data_type.end(), data_type) != (*iter)._data_type.end()) { 
			obj = &(*iter); 
		} 
		//if((*iter)._data_type == data_type) { 
		//	obj = &(*iter); 
		//} 
		++iter; 
	} 

	return obj; 
} 

KCanvas * KFrame::FindCanvasObject(const std::string& data_type) { 
	KCanvas *obj = 0; 
	std::vector<KCanvas>::iterator iter = _veccanvas.begin(); 
	while(obj == 0 && iter != _veccanvas.end()) { 
		if((*iter)._data_type == data_type) { 
			obj = &(*iter); 
		} 
		++iter; 
	} 

	return obj; 
} 

BOOL KFrame::ActivateToolBar(SReBar& rebar, const int toolbar_id, const std::string img[], const std::string img_h[], const std::string img_d[], const int img_cnt) { 
	BOOL rc = FALSE; 

	int nstep = 1; 
	while(nstep) switch(nstep) { 
		case 1: 
			if(!_toolbar) { 
				_toolbar = new SToolBar(); 
			} 
			nstep = 2; 
			break; 
		case 2: 
			if(!_toolbar->_hwndtoolbar) { 
				SCreateToolBar crtoolbar; 
				crtoolbar._siml.insert(crtoolbar._siml.begin(), img, img + img_cnt); 
				if(img_h) { 
					crtoolbar._simlhot.insert(crtoolbar._simlhot.begin(), img_h, img_h + img_cnt); 
				} 
				if(img_d) { 
					crtoolbar._simldisabled.insert(crtoolbar._simldisabled.begin(), img_d, img_d + img_cnt); 
				} 

				crtoolbar._hwndparent = rebar._hwndrebar; 
				crtoolbar._idtoolbar = toolbar_id; 
				crtoolbar._idfirstbutton = toolbar_id; 

				if(!KWinCreateToolBar(crtoolbar, *_toolbar)) { 
					std::cout << "KFrame->can not create toolbar\r\n"; 
				} 
			} 
			nstep = _toolbar->_hwndtoolbar? 3: 0; 
			break; 
		case 3: 
			if(!_toolbar_delegates) { 
				rebar._wmcommand_delegates.Add(&KFrame::ProcessToolBarCommand, this); 
				rebar._wmnotify_delegates.Add(&KFrame::ProcessToolBarNotify, this); 
			} 
			_toolbar_delegates = TRUE; 

			nstep = _toolbar_delegates? 4: 0; 
			break; 
		case 4: 
			if(_toolbar->_rebarbandid == -1) { 
				SCreateRebarBand crband; 
				crband._hwndrebar = rebar._hwndrebar; 
				crband._hwndchild = _toolbar->_hwndtoolbar; 
				crband._fstyle = rebar._fbandstyle; 

				crband._childminwidth = _toolbar->_buttonwidth * _toolbar->_nbuttons; 
				crband._childminheight = _toolbar->_buttonheight; 
				crband._cxideal = _toolbar->_buttonwidth * _toolbar->_nbuttons; 

				if(!KWinCreateRebarBand(crband)) { 
					std::cout << "KFrame->can not create toolbar rebarband\r\n"; 
				} 
				else { 
					_toolbar->_rebarbandid = crband._rebarbandid; 
				} 
			} 
			rc = TRUE; 
			nstep = 0; 
			break; 
	} 

	return rc; 
} 

void KFrame::ProcessToolBarCommand(SReBar& rebar) { 
	if(rebar._lparam == (LPARAM)_toolbar->_hwndtoolbar) { 
//		switch(rebar._wparam) { 
//		} 
	} 
} 

void KFrame::ProcessToolBarNotify(SReBar& rebar) { 
	if(rebar._lparam) { 
		if(((NMHDR*)(rebar._lparam))->hwndFrom == _toolbar->_hwndtoolbar) { 
//			switch((NMHDR*)(rebar._lparam))->code) { 
//			} 
		} 
	} 
} 

void KFrame::OnChangeFrameState(SReBar& rebar, int state) { 
	switch(state) { 
		case -1: /*out of scope*/
			if(_toolbar_delegates) { 
				rebar._wmcommand_delegates.Remove(&KFrame::ProcessToolBarCommand, this); 
				rebar._wmnotify_delegates.Remove(&KFrame::ProcessToolBarNotify, this); 
			} 
			_toolbar_delegates = FALSE; 

			if(_toolbar && _toolbar->_rebarbandid > -1) { 
				SendMessage(rebar._hwndrebar, RB_DELETEBAND, _toolbar->_rebarbandid, 0); 
				_toolbar->_rebarbandid = -1; 
			} 
			if(_toolbar && _toolbar->_hwndtoolbar) { 
				DestroyWindow(_toolbar->_hwndtoolbar); 
				_toolbar->_hwndtoolbar = 0; 
			} 
			if(_toolbar) { 
				delete _toolbar; 
				_toolbar = 0; 
			} 
			break; 
		case 0: /*inactive*/
			if(_toolbar && _toolbar->_rebarbandid > -1) { 
				SendMessage(rebar._hwndrebar, RB_SHOWBAND, _toolbar->_rebarbandid, 0/*hide*/); 
			} 
			break; 
		case 1: /*active*/
			if(_toolbar && _toolbar->_rebarbandid > -1) { 
				SendMessage(rebar._hwndrebar, RB_SHOWBAND, _toolbar->_rebarbandid, 1/*show*/); 
			} 
			break; 
	} 
} 

BOOL KFrame::ShowWindow(int nCmdShow) { 
	BOOL rc = KWindow::ShowWindow(nCmdShow); 
	if(nCmdShow == SW_SHOW) { 
		if(_pHyperlist) { 
			_pHyperlist->Invalidate(); 
		} 
		if(_pLogwin) { 
			_pLogwin->Invalidate(); 
		} 
	} 

	return rc; 
} 

void KFrame::SetTopMost(HWND hWndInsertAfter) { 
	KWindow::SetTopMost(hWndInsertAfter); 
	if(_pHyperlist) { 
		_pHyperlist->SetTopMost(); 
	} 
	if(_pCanvas) { 
		_pCanvas->SetTopMost(); 
	} 
	if(_pLogwin) { 
		_pLogwin->SetTopMost(); 
	} 
	SetFocus(_hwnd); 
} 

void KFrame::SetTopMostTreeObject(KHyperlist& obj) { 
	_pHyperlist = &obj; 
	_pHyperlist->SetTopMost(); 

	SetFocus(_pHyperlist->_hwnd); 
} 

void KFrame::SetTopMostCanvasObject(KCanvas& obj) { 
	_pCanvas = &obj; 
	_pCanvas->SetTopMost(); 
} 


void KFrame::OnMessage(SOnMessageParams& params) { 
	switch(params._umsg) { 
		case WM_SIZE: 
			if(!_is_sizing) { 
				params._processed = OnSize(LOWORD(params._lparam), HIWORD(params._lparam)); 
			} 
			break; 
		case WM_DESTROY: 
			params._processed = TRUE; 
			_pCanvas = NULL; 
			_pStatuswin = NULL; 
			_pLogwin = NULL; 
			if(_messagehandler) { 
				_delegates->_onmessage_delegates.Remove(_messagehandler); 
				delete _messagehandler; 
				_messagehandler = NULL; 
			} 
			break; 

		case WM_KEYDOWN: 
			params._processed = TRUE; 
			break; 

		case WM_SETCURSOR: 
//			params._processed = TRUE; 
//			params._lresult = TRUE; 
			break; 

		case WM_USER: 
			params._lresult = OnUserMessage(params._wparam, params._lparam); 
			params._processed = TRUE; 
			break; 

		case WM_COPYDATA: 
			params._processed = OnCopyData(params._lparam); 
			if(params._processed) { 
				params._lresult = TRUE; 
			} 
			break; 
	} 
} 

WPARAM KFrame::MessageLoop(void) { 
	MSG msg;
	_ismainWindow = TRUE;
	while(GetMessage(&msg, NULL, 0, 0)) { 
		if(!TranslateAccelerator(_hwnd, _haccel, &msg)) { 
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
	} 

	return msg.wParam;
}




KFrameHistory::KFrameHistory(std::auto_ptr<SReBar>& prebar, SToolBar *toolbar): _prebar(prebar), _toolbar(toolbar) { 
	_frameshistory_pos = -1; 
} 

const PSReBar KFrameHistory::GetRebarPtr() { 
	return _prebar.get(); 
} 

std::auto_ptr<SReBar>& KFrameHistory::TakeRebar() { 
	return _prebar; 
} 

void KFrameHistory::InsertHistory(KFrame *pframe) { 
	if((size_t)_frameshistory_pos < (_frameshistory.size() - 1)) { 
		_frameshistory.erase(_frameshistory.begin() + _frameshistory_pos + 1, _frameshistory.end()); 
	} 
	if(_frameshistory_pos > -1) { 
		_frameshistory[_frameshistory_pos]->OnChangeFrameState(*(_prebar.get()), 0/*inactive*/); 
	} 
	_frameshistory.push_back(pframe); 
	_frameshistory_pos = _frameshistory.size() - 1; 

	pframe->OnChangeFrameState(*(_prebar.get()), 1/*active*/); 

	SynchronizeToolBar(); 
} 

BOOL KFrameHistory::StepBackHistory() { 
	BOOL rc = FALSE; 
	if(_frameshistory_pos > 0) { 
		_frameshistory[_frameshistory_pos]->OnChangeFrameState(*(_prebar.get()), 0/*inactive*/); 

		KFrame *pframe = _frameshistory[--_frameshistory_pos]; 
		pframe->SetTopMost(); 
		pframe->OnChangeFrameState(*(_prebar.get()), 1/*active*/); 
		rc = TRUE; 
	} 
	SynchronizeToolBar(); 
	return rc; 
} 

BOOL KFrameHistory::StepForthHistory() { 
	BOOL rc = FALSE; 
	if((size_t)_frameshistory_pos < (_frameshistory.size() - 1)) { 
		_frameshistory[_frameshistory_pos]->OnChangeFrameState(*(_prebar.get()), 0/*inactive*/); 

		KFrame *pframe = _frameshistory[++_frameshistory_pos]; 
		pframe->SetTopMost(); 
		pframe->OnChangeFrameState(*(_prebar.get()), 1/*active*/); 
		rc = TRUE; 
	} 
	SynchronizeToolBar(); 
	return rc; 
} 

void KFrameHistory::SynchronizeToolBar() { 
	bool btenable[2]; 
	btenable[0] = _frameshistory_pos > 0; 
	btenable[1] = (size_t)_frameshistory_pos < (_frameshistory.size() - 1); 

	for(int x = 0; x < 2; ++x) { 
		if(_toolbar) { 
			_toolbar->SetButtonStateByindex(TBSTATE_ENABLED, x, !btenable[x]); 
		} 
	} 
} 

BOOL KFrameHistory::ShowCurrent() { 
	BOOL rc = FALSE; 
	if((size_t)_frameshistory_pos < _frameshistory.size()) { 
		_frameshistory[_frameshistory_pos]->ShowWindow(); 
		rc = TRUE; 
	} 
	return rc; 
} 


