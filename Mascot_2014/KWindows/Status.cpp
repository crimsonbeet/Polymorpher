// Originally written by Yuan, Feng 
// Created by Alex Bourakov. 

#include "stdafx.h"
#include "Status.h"

#pragma comment(lib, "comctl32.lib")


KStatusWindow::KStatusWindow() { 
	_hwnd = NULL; 
	_id = 0; 
}

void KStatusWindow::Create(HINSTANCE hInst, HWND hParent) { 
	if(_id == 0) { 
		_id = 101; 
	} 
	_hwnd = CreateStatusWindow(
		WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP, 0, hParent, _id);
}


void KStatusWindow::Resize(HWND hParent, int& width, int& height) { 
	RECT Parent;
	RECT Self;

	GetClientRect(hParent, &Parent);
	GetClientRect(_hwnd, &Self);

	int parts[3]; 

	// (width-60)/2, (width-60)/2, 40, 20 
	parts[0] = (width - 60) / 2; 
	parts[1] = (width - 60); 
	parts[2] = (width - 20); 
	SendMessage(_hwnd, SB_SETPARTS, 3, (LPARAM)parts); 

//	MoveWindow(_hwnd, 0, Parent.bottom - Self.bottom, width, height, TRUE);
	MoveWindow(_hwnd, 0, Parent.bottom - Self.bottom, width, Self.bottom, TRUE); 
	height -= Self.bottom; 
}


void KStatusWindow::SetText(int pane, HINSTANCE hInst, int messid, int param) { 
	CHAR pattern[MAX_PATH]; 
	CHAR message[MAX_PATH];

	LoadStringA(hInst, messid, pattern, sizeof(pattern));
	wsprintfA(message, pattern, param); 

	SendMessage(_hwnd, SB_SETTEXT, pane, (LPARAM) message); 
} 

void KStatusWindow::SetText(int pane, LPCTSTR message) { 
	SendMessage(_hwnd, SB_SETTEXT, pane, (LPARAM)message);
} 
