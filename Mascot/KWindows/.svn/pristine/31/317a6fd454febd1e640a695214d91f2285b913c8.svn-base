#pragma once
// Adopted from sources of Yuan, Feng 
// Created by Alex Bourakov. 

#include ".\win.h"

class EXP_KWIN KLogWindow: public KWindow { 
	HINSTANCE _hinst;
	HWND _heditWnd;

	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public: 
	int _limitsize; 
	int _hscroll; // allow horiz. scroll
	int _vscroll; // allow vert. scroll

	KLogWindow(); 
	virtual ~ KLogWindow(); 
	
	void ShowWindow(bool bShow); 

	void Create(HINSTANCE hInst, HWND hParent);
	
	void Log(const char * format, ...);
}; 


BEGIN_WSI_SERIALIZATION_OBJECT(KLogWindow)

CONTAINS_FLAT_MEMBER(_limitsize, LimitSize)
CONTAINS_FLAT_MEMBER(_hscroll, HorizScroll)
CONTAINS_FLAT_MEMBER(_vscroll, VertScroll)
//CONTAINS_INHERITED_OBJECT_MEMBER(KWindow, _ctl, Situation)
CONTAINS_OBJECT_MEMBER(_ctl, Situation)
END_WSI_SERIALIZATION_OBJECT()
