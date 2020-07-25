#pragma once
// Adopted from sources of Yuan, Feng 
// Created by Alex Bourakov. 

#include ".\win.h"
#include ".\Status.h"
#include ".\LogWindow.h"

// main drawing window, normally a child window of a frame window

class EXP_KWIN KCanvas : public KWindow { 
	void Destroy(); 

	void OnScroll(int nBar, int nScrollCode, int nPos); 

	IXFunction<SOnMessageParams, void> *_messagehandler; 
	void OnMessage(SOnMessageParams& params); 

	IXFunction<SOnDrawParams, void> *_painthandler; 
	virtual void OnPaint(SOnDrawParams& params); 

	BOOL SetScrollBar(int side, int canvassize); 

	int _linedx; 
	int _linedy; 

protected: 
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam); 

	std::vector<KCanvas*> _mdichilds; 
	
public: 
	void SetScrollDim(int win_width, int win_height, int linedx, int linedy); 

	std::string _data_type; 

	HINSTANCE _hinst; 

	KCanvas(); 

	void Create(HINSTANCE hInst, HWND hParent); 

	bool CreateMDIChild(KCanvas *mdichild/*must not be initialized*/); 
	bool RemoveMDIChild(HWND hwnd); 

	void Resize(HWND hParent, int width, int height); 
	void Invalidate(); 

	virtual ~KCanvas();
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(KCanvas)

CONTAINS_FLAT_MEMBER(_data_type, DataType)
//CONTAINS_INHERITED_OBJECT_MEMBER(KWindow, _ctl, Situation)
CONTAINS_OBJECT_MEMBER(_ctl, Situation)
END_WSI_SERIALIZATION_OBJECT()

