// Adopted from sources of Yuan, Feng 
// Created by Alex Bourakov. 

#include "stdafx.h"
#include "Canvas.h"

KCanvas::KCanvas() { 
	_messagehandler = 0; 
	_painthandler = 0; 
	_linedx = 0; 
	_linedy = 0; 
} 


void KCanvas::Destroy() { 
	if(_messagehandler) { 
		if(_delegates) { 
			_delegates->_onmessage_delegates.Remove(_messagehandler); 
		} 
		delete _messagehandler; 
	} 
	if(_painthandler) { 
		if(_delegates) { 
			_delegates->_ondraw_delegates.Remove(_painthandler); 
		} 
		delete _painthandler; 
	} 
	if(_delegates) { 
		delete _delegates; 
	} 
	_messagehandler = 0; 
	_painthandler = 0; 
	_delegates = 0; 
} 

KCanvas::~KCanvas() { 
	Destroy(); 
}

void KCanvas::Create(HINSTANCE hInst, HWND hParent) { 
	BOOL mdichild = (hInst == NULL); 

	if(hInst == NULL) { 
		hInst = GetModuleHandle(0); 
	} 

	_hinst = hInst; 

	if(_ctl.classname.size() == 0) { 
		if(_data_type.size()) { 
			_ctl.classname = _data_type; 
		} 
		else { 
			_ctl.classname = "KCanvas"; 
		} 
	} 

	if(!_messagehandler) { 
		_messagehandler = AllocXMemFunction(&KCanvas::OnMessage, this); 
	} 

	if(!_painthandler) { 
		_painthandler = AllocXMemFunction(&KCanvas::OnPaint, this); 
	} 

	CreateEx(0, (mdichild? WS_DISABLED: 0) | WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, hParent, hInst, mdichild); 

	if(_delegates) { 
		_delegates->_onmessage_delegates.Add(_messagehandler); 
		_delegates->_ondraw_delegates.Add(_painthandler); 
	} 
} 

bool KCanvas::CreateMDIChild(KCanvas *mdichild/*must not be initialized*/) { 
	if(std::find(_mdichilds.begin(), _mdichilds.end(), mdichild) != _mdichilds.end()) { 
		return true; 
	} 
	bool rc = false; 
	if(_ctl.classname == "MDICLIENT") { 
		if(mdichild) { 
			if(mdichild->_hwnd == 0) { 
				if(mdichild->_ctl.max_mdichilds <= 0) { 
					mdichild->_ctl.max_mdichilds = _ctl.max_mdichilds; 
				} 
				mdichild->Create(NULL, _hwnd); // zero HINSTANCE forces MDI child creation. 

				if(mdichild->_hwnd != NULL) { 
					_mdichilds.push_back(mdichild); 
					rc = true; 
				} 
			} 
		} 
	} 
	return rc; 
} 

bool KCanvas::RemoveMDIChild(HWND hwnd) { 
	bool rc = false; 
	std::vector<KCanvas*>::iterator it = _mdichilds.begin(); 
	while(rc == false && it < _mdichilds.end()) { 
		if((*it)->_hwnd == hwnd) { 
			delete (*it); 
			_mdichilds.erase(it); 
			rc = true; 
		} 
		else { 
			++it; 
		} 
	} 
	return rc; 
} 

void KCanvas::Resize(HWND hParent, int width, int height) { 
	KWindow::Resize(hParent, width, height); 

	std::vector<KCanvas*>::iterator it = _mdichilds.begin(); 
	while(it < _mdichilds.end()) { 
		if((*it)->_ctl.width) { 
			(*it)->Resize(_hwnd, _ctl.width, _ctl.height); 
		} 
		++it; 
	} 
} 

void KCanvas::Invalidate() { 
	KWindow::Invalidate(); 

	std::vector<KCanvas*>::iterator it = _mdichilds.begin(); 
	while(it < _mdichilds.end()) { 
		if((*it)->_ctl.width) { 
			(*it)->Invalidate(); 
		} 
		++it; 
	} 
} 

void KCanvas::OnPaint(SOnDrawParams& params) { 
} 

