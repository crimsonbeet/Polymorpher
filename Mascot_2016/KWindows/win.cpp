// Adopted from sources of Yuan, Feng 
// Created by Alex Bourakov. 

#include "stdafx.h"
#include "win.h"
#include "Winuser.h"

size_t _g_horizontalResolution = 1650; 

KWinctl::KWinctl() { 
	x = 0; 
	y = 0; 
	bottom = 0; 
	right = 0; 

	transparent = 0; 

	width = 0; 
	height = 0; 

	id = 0; 

	max_mdichilds = 0; 

	font_height = 0; 
	font_bold = 0; 
	font_italic = 0; 
	font_charset = ANSI_CHARSET; 

	backcolor = (DWORD)GetSysColor(COLOR_APPWORKSPACE); 

	hbrBackground = NULL; 
} 

RECT KWinctl::GetRect(int parent_width, int parent_height) { 
	RECT r; 
	if(parent_width == CW_USEDEFAULT) { 
		r.right = r.left = CW_USEDEFAULT; 
		width = 0; 
	} 
	else { 
		double sfx = ((double)parent_width) / 1000; 
		r.left = (int)floor((double)x * sfx); 
		r.right = (int)ceil((double)right * sfx); 
		width = r.right - r.left; 
	} 
	if(parent_height == CW_USEDEFAULT) { 
		r.bottom = r.top = CW_USEDEFAULT; 
		height = 0; 
	} 
	else { 
		double sfy = ((double)parent_height) / 1000; 
		r.top = (int)floor((double)y * sfy); 
		r.bottom = (int)ceil((double)bottom * sfy); 
		height = r.bottom - r.top; 
	} 

	return r; 
} 

VOID KWinctl::SetRect(int newwidth, int newheight) { 
	if(width) { 
		right = (int)ceil(right * ((double)newwidth / (double)width)); 
		if(right < x) { 
			right = x; 
		} 
		width = newwidth; 
	} 
	if(height) { 
		bottom = (int)ceil(bottom * ((double)newheight / (double)height)); 
		if(bottom < y) { 
			bottom = y; 
		} 
		height = newheight; 
	} 
} 

VOID KWinctl::SetRect(HWND hwnd) { 
	RECT rect; 
	HWND hparent = GetParent(hwnd); 
	double xscale = 0; 
	double yscale = 0; 
	BOOL ok = FALSE; 
	if(hparent) { 
		if(GetClientRect(hparent, &rect)) { 
			xscale = 1000.0 / (double)rect.right; 
			yscale = 1000.0 / (double)rect.bottom; 
			ok = TRUE; 
		} 
	} 
	if(!ok) { 
		return; 
	} 

	if(GetWindowRect(hwnd, &rect)) { 
		POINT pt = { rect.left, rect.top }; 
		ScreenToClient(hparent, &pt); 

		x = (int)floor(pt.x * xscale); 
		y = (int)floor(pt.y * yscale); 

		width = rect.right - rect.left; 
		height = rect.bottom - rect.top; 

		right = x + (int)ceil(width * xscale); 
		bottom = y + (int)ceil(height * yscale); 
	} 
} 



KBitmapctl::KBitmapctl() { 
	id = 0; 

	bitmap_rows = 1; 
	bitmap_columns = 1; 
} 

KWindow::KWindow() { 
	_hwnd = 0;
	_hpalette = 0; 
	_hicon = 0; 
	_updateCount = 0;
	_ismainWindow = FALSE; 
	_ismdichildWindow = FALSE; 
	_is_sizing = FALSE; 
	_destroyed = FALSE; 
	_delegates = 0; 
} 

BOOL KWindow::_is_sizing = FALSE; 

BOOL KWindow::IsSizing() { 
	return _is_sizing; 
} 

KWindow::~KWindow() { 
	if(_hpalette) { 
		DeleteObject(_hpalette); 
		_hpalette = NULL; 
	} 
} 
    
