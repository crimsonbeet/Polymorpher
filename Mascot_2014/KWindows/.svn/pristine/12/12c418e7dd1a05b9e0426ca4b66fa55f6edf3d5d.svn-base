#pragma once
// Adopted from sources of Yuan, Feng 
// Created by Alex Bourakov. 

#include <commctrl.h>
#include ".\win.h"

typedef enum { 
	pane_1,
	pane_2,
	pane_3
};


// Status window
class EXP_KWIN KStatusWindow { 
public:
	HWND _hwnd;
	UINT _id;

	KStatusWindow(); 

	void Create(HINSTANCE hInst, HWND hParent); 
	void Resize(HWND hParent, int& width, int& height);
	void SetText(int pane, HINSTANCE hInst, int messid, int param=0);
	void SetText(int pane, LPCTSTR message);
};

BEGIN_WSI_SERIALIZATION_OBJECT(KStatusWindow)

CONTAINS_FLAT_MEMBER(_id, Id)
END_WSI_SERIALIZATION_OBJECT()
