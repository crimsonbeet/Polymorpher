#include "StdAfx.h"
#include ".\windowimgcapturer.h" 

const char *_g_KWindowImgCapturerName = "KWindowImgCapturer"; 

KWindowImgCapturer::KWindowImgCapturer() { 
	Init(); 
}

KWindowImgCapturer::~KWindowImgCapturer() {
}

void KWindowImgCapturer::Init() { 
	_hwnd = 0; 
	_hbitmap = 0; 
	_holdbitmap = 0; 
	_hmemdc = 0; 
}



LRESULT CALLBACK KWindowImgCapturer::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
	KWindowImgCapturer *pWindow = 0; 
        
	if(uMsg == WM_NCCREATE) { 
		MDICREATESTRUCT *pMDIC = (MDICREATESTRUCT*)((LPCREATESTRUCT)lParam)->lpCreateParams; 
		pWindow = (KWindowImgCapturer*)(pMDIC->lParam);
		if(pWindow) { 
			SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pWindow);
		} 
	}
	else { 
		pWindow = (KWindowImgCapturer*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA); 
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

LRESULT KWindowImgCapturer::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
	switch(uMsg) {
		case WM_PAINT:	{ 
			PAINTSTRUCT ps; 
			HDC hdc = BeginPaint(_hwnd, &ps); 
			EndPaint(_hwnd, &ps); 
		} 
		return 0; 

		case WM_ERASEBKGND: if(_hmemdc) { 
			BITMAP bmp; 
			if(GetObject(_hbitmap, sizeof(bmp), &bmp)) { 
				BitBlt((HDC)wParam, 0, 0, bmp.bmWidth, bmp.bmHeight, _hmemdc, 0, 0, SRCCOPY); 
			} 
			return 1; 
		} 
		break; 

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
} 

bool KWindowImgCapturer::Create(HINSTANCE hInst, HWND hParent) { 
	WNDCLASSEXA wc;
	wc.cbSize = sizeof(wc); 

//	Check if class is already registered
	bool ok = true; 
	if(!GetClassInfoExA(hInst, _g_KWindowImgCapturerName, &wc)) { 
		memset(&wc, 0, sizeof(wc));

		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc   = WindowProc;
		wc.hInstance     = hInst;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW); 

		wc.lpszClassName = _g_KWindowImgCapturerName; 

		if(!RegisterClassExA(&wc)) { 
			ok = false; 
		} 
	} 

	if(!ok) { 
		return false; 
	} 

	RECT rect; 
	if(!GetClientRect(hParent, &rect)) { 
		return false; 
	} 

	int width = rect.right; 
	int height = rect.bottom; 

//	Capture client area image into memory device context based on DDB. 
	HDC hdc = GetWindowDC(NULL); // HDC of the screen. 

	_hbitmap = CreateCompatibleBitmap(hdc, width, height); 
	if(_hbitmap) { 
		_hmemdc = CreateCompatibleDC(hdc); 
		_holdbitmap = (HBITMAP)SelectObject(_hmemdc, _hbitmap); 

		POINT actual_origin = { 0, 0 }; 
		ClientToScreen(hParent, &actual_origin); 

		BitBlt(_hmemdc, 0, 0, width, height, hdc, actual_origin.x, actual_origin.y, SRCCOPY); 
	} 

	ReleaseDC(NULL, hdc); 

//	Use MDICREATESTRUCT to support MDI child window
	MDICREATESTRUCT mdic; 
	memset(&mdic, 0, sizeof(mdic)); 
	mdic.lParam = (LPARAM)this; 

	_hwnd = CreateWindowExA(0, _g_KWindowImgCapturerName, 0, WS_VISIBLE | WS_CHILD, 0, 0, width, height, hParent, 0, hInst, &mdic); 
	if(_hwnd) { 
		SetWindowPos(_hwnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE); 
		UpdateWindow(_hwnd); 
	} 

	return _hwnd != NULL; 
}

void KWindowImgCapturer::Destroy() { 
	if(_hmemdc) { 
		SelectObject(_hmemdc, _holdbitmap); 
		DeleteObject(_hmemdc); 
	} 
	if(_hbitmap) { 
		DeleteObject(_hbitmap); 
	} 
	if(_hwnd) { 
		DestroyWindow(_hwnd); 
	} 

	Init(); 
} 


HRGN KWindowImgCapturer::CreateWinRegion() { 
	HRGN hrgn = NULL; 
	RECT rect; 
	if(GetClientRect(_hwnd, &rect)) { 
		if(!IsRectEmpty(&rect)) { 
			hrgn = CreateRectRgn(0, 0, rect.right, rect.bottom); 
		} 
	} 
	return hrgn; 
} 