// Default message handler for main program window, dispatch to OnKeyDown, OnDraw, etc.
LRESULT KWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
	switch(uMsg) {
		case WM_CREATE: 
			_hwnd = hWnd; 
			if(!_delegates) { 
				_delegates = new SKWindowDelegates(); 
			} 
			break; 

		case WM_PAINT: 
			if(_is_sizing) { 
				return 0;  
			} 
			else 
			if(_delegates && _delegates->_ondraw_delegates.Size() > 0) { 
				PAINTSTRUCT ps; 
				HDC hdc = BeginPaint(_hwnd, &ps); 
	
				SOnDrawParams params; 
				params._pwin = this; 
				params._hdc = hdc; 
				params._rcpaint = ps.rcPaint; 

				_delegates->_ondraw_delegates(params); 

				EndPaint(_hwnd, &ps); 
				return 0; 
			} 
			break; 

		case WM_PALETTEISCHANGING: // should not happen 
			return 0; 

		case WM_PALETTECHANGED: 
			return OnPaletteChanged(hWnd, wParam); 

		case WM_QUERYNEWPALETTE: 
			return OnQueryNewPalette(); 

		case WM_ENTERSIZEMOVE: 
			if(_ismainWindow) { // Capture current image of the window; stop painting until WM_EXITSIZEMOVE.
				_is_sizing = TRUE; 
				_imgcapturer.Create(GetModuleHandle(0), _hwnd); // repaints same image until WM_EXITSIZEMOVE. 
			} 
			break; 
		case WM_EXITSIZEMOVE: 
			if(_ismainWindow) { 
				_imgcapturer.Destroy(); 
				_is_sizing = FALSE; 

				RECT clrect; 
				GetClientRect(_hwnd, &clrect); 
				PostMessage(_hwnd, WM_SIZE, 0, MAKELPARAM(clrect.right - clrect.left, clrect.bottom - clrect.top)); 
			} 
			break; 

		case WM_ERASEBKGND: 
			if(_is_sizing) { // The following code paints only the area outside of original image. 
				HRGN sysrgn = CreateRectRgn(0, 0, 1, 1); 
				if(sysrgn) { 
					if(GetRandomRgn((HDC)wParam, sysrgn, SYSRGN)) { // client system region is in screen coord. 
						POINT actual_origin = { 0, 0 }; 
						ClientToScreen(_hwnd, &actual_origin); 
						OffsetRgn(sysrgn, -actual_origin.x, -actual_origin.y); // convert to logical units. 

						HRGN cliprgn = _imgcapturer.CreateWinRegion(); 
						if(cliprgn) { 
							CombineRgn(sysrgn, sysrgn, cliprgn, RGN_DIFF); // remove image area from sys.region. 
							DeleteObject(cliprgn); 

							PaintRgn((HDC)wParam, sysrgn); // use brush currently selected into dc. 
						} 
					} 
					DeleteObject(sysrgn); 
				} 
				return 1; 
			} 
//			WM_ERASEBKGND sinks into default handler. 

		default: { 
			SOnMessageParams params; 
			params._pwin = this; 
			params._umsg = uMsg; 
			params._wparam = wParam; 
			params._lparam = lParam; 
			if(_delegates) { 
				_delegates->_onmessage_delegates(params); 
			} 

			if(uMsg == WM_DESTROY) { 
				if(_delegates) { 
					delete _delegates; 
				} 
				_delegates = 0; 
				_destroyed = 1; 
				if(_ismainWindow) { 
					PostQuitMessage(0); // main window only 
				} 

				return 0; 
			} 

			if(params._processed) { 
				return params._lresult; 
			} 
		} 
	} 

	if(this->_ismdichildWindow) { 
		return CommonMDIChildProc(hWnd, uMsg, wParam, lParam); 
	} 
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
} 


// Generic window procedure passed to WIN32 API, dispatches to KWindow::WndProc

LRESULT CALLBACK KWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
	KWindow *pWindow = 0;
        
	if(uMsg == WM_NCCREATE) { 
		MDICREATESTRUCT *pMDIC = (MDICREATESTRUCT*)((LPCREATESTRUCT)lParam)->lpCreateParams; 
		pWindow = (KWindow*)(pMDIC->lParam); 
		if(pWindow) { 
			SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pWindow); 
			if(pWindow->_ismdichildWindow) { 
				SetWindowLong(hWnd, GWL_STYLE, pMDIC->style | WS_CAPTION/* | WS_THICKFRAME*/ | WS_BORDER | WS_SYSMENU); 
			} 
		} 
	}
	else { 
		pWindow = (KWindow *)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);
	} 

	LRESULT lr = 0; 

	if(pWindow) { 
		lr = pWindow->WndProc(hWnd, uMsg, wParam, lParam); 
	} 
	else { 
		lr = DefWindowProc(hWnd, uMsg, wParam, lParam); 
	} 

	return lr; 
}


