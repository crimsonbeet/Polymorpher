#pragma once

#include ".\win.h"
#include ".\HListItem.h" 


#define KHL_XBMPOFFSET  2

#define KHL_IDLIST    101
#define KHL_IDEDIT    102

EXPORT_STL_VECTOR(CHFormatItem)

struct CHListItemDrawCtl { 
	int nUnits; 
	int nColumns[32]; 
	RECT rcTemp; 
	int nLevel;
	int nSelectedCell; 
	DWORD dwConnectLevel; 
	LONG wIndent; 
	LONG wAbsIndent; 
	LONG wTopText; 
	LONG wTopBitmap; 
	LONG wTotalWidth; 

	CHListItemDrawCtl() { 
		memset(this, 0, sizeof(CHListItemDrawCtl)); 
	} 
}; 


class EXP KTreeWindow: public KWindow { 
	HINSTANCE _hinst; 

	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

	HWND _hwndListBox; 
	HWND _hwndEditBox; 
	static LRESULT CALLBACK ListBoxSubclassWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 

public: 
	LONG _preallocated_items; 
	double _line_heightfactor; 
	BOOL _debug; 
	BOOL _noheaders; 

	std::vector<CHFormatItem> _format_list; 
	std::string _data_type; 

	KTreeWindow(); 
	virtual ~ KTreeWindow(); 
	
	void Create(HINSTANCE hInst, HWND hParent); 
	void OnCreate(HWND hMe); 
	BOOL OnCommand(WPARAM wParam, LPARAM lParam); 

private: 
	HDC m_hdcMem; 

	HBITMAP m_hbmMem;
	LONG m_nBitmapHeight;
	LONG m_nBitmapWidth;
	LONG m_nTextHeight;
	LONG m_nLineHeight;
	BOOL m_bLines; 

	BOOL m_bEditBoxActive; 
	WORD m_bEditBoxChanges; 

	RECT m_rLastHyperlinkRect; 

	BOOL ClearLastHyperLinkSelection(); 
	BOOL DrawInit(); 
	VOID DrawSetTextHeight(HFONT hFont); 
	VOID BuildDrawItemCtl(CHListItem *pItem/*in*/, RECT& item_rect/*in*/, CHListItemDrawCtl& ctl/*out*/); 

	VOID OnLbMouseMove(LPARAM lParam); 

	VOID OnMeasureItem(MEASUREITEMSTRUCT *lpMeasureItem); 
	VOID OnDrawItem(DRAWITEMSTRUCT * lpDrawItem); 
	VOID OnActionItem(CHListItem *pItem, DWORD wItemNum); 
	BOOL OnSelectItem(CHListItem *pItem, DWORD wItemNum); 

	DWORD RGB2BGR(DWORD rgb) { return RGB(GetBValue(rgb), GetGValue(rgb), GetRValue(rgb)); } 
	VOID FastRect(HDC hDC, int x, int y, int cx, int cy); 

private: 
	CHListItem *m_pSelectedItem; 

	RECT m_nSelItemsInnerBoxes[32]; 
	RECT m_nSelItemsOuterBoxes[32]; 

	int m_nSelectedCell; 


public: 
	WNDPROC ListBoxWndProc; 

	int m_nMouseXPos; 
	int m_nMouseYPos; 

	HWND _hwndCntxt; 

	BOOL get_DrawLines() { return m_bLines; } 
	VOID set_DrawLines(BOOL bLines) { m_bLines = bLines; } 

	void Invalidate(); 
	void Clear(); 

	int AddRootItem(CHListItem *); // Returns either index of added item or 0 if it was not added. 
	int RedrawItem(CHListItem *); // Redraws an item, returns either index of item or 0 if it was not found. 

	CHFormatItem * GetFormatItem(BYTE nbLevel); 

	void KHL_DEBUG(const std::string& format, ...); 

}; 


BEGIN_WSI_SERIALIZATION_OBJECT(KTreeWindow)

CONTAINS_FLAT_MEMBER(_debug, Debug)
CONTAINS_FLAT_MEMBER(_data_type, DataType)
CONTAINS_FLAT_MEMBER(_noheaders, NoHeaders)
CONTAINS_FLAT_MEMBER(_preallocated_items, PreallocatedItems)
CONTAINS_FLAT_MEMBER(_line_heightfactor, LineHeightFactor)
CONTAINS_OBJECT_MEMBER(_format_list, ItemFormat)
CONTAINS_INHERITED_OBJECT_MEMBER(KWindow, _ctl, Situation)
CONTAINS_INHERITED_OBJECT_MEMBER(KWindow, _bitmapctl, Bitmap)
END_WSI_SERIALIZATION_OBJECT()
