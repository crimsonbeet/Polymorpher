#pragma once
// Created by Alex Bourakov. 

#ifdef WSI_NAMESPACE
#undef WSI_NAMESPACE
#endif
#define WSI_NAMESPACE KWindows 

#ifdef autocreateserialization_not_necessaryH
#undef autocreateserialization_not_necessaryH
#endif


#include "WSIClassFactory.h"

#ifdef EXP_KWIN 
#define EXP_STL
#else 
#define EXP_KWIN 
#endif 

#pragma warning(disable: 4251) 


EXPORT_TEMPLATE_CLASS(std::string)
EXPORT_STL_VECTOR(std::string)


#include "windowimgcapturer.h" 


struct XHColor { 
	unsigned char _hex_color[sizeof(DWORD)]; 

	DWORD operator = (DWORD val) { 
		*((DWORD*)_hex_color) = val; 
//		std::reverse(_hex_color, _hex_color + ARRAY_NUM_ELEMENTS(_hex_color)); 

		return val; 
	} 

	operator DWORD() { 
		DWORD val = *((DWORD*)_hex_color); 
//		std::reverse((PCHAR)&val, ((PCHAR)&val) + sizeof(DWORD)); 
		return val; 
	} 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(XHColor)

CONTAINS_FLAT_MEMBER(_hex_color, Hex)
END_WSI_SERIALIZATION_OBJECT()




struct EXP_KWIN KWinctl { 
//	in 1/1000 units. 
	int x; 
	int y; 
	int bottom; 
	int right; 

	int isAbsolute; 

	int transparent; 

//	in device units. 
	int width; 
	int height; 

	int id; 

	int max_mdichilds; // For MDI client only. Defines initial size of MDI child. 

	std::string name; 
	std::string classname; 

	XHColor backcolor; 

	HBRUSH hbrBackground; 

	std::string font_face; 
	int font_height; 
	int font_bold; 
	int font_italic; 
	DWORD font_charset; 

	KWinctl(); 
	RECT GetRect(int parent_width, int parent_height); // Calculates window rectangle from parent's width and height. 
	VOID SetRect(int newwidth, int newheight); 
	VOID SetRect(HWND hwnd); 
}; 


BEGIN_WSI_SERIALIZATION_OBJECT(KWinctl)

CONTAINS_FLAT_MEMBER(x, Left)
CONTAINS_FLAT_MEMBER(y, Top)
CONTAINS_FLAT_MEMBER(bottom, Bottom)
CONTAINS_FLAT_MEMBER(right, Right)
CONTAINS_FLAT_MEMBER(isAbsolute, IsAbsolute)
CONTAINS_FLAT_MEMBER(transparent, Transparent)
CONTAINS_FLAT_MEMBER(id, Id)
CONTAINS_FLAT_MEMBER(max_mdichilds, MaxMdiChilds)
CONTAINS_FLAT_MEMBER(name, Name)
CONTAINS_FLAT_MEMBER(classname, ClassName)
CONTAINS_FLAT_MEMBER(font_face, FontFace)
CONTAINS_FLAT_MEMBER(font_height, FontHeight)
CONTAINS_FLAT_MEMBER(font_bold, FontBold)
CONTAINS_FLAT_MEMBER(font_italic, FontItalic)
CONTAINS_FLAT_MEMBER(font_charset, FontCharset)
CONTAINS_OBJECT_MEMBER(backcolor, BackColor)
END_WSI_SERIALIZATION_OBJECT()




struct EXP_KWIN KBitmapctl { 
	int id; 

	std::string file_name; 

	int bitmap_rows; 
	int bitmap_columns; 

