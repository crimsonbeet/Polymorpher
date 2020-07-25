// Adopted from sources of Yuan, Feng 
// Created by Alex Bourakov. 

#include "stdafx.h"
#include "logwindow.h"


KLogWindow::KLogWindow() { 
	_hinst = NULL; 
	_heditWnd = NULL; 
	_limitsize = 0; 
}

KLogWindow::~KLogWindow() { 
} 

void KLogWindow::Create(HINSTANCE hInst, HWND hParent) { 
	_hinst = hInst; 
	if(_ctl.classname.size() == 0) { 
		_ctl.classname = "KLogWindow"; 
	} 
	CreateEx(0, WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_THICKFRAME/* | WS_BORDER*/, hParent, _hinst); 
}

void KLogWindow::ShowWindow(bool bShow) { 
	int ncmd = bShow? (SW_RESTORE | SW_SHOWNORMAL): (SW_HIDE | SW_MINIMIZE); 
	::ShowWindow(_hwnd, ncmd);
}

LRESULT KLogWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
	switch(uMsg) { 
		case WM_CREATE: { 
			_hwnd = hWnd; 

			RECT rect;
			GetClientRect(_hwnd, & rect); 

			DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY | ES_AUTOHSCROLL | ES_AUTOVSCROLL; 
			if(_hscroll) { 
				dwStyle |= WS_HSCROLL; 
			} 
			if(_vscroll) { 
				dwStyle |= WS_VSCROLL; 
			} 

			_heditWnd = CreateWindowA("EDIT", NULL,  dwStyle, 0, 0, rect.right, rect.bottom, _hwnd, NULL, _hinst, NULL); 

			if(_limitsize == 0) { 
				_limitsize = 2 * 1024 * 1024; 
			} 

			SendMessage(_heditWnd, WM_SETFONT, (WPARAM) GetStockObject(SYSTEM_FIXED_FONT), 0); 
			SendMessage(_heditWnd, EM_LIMITTEXT, _limitsize, 0); 
		} 
		return 0; 

		case WM_SETFONT: 
			SendMessage(_heditWnd, WM_SETFONT, wParam, lParam); 
			return 0; 

		case WM_KEYDOWN: 
			PostMessage(GetParent(_hwnd), uMsg, wParam, lParam); 
			return 0; 

		case WM_SIZE:
			MoveWindow(_heditWnd, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);	
			return 0; 

		case WM_NCDESTROY: 
			return 0; 
	} 

	return DefWindowProc(hWnd, uMsg, wParam, lParam); 
} 


void KLogWindow::Log(const char * format, ...) { 
	char buffer[1024];

    va_list ap;

	va_start(ap, format);
	_vsnprintf(buffer, 1024, format, ap); 
	WPARAM wParam = 0xFFFFFF; 
	if(SendMessage(_heditWnd, WM_GETTEXTLENGTH, 0, 0) > (_limitsize - 1024)) { 
		wParam = 0;
	} 

	SendMessage(_heditWnd, EM_SETSEL, wParam, 0xFFFFFF);
	SendMessage(_heditWnd, EM_REPLACESEL, 0, (LPARAM) buffer); 

	va_end(ap);
} 