// Register WNDCLASS for the window class. Registering only once
bool KWindow::RegisterClass(const std::string& classname, HINSTANCE hInst) { 
	WNDCLASSEXA wc;
	wc.cbSize = sizeof(wc);
	// Check if class is already registered
	bool ok = true; 
	if(!GetClassInfoExA(hInst, classname.c_str(), &wc)) { 
		GetWndClassEx(wc); // fill class attributes. 

		wc.hInstance = hInst;
		wc.lpszClassName = classname.c_str(); 

		if((DWORD)_ctl.backcolor) { 
			wc.hbrBackground = _ctl.hbrBackground = CreateSolidBrush((DWORD)_ctl.backcolor); 
		} 

		if(!RegisterClassExA(&wc)) { 
			ok = false; 
		} 
	} 
	else { 
		_ctl.hbrBackground = wc.hbrBackground; 
	} 

	return ok;
}


// Handles window creation
bool KWindow::CreateEx(DWORD dwExStyle, DWORD dwStyle, HWND hParent, HINSTANCE hInst, BOOL bMDIChild) { 
	if(hInst == 0) { 
		hInst = GetModuleHandle(0); 
	} 
	if(_ctl.classname.size() == 0) { 
		_ctl.classname = "KWindow"; 
	} 

	if(!RegisterClass(_ctl.classname, hInst)) { 
		return false; 
	} 

	int parents_width = CW_USEDEFAULT; 
	int parents_height = CW_USEDEFAULT; 

	if(hParent) { 
		RECT r; 
		GetClientRect(hParent, &r); 

		parents_width = r.right - r.left; 
		parents_height = r.bottom - r.top; 
	} 
	else { 
		HDC hdc = GetDC(NULL); 
		parents_width = GetDeviceCaps(hdc, HORZRES); 
		parents_height = GetDeviceCaps(hdc, VERTRES); 
		ReleaseDC(NULL, hdc); 

		_g_horizontalResolution = parents_width; 
	} 

	bool is_horizontal = parents_width > parents_height; 

	RECT rect = _ctl.GetRect(parents_width, parents_height); 

	int x = rect.left; 
	int y = rect.top; 
	int width = rect.right - rect.left; 
	int height = rect.bottom - rect.top; 

	if(width < 0) { 
		width = 0; 
	} 
	if(height < 0) { 
		height = 0; 
	} 

//	Use MDICREATESTRUCT to support MDI child window. 
//	All members are sero except one. This is a hint to WindowProc that this window is not a MDI child. 
//	If bMDIChild == TRUE, then all members will be populated accordingly. 
	MDICREATESTRUCT mdichild; 
	memset(&mdichild, 0, sizeof(mdichild)); 
	mdichild.lParam = (LPARAM)this; 

//	Use CLIENTCREATESTRUCT to support MDI client window
	CLIENTCREATESTRUCT mdiclient; 
	memset(&mdiclient, 0, sizeof(mdiclient)); 
	mdiclient.idFirstChild = 1000; 

	LPVOID lpParam = 0; 

	if(bMDIChild) { 
		int max_mdichilds = _ctl.max_mdichilds;
		if(max_mdichilds <= 0) { 
			max_mdichilds = 3; 
		} 

		mdichild.szClass = _ctl.classname.c_str();
		mdichild.szTitle = _ctl.name.c_str();
		mdichild.x = CW_USEDEFAULT;
		mdichild.y = CW_USEDEFAULT;
		mdichild.cx = is_horizontal? (parents_width / max_mdichilds): parents_width; 
		mdichild.cy = is_horizontal? parents_height: (parents_height / max_mdichilds);
		mdichild.style = dwStyle; 

		double xscale = 0; 
		double yscale = 0; 
		xscale = 1000.0 / (double)parents_width; 
		yscale = 1000.0 / (double)parents_height; 

		_ctl.x = 0; 
		_ctl.y = 0; 
		_ctl.right = _ctl.x + (int)floor(mdichild.cx * xscale + 0.5); 
		_ctl.bottom = _ctl.y + (int)floor(mdichild.cy * yscale + 0.5); 

		_ctl.width = mdichild.cx; 
		_ctl.height = mdichild.cy; 

		_ismdichildWindow = TRUE; 

		_hwnd = (HWND)SendMessage(hParent, WM_MDICREATE, 0, (LPARAM)&mdichild); 

		SendMessage(hParent, WM_MDITILE, (is_horizontal? MDITILE_VERTICAL: MDITILE_HORIZONTAL), 0); 
	} 
	else { 
		if(_ctl.classname == "MDICLIENT") { 
			if(_ctl.max_mdichilds <= 0) { 
				_ctl.max_mdichilds = 3; 
			} 
			lpParam = &mdiclient; 
			dwStyle = dwStyle | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL; 
		} 
		else { 
			lpParam = &mdichild; 
		} 

		_hwnd = CreateWindowExA(
			dwExStyle, _ctl.classname.c_str(), _ctl.name.c_str(), dwStyle, x, y, width, height, hParent, NULL, hInst, lpParam); 
	} 

//	This may not work at all when it is a child window. 
	if(_ctl.transparent) { 
		if(SetWindowLong(_hwnd, GWL_EXSTYLE, GetWindowLong(_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED)) { 
			SetLayeredWindowAttributes(_hwnd, (DWORD)_ctl.backcolor, 0, LWA_COLORKEY); 
		} 
	} 

	if(_ctl.font_face.size()) { 
		HDC hdc = GetDC(_hwnd); 
		int font_height = _ctl.font_height; 
		if(_g_horizontalResolution < 1650) { 
			if(font_height > 8) { 
				font_height -= 1; 
			} 
		} 
		HFONT hFont = MyCreateFont(hdc, _ctl.font_face, font_height, _ctl.font_bold, _ctl.font_italic, FALSE, _ctl.font_charset); 
		ReleaseDC(_hwnd, hdc); 

		if(hFont) { 
			SendMessage(_hwnd, WM_SETFONT, (WPARAM)hFont, 0); 
		} 
	} 
    
	if(_ctl.name.size()) { 
		SetWindowTextA(_hwnd, (LPCSTR)_ctl.name.c_str()); 
	} 

	return _hwnd != NULL; 
}

void KWindow::Resize(HWND hParent, int width, int height) { 
	RECT rect; 
	rect = _ctl.GetRect(width, height); 

	if(!_ctl.width || !_ctl.height) { 
		return; 
	} 

	int x = rect.left; 
	int y = rect.top; 
	int w = rect.right - rect.left; 
	int h = rect.bottom - rect.top; 

	MoveWindow(_hwnd, x, y, w, h, TRUE); 
} 

BOOL KWindow::ShowWindow(int nCmdShow) const { 
	BOOL rc = ::ShowWindow(_hwnd, nCmdShow); 
	if(rc) { 
		rc = UpdateWindow(); 
	} 
	return rc; 
}

BOOL KWindow::UpdateWindow(void) const { 
	BOOL rc = ::UpdateWindow(_hwnd); 
	return rc;
}

void KWindow::Invalidate() { 
	RECT rect; 
	if(GetWindowRect(_hwnd, &rect)) { 
		HWND hparent = GetParent(_hwnd); 
		if(hparent) { 
			POINT actual_origin = { rect.left, rect.top }; 
			ScreenToClient(hparent, &actual_origin); 

			MoveWindow(_hwnd, actual_origin.x, actual_origin.y, rect.right + 1 - rect.left, rect.bottom + 1 - rect.top, TRUE); 
			MoveWindow(_hwnd, actual_origin.x, actual_origin.y, rect.right - rect.left, rect.bottom - rect.top, TRUE); 
		} 
		else { 
			MoveWindow(_hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE); 
		} 
	} 
} 




void KWindow::SetTopMost(HWND hWndInsertAfter) { 
	UINT flags = 0; 
	switch((unsigned long long)hWndInsertAfter) { 
		case HWND_BOTTOM: 
			flags = SWP_HIDEWINDOW; 
			EnableWindow(_hwnd, FALSE); 
			break; 
		case HWND_NOTOPMOST: // Places the window above all non-topmost windows. 
			flags = SWP_SHOWWINDOW; 
			for(HWND htopmost = GetTopWindow(GetParent(_hwnd)), holdtopmost = 0; htopmost && htopmost != _hwnd && holdtopmost != htopmost; htopmost = GetTopWindow(GetParent(_hwnd))) { 
				SetWindowPos(htopmost, _hwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
				holdtopmost = htopmost; 
//				EnableWindow(htopmost, FALSE); 
			} 
			break; 
		case HWND_TOP: 
			flags = SWP_SHOWWINDOW; 
			break; 
		case HWND_TOPMOST:
			flags = SWP_SHOWWINDOW; 
			break; 
	} 
	if(flags == SWP_SHOWWINDOW) { 
		EnableWindow(_hwnd, TRUE); 
	} 
	SetWindowPos(_hwnd, hWndInsertAfter, 0, 0, 0, 0, flags | SWP_NOMOVE | SWP_NOSIZE); 
} 

// Fill WNDCLASSEX, virtual function
void KWindow::GetWndClassEx(WNDCLASSEXA& wc)
{
	memset(&wc, 0, sizeof(wc));

	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;       
	wc.hInstance     = NULL;
	wc.hIcon         = _hicon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = NULL;
	wc.hIconSm       = NULL;
}


WPARAM KWindow::MessageLoop(void) { 
	MSG msg;

	_ismainWindow = TRUE;

	while(GetMessage(&msg, NULL, 0, 0)) { 
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


// Common message processing for MDI Child Window
HRESULT KWindow::CommonMDIChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
	switch (uMsg) { 
		case WM_NCDESTROY:						   // should be the last message 
			SetWindowLongPtr(hWnd, GWL_USERDATA, 0);  // make sure no more message through WindowProc 
//			send message to parent window to response to child window deletion. 
			if(!SendMessage(GetParent(GetParent(hWnd)), WM_USER, WM_NCDESTROY, (LPARAM)hWnd)) { 
//				MDI childs are created using new operator; 
//				Zero lResult means that parent did not delete it. 
//				Delete it from here. 
				delete this; 
			} 
			return 0; 

		case WM_MDIACTIVATE: 
//			wParam Handle to the MDI child window being deactivated. 
//			lParam Handle to the MDI child window being activated

//			send message to parent window to response to child window change. 
			if(lParam == (LPARAM)hWnd) { 
				SendMessage(GetParent(GetParent(hWnd)), WM_USER, WM_MDIACTIVATE, (LPARAM)hWnd); 
				Invalidate(); 
			} 
			return 0;
			
		default:
//			generic MDI child window message handling provided by OS. 
			return (HRESULT)DefMDIChildProc(hWnd, uMsg, wParam, lParam);
	}
}


LRESULT KWindow::OnQueryNewPalette(void) { 
	if(_hpalette == NULL) { 
		return FALSE; 
	} 

	HDC hDC = GetDC(_hwnd); 
	HPALETTE hOld= SelectPalette(hDC, _hpalette, FALSE);

	BOOL changed = RealizePalette(hDC) != 0;
	SelectPalette(hDC, hOld, FALSE);
	ReleaseDC(_hwnd, hDC);

	if(changed) { 
		InvalidateRect(_hwnd, NULL, TRUE); // repaint
	}

	return changed;
}


LRESULT KWindow::OnPaletteChanged(HWND hWnd, WPARAM wParam) { 
	if(hWnd != (HWND)wParam && _hpalette != 0) { 
		HDC hDC = GetDC(hWnd);
		HPALETTE hOld = SelectPalette(hDC, _hpalette, FALSE);
				
		if(RealizePalette(hDC)) { 
			if(_updateCount >= 2) { 
				InvalidateRect(hWnd, NULL, TRUE);
				_updateCount = 0;
			}
			else { 
				UpdateColors(hDC);
				++_updateCount; 
			} 
		} 

		SelectPalette(hDC, hOld, FALSE);
		ReleaseDC(hWnd, hDC);
	}

	return 0;
}


void KWindow::OutputText(HDC hDC, UINT align, int x, int y, LPCTSTR szFace, LPCTSTR szMessage, int point) { 
    HFONT hFont = CreateFont(
        - point * GetDeviceCaps(hDC, LOGPIXELSY) / 72,
        0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE, 
        ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, 
        PROOF_QUALITY, VARIABLE_PITCH, szFace);
    assert(hFont);

    HGDIOBJ hOld = SelectObject(hDC, hFont);

    SetTextAlign(hDC, align | TA_BASELINE); // TA_CENTER 

    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, RGB(0, 0, 0xFF));
    TextOut(hDC, x, y, szMessage, (int)_tcslen(szMessage));

    SelectObject(hDC, hOld);
    DeleteObject(hFont);
} 



int MyMessageBox(HWND hWnd, const std::string& text, const std::string& caption, DWORD style, int iconid) { 
	MSGBOXPARAMSA param;

	memset(& param, 0, sizeof(param));
	param.cbSize	  = sizeof(param);
	param.hwndOwner   = hWnd;
	param.hInstance   = GetModuleHandle(NULL);
	param.lpszText    = text.c_str();
	param.lpszCaption = caption.c_str();
	param.dwStyle     = style | MB_USERICON;
	param.lpszIcon    = MAKEINTRESOURCE(iconid);

	return MessageBoxIndirectA(&param);
}


HFONT MyCreateFont(HDC hdc, const std::string& face, int point, BOOL bold, BOOL italic, BOOL underline, BOOL strikeout, DWORD charset) { 
	if(point <= 0) { 
		point = 10; 
	} 

	HFONT hFont = CreateFont( 
		-point * GetDeviceCaps(hdc, LOGPIXELSY) / 72, 0, 0, 0, 
		bold? FW_BOLD: 0, 
		italic? TRUE: FALSE, 
		underline? TRUE: FALSE, 
		strikeout? TRUE: FALSE, 
		charset, 
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY/*PROOF_QUALITY*/, VARIABLE_PITCH, 
		face.c_str()); 

	return hFont; 
} 
















