#pragma once

class EXP_KWIN KWindowImgCapturer { 
protected: 
//	Generic window procedure passed to WIN32 API, dispatches to WndProc
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

	HBITMAP _hbitmap; 
	HBITMAP _holdbitmap; 
	HDC _hmemdc; 

	void Init(); 
   
public:
	HWND _hwnd; 

	bool Create(HINSTANCE hInst, HWND hParent); 
	void Destroy(); 

	HRGN CreateWinRegion(); 

	KWindowImgCapturer(); 
	~KWindowImgCapturer(); 
};