	KBitmapctl(); 
}; 


BEGIN_WSI_SERIALIZATION_OBJECT(KBitmapctl)

CONTAINS_FLAT_MEMBER(id, Id)
CONTAINS_FLAT_MEMBER(file_name, FileName)
CONTAINS_FLAT_MEMBER(bitmap_rows, Rows)
CONTAINS_FLAT_MEMBER(bitmap_columns, Columns)
END_WSI_SERIALIZATION_OBJECT()



class EXP_KWIN KWindow; 



typedef struct _OnDrawParams { 
	KWindow *_pwin; 
	HDC _hdc; 
	RECT _rcpaint; 

	_OnDrawParams() { 
		memset(this, 0, sizeof(_OnDrawParams)); 
	} 

} SOnDrawParams, *PSOnDrawParams; 

typedef struct _OnMessageParams { 
	KWindow *_pwin/*in*/; 
	UINT _umsg/*in*/; 
	WPARAM _wparam/*in*/; 
	LPARAM _lparam/*in*/; 

	BOOL _processed/*out*/; 
	LRESULT _lresult/*out*/; 

	_OnMessageParams() { 
		_pwin = 0; 
		_umsg = 0; 
		_wparam = 0; 
		_lparam = 0; 
		_processed = FALSE; 
		_lresult = 0; 
	} 

} SOnMessageParams, *PSOnMessageParams; 


//EXPORT_TEMPLATE_STRUCT(IXDelegate<SOnDrawParams, void>)
//EXPORT_TEMPLATE_STRUCT(IXDelegate<SOnMessageParams, void>)

typedef struct _KWindowDelegates { 

	IXDelegate<SOnDrawParams, void> _ondraw_delegates; 
	IXDelegate<SOnMessageParams, void> _onmessage_delegates; 

} SKWindowDelegates, *PSKWindowDelegates; 


typedef IXFunction<SOnDrawParams, void> SOnDrawHandler; 
typedef IXFunction<SOnMessageParams, void> SOnMessageHandler; 


// It was started from sources of Yuan, Feng, 
// although there is almost nothing left in common. 

class EXP_KWIN KWindow { 
protected:
	virtual void GetWndClassEx(WNDCLASSEX & wc);

	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

//	Generic window procedure passed to WIN32 API. 
//	It dispatches to KWindow::WndProc.
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
   
	HRESULT CommonMDIChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnQueryNewPalette(void);
	virtual LRESULT OnPaletteChanged(HWND hWnd, WPARAM wParam);

	KWindowImgCapturer _imgcapturer; 

	HPALETTE _hpalette;
	int	_updateCount; 

	HICON _hicon; 

	BOOL _ismainWindow; 
	BOOL _ismdichildWindow; 

	static BOOL _is_sizing; 

	bool CreateEx(DWORD dwExStyle, DWORD dwStyle, HWND hParent, HINSTANCE hInst, BOOL bMDIChild = FALSE); 

public:
	HWND _hwnd; 
	KWinctl _ctl; 
	KBitmapctl _bitmapctl; 

	PSKWindowDelegates _delegates; 

	BOOL _destroyed; 
	
	KWindow(); 
	virtual ~KWindow(); 
    
	virtual void Create(HINSTANCE hInst, HWND hParent) = 0; 
	virtual void Invalidate(); 
	virtual void Resize(HWND hParent, int width, int height); 

	BOOL IsSizing(); 

	bool RegisterClass(const std::string& classname, HINSTANCE hInst);

	virtual WPARAM MessageLoop(void); 

	BOOL ShowWindow(int nCmdShow = SW_SHOW) const; 
	BOOL UpdateWindow(void) const; 

	void SetTopMost(HWND hWndInsertAfter = HWND_NOTOPMOST/*HWND_TOP*/); 

	void OutputText(HDC hDC, UINT align, int x, int y, LPCTSTR szFace, LPCTSTR szMessage, int point); 

};



HFONT MyCreateFont(HDC hdc, const std::string& face, int point, BOOL bold, BOOL italic, BOOL underline = FALSE, BOOL strikeout = FALSE, DWORD charset = ANSI_CHARSET); 

int MyMessageBox(HWND hWnd, const std::string& text, const std::string& caption, DWORD style, int iconid);