void KCanvas::OnMessage(SOnMessageParams& params) { 
	switch(params._umsg) { 
		case WM_SIZE: 
			if(!_is_sizing) { 
				RECT clrect; 
				GetClientRect(_hwnd, &clrect); 
				int iwidth = clrect.right - clrect.left; 
				int iheight = clrect.bottom - clrect.top; 

				int inewwidth = LOWORD(params._lparam); 
				int inewheight = HIWORD(params._lparam); 

				if(_ismdichildWindow) { 
				} 
				else 
				if(iwidth != inewwidth || iheight != inewheight) { // Resize request. Sent by application. 
					RECT winrect; 
					GetWindowRect(_hwnd, &winrect); 
					inewwidth += (winrect.right - winrect.left) - iwidth; 
					inewheight += (winrect.bottom - winrect.top) - iheight; 

					_ctl.SetRect(inewwidth, inewheight); 

					HWND hparent = GetParent(_hwnd); 
					RECT clrect; 
					if(GetClientRect(hparent, &clrect)) { 
						PostMessage(hparent, WM_SIZE, NULL, MAKELPARAM(clrect.right - clrect.left, clrect.bottom - clrect.top)); 
					} 
				} 
				else { 
					SCROLLINFO si; 
				    si.cbSize = sizeof(SCROLLINFO); 
				    si.fMask  = SIF_RANGE;
 
					if(GetScrollInfo(_hwnd, SB_HORZ, &si) && si.nMax) { 
						SetScrollBar(SB_HORZ, si.nMax); 
					} 
					if(GetScrollInfo(_hwnd, SB_VERT, &si) && si.nMax) { 
						SetScrollBar(SB_VERT, si.nMax); 
					} 
				} 
			} 
			break; 
		case WM_MOVE: 
			if(_ismdichildWindow) { 
				_ctl.SetRect(_hwnd); 
			} 
			break; 

		case WM_HSCROLL:
			OnScroll(SB_HORZ, LOWORD(params._wparam), HIWORD(params._wparam)); 
			break; 
		case WM_VSCROLL:
			OnScroll(SB_VERT, LOWORD(params._wparam), HIWORD(params._wparam)); 
			break; 
		case WM_KEYDOWN: 
			OnKeyDown(params._wparam, params._lparam); 
			params._processed = TRUE; 
			break; 
		case WM_DESTROY: 
			params._processed = TRUE; 
			Destroy(); 
			break; 
	} 
} 

void KCanvas::OnKeyDown(WPARAM wParam, LPARAM lParam) { 
	PostMessage(GetParent(_hwnd), WM_KEYDOWN, wParam, lParam); 
} 

void KCanvas::OnScroll(int nBar, int nScrollCode, int nPos) { 
	SCROLLINFO si;
	
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_PAGE;
 
	GetScrollInfo(_hwnd, nBar, &si); 
	
	int nPageSize = si.nPage;
	int nDis;

	switch(nScrollCode) { 
		case SB_LINEDOWN: // One line right/down. 
			nDis = (nBar==SB_VERT) ? _linedy : _linedx;
		break;
		case SB_LINEUP: // One line left/up. 
			nDis = (nBar==SB_VERT) ? -_linedy : -_linedx;
		break;
		case SB_PAGEDOWN: // One page right/down.
			nDis = nPageSize;
		break;
		case SB_PAGEUP: // One page left/up.
			nDis = -nPageSize;
		break;
		case SB_THUMBPOSITION: // Absolute position.
			nDis = nPos - GetScrollPos(_hwnd, nBar);
		break;
		default: // No change.
			nDis = 0;
		break;
    }

	if(nDis) { 
		int nMin, nMax;

		GetScrollRange(_hwnd, nBar, &nMin, &nMax); 

		nPos = GetScrollPos(_hwnd, nBar) + nDis; 
		nPos = std::max(nMin, nPos); 
		nPos = std::min(nMax - nPageSize, nPos); 

		nDis = nPos - GetScrollPos(_hwnd, nBar); 
		if(nDis) { 
			SetScrollPos(_hwnd, nBar, nPos, TRUE);
			if(nBar == SB_HORZ) { 
				ScrollWindow(_hwnd, -nDis, 0, NULL, NULL); 
			} 
			else { 
				ScrollWindow(_hwnd, 0, -nDis, NULL, NULL); 
			} 
        }
	}
}

BOOL KCanvas::SetScrollBar(int side, int canvassize) { 
	RECT rect; 
	GetClientRect(_hwnd, &rect); 

	int pagesize = canvassize; 
	switch(side) { 
		case SB_HORZ: 
			pagesize = rect.right; 
		break; 
		case SB_VERT: 
			pagesize = rect.bottom; 
		break; 
	} 

	BOOL rc = FALSE; // Hide scrollbar.

	if(pagesize < canvassize) { 
		SCROLLINFO si;

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask  = SIF_RANGE | SIF_PAGE;
		si.nMin   = 0;
		si.nMax   = canvassize/* - 1*/;
		si.nPage  = pagesize;

		SetScrollInfo(_hwnd, side, &si, TRUE);  // Can generate WM_SIZE. 
		EnableScrollBar(_hwnd, side, ESB_ENABLE_BOTH); 

		rc = TRUE; // Show scrollbar.
	} 
	else { 
		SCROLLINFO si;

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask  = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
		si.nMin   = 0;
		si.nMax   = 0;
		si.nPage  = 0;

		SetScrollInfo(_hwnd, side, &si, TRUE); // Can generate WM_SIZE. 
	} 

	return rc; 
} 

void KCanvas::SetScrollDim(int width, int height, int linedx, int linedy) { 
	_linedx = linedx; 
	_linedy = linedy; 

	ShowScrollBar(_hwnd, SB_HORZ, SetScrollBar(SB_HORZ, width)); 
	ShowScrollBar(_hwnd, SB_VERT, SetScrollBar(SB_VERT, height)); 
} 