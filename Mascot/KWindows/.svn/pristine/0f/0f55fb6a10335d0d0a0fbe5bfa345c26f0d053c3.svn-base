/*****************************************************************************
Module:  HierarchyList.h
Notices: Created by ANB on 06/21/05
******************************************************************************/


#ifndef hierarchylistH
#define hierarchylistH

#include <windows.h> 
#include <math.h> 

#ifndef ACE_DEBUG 
#define ACE_DEBUG(X) ; 
#endif

#define HLI_DO_ACTION_ONSELECT 1 
#define HLI_DO_INVALIDATE_ONSELECT 2 

class CHListItem 
{ 
	friend class CHierarchyList; 
	BYTE m_nbLevel; 
	BYTE m_nbOpened; 

	LONG m_dwMask; 

	int m_cRef; 

	int AddRef() { return ++m_cRef; } 
	int Release() { 
		int cRef = --m_cRef; 
		if(cRef <= 0) { 
			FreeThis(); 
		} 

		return cRef; 
	} 

public: 
	BYTE m_nbHasKids; 

	CHListItem() : m_cRef(0) { 
		m_dwMask = 0; 

		m_nbLevel = 0; 
		m_nbOpened = 0; 
		m_nbHasKids = 0; 
	} 
	~CHListItem() { 
	} 

	virtual void FreeThis() = 0; 

	virtual void OnAction(HWND hWndCntxt) { } 
	virtual void OnSelect(HWND hWndCntxt, int nCell/*1 - Based*/) { } 
	virtual void OnLostSelect(HWND hWndCntxt) { } 
	virtual BOOL OnClick(HWND hWndCntxt, int nCell/*1 - Based*/) { return FALSE; } 

	virtual void GetColumnsOffsets(
		int &nUnits/*0 - Pos. in Pixels, 1 - Width in Pixels, 2 - Width in Percents*/, 
									int nOffsets[32]) 
	{ 
		nUnits = 0; 
		memset(nOffsets, 0, sizeof(int) * 32); 
	} 

	virtual void GetColumnsBorders(bool bBorders[32]) 
	{ 
		memset(bBorders, 0, sizeof(bool) * 32); 
	} 

	virtual void GetColumnsColors(DWORD dwBkColor[32], DWORD dwTextColor[32]) 
	{ 
		memset(dwBkColor, 0, sizeof(DWORD) * 32); 
		memset(dwTextColor, 0, sizeof(DWORD) * 32); 
	} 

	virtual void GetCanHighlight(bool bCanHighlight[32]) 
	{ 
		memset(bCanHighlight, 1, sizeof(bool) * 32); 
	} 

	virtual void GetBitMapIndices(BYTE nbOpened, int &nRow, int nColumns[32]) // 1 - Based. 
	{ 
		nRow = m_nbLevel + 1; 
		memset(nColumns, 0, 32 * sizeof(int)); 
		nColumns[0] = m_nbHasKids? (m_nbOpened? 2: 1): 3; 
	} 

	virtual void GetColumnsEditFlags(BYTE nbFlags[32]) 
	{ 
		memset(nbFlags, 0, sizeof(bool) * 32); 
	} 

	virtual PTCHAR GetText(int nColumn = 0) = 0;  // 0 - Based 
	virtual PTCHAR SetText(int nColumn = 0, PTCHAR szText = 0) // 0 - Based
	{ 
		return GetText(nColumn); 
	} 

	virtual BOOL IsRightAlignment(int nColumn = 0) // 0 - Based
	{ 
		return false; 
	} 

	virtual CHListItem *FirstChild() = 0; 
	virtual CHListItem *GetNextChild() = 0; 
}; 

class CHierarchyList 
{ 
	PTCHAR m_szWindowClass; 
	WORD m_wCtrlId; 
	HWND m_hWnd; 
	HWND m_hWndListBox; 
	HWND m_hWndEditBox; 
	BOOL m_bEditBoxActive; 
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 
	static LRESULT CALLBACK ListBoxSubclassWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 
	static DWORD m_dwTlsIndex; 

	double m_fLineHeightFactor; 

	WORD m_bEditBoxChanges; 

	RECT m_rcPos; 

	WORD m_nBMapRows; 
	WORD m_nBMapColumns; 

	HDC m_hdcMem; 
	HGLOBAL m_hResMem; 

	HBITMAP m_hbmIcons;
	HBITMAP m_hbmMem;
	LONG m_nBitmapHeight;
	LONG m_nBitmapWidth;
	LONG m_nTextHeight;
	LONG m_nLineHeight;
	BOOL m_bLines; 

	BOOL DrawInit(); 
	VOID DrawSetTextHeight(HFONT hFont); 

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

//	the Tree icons bitmap is assumed to be a 16 color DIB! 
//	it should be loaded through HGLOBAL hResMem = LoadResource(HMODULE hModule, HRSRC hResInfo);
	CHierarchyList(WORD nCtrlId, HWND hWndParent, HWND hWndCntxt, HGLOBAL hResMem, WORD nBMapRows, WORD nBMapColumns, LONG nPreallocatedItems = 0, double fLineHeightFactor = 1.1); 

	VOID set_XPos(LONG x) { m_rcPos.left = x; } 
	LONG get_XPos() { return m_rcPos.left; } 
	VOID set_YPos(LONG y) { m_rcPos.top = y; } 
	LONG get_YPos() { return m_rcPos.top; } 
	VOID set_RPos(LONG r) { m_rcPos.right = r; } 
	LONG get_RPos() { return m_rcPos.right; } 
	VOID set_BPos(LONG b) { m_rcPos.bottom = b; } 
	LONG get_BPos() { return m_rcPos.bottom; } 

	BOOL get_DrawLines() { return m_bLines; } 
	VOID set_DrawLines(BOOL bLines) { m_bLines = bLines; } 

	void Invalidate(); 
	int Show(); 
	void Clear(); 

	int AddRootItem(CHListItem *); // Returns either index of added item or 0 if it was not added. 
	int RedrawItem(CHListItem *); // Redraws an item, returns either index of item or 0 if it was not found. 

	~CHierarchyList(); 
}; 


#endif //hierarchylistH

#ifdef HIERARCHYLIST_IMPL 

#define KHL_XBMPOFFSET  2

#define KHL_IDLIST    101
#define KHL_IDEDIT    102
#define IDBUTTON  104

VOID CHierarchyList::FastRect(HDC hDC, int x, int y, int cx, int cy) 
{ 
	RECT rc;

	rc.left = x;
	rc.right = x + cx;
	rc.top = y;
	rc.bottom = y + cy;

	ExtTextOut(hDC, x, y, ETO_OPAQUE, &rc, NULL, 0, NULL); 
}

CHierarchyList::CHierarchyList(WORD wCtrlId, HWND hWndParent, HWND hWndCntxt, HGLOBAL hResMem, WORD nBMapRows, WORD nBMapColumns, LONG nPreallocatedItems, double fLineHeightFactor) 
{ 
	m_bLines = TRUE; 

	m_pSelectedItem = 0; 

	m_nMouseXPos = 0; 
	m_nMouseYPos = 0; 

	ListBoxWndProc = 0; 

	m_nTextHeight = 0; 
	m_nLineHeight = 0; 

	_line_heightfactor = fLineHeightFactor; 

	m_hResMem = hResMem; 
	m_nBMapRows = nBMapRows; 
	m_nBMapColumns = nBMapColumns; 

	HDC hDC = GetDC(NULL);
	m_hdcMem = CreateCompatibleDC(hDC);
	ReleaseDC(NULL, hDC);

	m_hbmMem = NULL; 

	m_hWndListBox = NULL; 
	m_hWnd = NULL; 

	m_hWndCntxt = hWndCntxt; 

	DrawInit(); 

	HINSTANCE hInstance = GetModuleHandle(NULL); 

	WNDCLASSEX wcex = { 0 };

	m_szWindowClass = TEXT("HierarchyList"); 

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= sizeof(PVOID);
	wcex.hInstance		= hInstance;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_SCROLLBAR + 1);
	wcex.lpszClassName	= m_szWindowClass; 

	if(!RegisterClassEx(&wcex)) { 
//		return; 
	} 

	GetClientRect(hWndParent, &m_rcPos); 

	m_wCtrlId = wCtrlId; 

	m_hWnd = CreateWindow(m_szWindowClass, m_szWindowClass, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
				hWndParent, (HMENU)wCtrlId, hInstance, NULL); 

	if(!m_hWnd) { 
		return; 
	} 

	SetWindowLongPtr(m_hWnd, 0, (LONG_PTR)this); 

	m_bEditBoxActive = false; 
	m_bEditBoxChanges = 0; 

	m_hWndListBox = CreateWindow("LISTBOX", NULL, WS_CHILD | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_WANTKEYBOARDINPUT | WS_VSCROLL | WS_BORDER, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_hWnd, (HMENU)KHL_IDLIST, hInstance, NULL); 

	m_hWndEditBox = CreateWindow("EDIT", NULL, WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_hWndListBox, (HMENU)KHL_IDEDIT, hInstance, NULL); 

	if(m_hWndListBox) { 
		if(_preallocated_items > 0) { 
			SendMessage(m_hWndListBox, LB_INITSTORAGE, _preallocated_items, 8); 
		} 

		ListBoxWndProc = (WNDPROC)SetWindowLongPtr(m_hWndListBox, GWL_WNDPROC, (DWORD)ListBoxSubclassWndProc); 
	} 
} 

CHierarchyList::~CHierarchyList() 
{ 
} 

BOOL CHierarchyList::DrawInit() 
{ 
	HDC hDC = NULL; 

	m_hbmMem = NULL; 
	m_nBitmapHeight = 0; 
	m_nBitmapWidth = 0; 

	if(!m_hdcMem) 
	{ 
		return FALSE; 
	} 

	if(!m_hResMem) 
	{ 
		return FALSE; 
	} 

	if(m_nBMapRows == 0) 
	{ 
		return FALSE; 
	} 

	if(m_nBMapColumns == 0) 
	{ 
		return FALSE; 
	} 

	TEXTMETRIC TextMetrics; 

	hDC = GetDC(m_hWndListBox); 
	GetTextMetrics(hDC, &TextMetrics); 
	ReleaseDC(m_hWndListBox, hDC); 

	m_nTextHeight = TextMetrics.tmHeight + TextMetrics.tmExternalLeading; 

	// Now figure out the bitmaps background color.
	// This code assumes the these are 16 color bitmaps
	// and that the lower left corner is a bit in the background
	// color.
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)LockResource(m_hResMem); 
	if(!lpbi) 
	{ 
		return FALSE; 
	} 

	m_nBitmapHeight = (LONG)lpbi->biHeight / m_nBMapRows; 
	m_nBitmapWidth = (LONG)lpbi->biWidth / m_nBMapColumns; 

	m_nLineHeight = (LONG)(max(m_nBitmapHeight, m_nTextHeight) * _line_heightfactor); 

	PDWORD lpColorTable = (PDWORD)(lpbi + 1); 
	LPSTR lpBits = (LPSTR)(lpColorTable + 16); // ASSUMES 16 COLOR
	int bc = (lpBits[0] & 0xF0) >> 4; // ASSUMES LOWER LEFT CORNER IS BACKGROUND!!!

	BYTE lpbi_container[sizeof(BITMAPINFO) + 16 * sizeof(DWORD)]; 
	LPBITMAPINFO lpbi_newcolor = (LPBITMAPINFO)lpbi_container; 
	memcpy(lpbi_newcolor, lpbi, sizeof(lpbi_container)); 

	*(PDWORD)&lpbi_newcolor->bmiColors[bc] = RGB2BGR(GetSysColor(COLOR_WINDOW)); 

	hDC = GetDC(NULL); 
	m_hbmIcons = CreateDIBitmap(hDC, &lpbi_newcolor->bmiHeader, (DWORD)CBM_INIT, lpBits, lpbi_newcolor, DIB_RGB_COLORS); 
	ReleaseDC(NULL, hDC); 

	if(!m_hbmIcons) 
	{ 
		return FALSE; 
	} 

	m_hbmMem = (HBITMAP)SelectObject(m_hdcMem, m_hbmIcons); 
	if(!m_hbmMem) 
	{ 
		return FALSE; 
	} 

	return TRUE; 
} 

VOID CHierarchyList::DrawSetTextHeight(HFONT hFont) 
{
	TEXTMETRIC TextMetrics; 

	HDC hdc = GetDC(m_hWndListBox);
	SelectObject(hdc, hFont); 
	GetTextMetrics(hdc, &TextMetrics);
	ReleaseDC(m_hWndListBox, hdc);

	m_nTextHeight = TextMetrics.tmHeight; 
	m_nLineHeight = (LONG)(max(m_nBitmapHeight, m_nTextHeight) * _line_heightfactor); 

	SendMessage(m_hWndListBox, LB_SETITEMHEIGHT, 0, MAKELPARAM(m_nLineHeight, 0)); 
}

void CHierarchyList::Invalidate() 
{ 
	if(!m_hWnd) 
	{ 
		return; 
	} 

	SetWindowPos(m_hWnd, NULL, m_rcPos.left, m_rcPos.top, m_rcPos.right, m_rcPos.bottom, 0); 

	if(!m_hWndListBox) 
	{ 
		return; 
	} 

	RECT rc;
	GetClientRect(m_hWnd, &rc); 
	SetWindowPos(m_hWndListBox, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);
	InvalidateRect(m_hWndListBox, NULL, TRUE); 
} 

int CHierarchyList::Show() 
{ 
	if(!m_hWnd) 
	{ 
		return 0; 
	} 

	if(!m_hWndListBox) 
	{ 
		return 0; 
	} 

	SetWindowPos(m_hWnd, NULL, m_rcPos.left, m_rcPos.top, m_rcPos.right - m_rcPos.left, m_rcPos.bottom - m_rcPos.top, 0); 

	RECT rc; 
	GetClientRect(m_hWnd, &rc); 
	SetWindowPos(m_hWndListBox, NULL, 0, 0, m_rcPos.right - m_rcPos.left, m_rcPos.bottom - m_rcPos.top, SWP_NOZORDER); 
	GetWindowRect(m_hWndListBox, &rc); 
	m_rcPos.bottom = rc.bottom - rc.top + m_rcPos.top; 

	SetWindowPos(m_hWndEditBox, NULL, 0, 0, 0, 0, SWP_NOZORDER); 

	ShowWindow(m_hWnd, SW_SHOWNORMAL); 
	ShowWindow(m_hWndListBox, SW_SHOWNORMAL); 
	ShowWindow(m_hWndEditBox, SW_HIDE); 

	UpdateWindow(m_hWnd);
	UpdateWindow(m_hWndListBox);

	return 1; 
} 

void CHierarchyList::Clear() 
{ 
	if(m_hWndListBox) 
	{ 
		SendMessage(m_hWndListBox, LB_RESETCONTENT, 0, 0L); 
	} 
}

int CHierarchyList::AddRootItem(CHListItem *pItem) 
{ 
	if(pItem == 0) 
	{ 
		return 0; 
	} 

	LRESULT lResult = LB_ERR; 
	if(m_hWndListBox) 
	{ 
		pItem->AddRef(); 
		lResult = SendMessage(m_hWndListBox, LB_ADDSTRING, 0, (LPARAM)pItem); 
		if(lResult == LB_ERR || lResult == LB_ERRSPACE) 
		{ 
			pItem->Release(); 
		} 
	} 

	int rc = (lResult == LB_ERR || lResult == LB_ERRSPACE)? 0: ((int)lResult + 1); 
	return rc; 
} 

int CHierarchyList::RedrawItem(CHListItem *pItem) 
{ 
	if(pItem == 0) 
	{ 
		return 0; 
	} 

	LRESULT lResult = LB_ERR; 
	if(m_hWndListBox) 
	{ 
		lResult = SendMessage(m_hWndListBox, LB_FINDSTRING, (WPARAM)-1, (LPARAM)pItem); 
		if(lResult != LB_ERR) 
		{ 
			LONG wItem = (LONG)lResult; 

			LONG wBottomIndex;
			LONG wTopIndex;
			RECT rc;

			wTopIndex = (WORD)SendMessage(m_hWndListBox, LB_GETTOPINDEX, 0, 0L);
			GetClientRect(m_hWndListBox, &rc); 
			wBottomIndex = wTopIndex + (rc.bottom + 1) / m_nLineHeight; 
			if(wItem < wTopIndex) 
			{ 
			} 
			else 
			if(wItem > wBottomIndex) 
			{ 
			} 
			else 
			{ 
				rc.top += (wItem - wTopIndex) * m_nLineHeight; 
				rc.bottom = rc.top + m_nLineHeight; 
				InvalidateRect(m_hWndListBox, &rc, FALSE); // Force redraw
			} 
		} 
	} 

	int rc = (lResult == LB_ERR)? 0: ((int)lResult + 1); 
	return rc; 
} 

VOID CHierarchyList::OnMeasureItem(MEASUREITEMSTRUCT *lpMeasureItem) 
{ 
	lpMeasureItem->itemHeight = m_nLineHeight; 
} 

BOOL CHierarchyList::OnSelectItem(CHListItem *pItem, DWORD dwItemNum) 
{ 
	if(pItem == 0) 
	{ 
		return FALSE; 
	} 

	BOOL do_action = FALSE; 

	if(m_pSelectedItem == pItem) 
	{ 
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("OnSelect Item <%s>\n"), pItem->GetText())); 
		for(int j = 0; j < 32; ++j) { 
			if(	m_nMouseXPos >= m_nSelItemsOuterBoxes[j].left &&
				m_nMouseXPos <  m_nSelItemsOuterBoxes[j].right) { 
					do_action = pItem->OnClick(m_hWndCntxt, j + 1); 

					if(	m_nSelectedCell != j + 1) { 
						pItem->OnSelect(m_hWndCntxt, m_nSelectedCell = j + 1); 
					} 

					BYTE nbEditFlags[32]; 
					pItem->GetColumnsEditFlags(nbEditFlags); 
					if(nbEditFlags[j] > 0) { 
						SetWindowPos(m_hWndEditBox, HWND_TOP, 
							m_nSelItemsInnerBoxes[j].left, 
								m_nSelItemsInnerBoxes[j].top, 
									m_nSelItemsInnerBoxes[j].right - m_nSelItemsInnerBoxes[j].left, 
										m_nSelItemsInnerBoxes[j].bottom - m_nSelItemsInnerBoxes[j].top, 0); 
						SetWindowText(m_hWndEditBox, pItem->GetText(m_nSelectedCell - 1)); 
						ShowWindow(m_hWndEditBox, SW_SHOWNORMAL); 
						PostMessage(m_hWndEditBox, WM_LBUTTONDOWN, MK_LBUTTON, m_nMouseXPos - m_nSelItemsInnerBoxes[j].left);
						PostMessage(m_hWndEditBox, WM_LBUTTONUP, MK_LBUTTON, m_nMouseXPos - m_nSelItemsInnerBoxes[j].left);
//						SetFocus(m_hWndEditBox); 
						m_bEditBoxActive = true; 
					} 
			} 

			if(	m_nSelItemsOuterBoxes[j].right == 0) { 
				break; 
			} 

			InvalidateRect(m_hWndListBox, &m_nSelItemsOuterBoxes[j], TRUE);
		} 
	} 
	else 
	{ 
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("OnSelect Item <%s> is called for the item not being selected\n;current selected item is <%s>"), pItem->GetText(), m_pSelectedItem? m_pSelectedItem->GetText(): TEXT("NULL"))); 
	} 

	return do_action; 
} 

VOID CHierarchyList::OnActionItem(CHListItem *pItem, DWORD dwItemNum) 
{ 
	if(pItem == 0) 
	{ 
		return; 
	} 

	if(pItem->m_nbHasKids) 
	{ 
		if(pItem->m_nbOpened) // It's open ... Close it 
		{ 
			pItem->m_nbOpened = 0; 
			//
			// Remove the child items. Close any children that are 
			// open on the way.
			//
			// No need for recursion. We just close everything along 
			// the way and remove
			// items until we reach the next sibling to the current 
			// item.
			//
			++dwItemNum;

			BYTE nbLevel = pItem->m_nbLevel; 

			CHListItem *pChildItem = (CHListItem *)SendMessage(m_hWndListBox, LB_GETITEMDATA, dwItemNum, 0L); 
			if(pChildItem) 
			{ 
				nbLevel = pChildItem->m_nbLevel;  
			} 

			while(nbLevel > pItem->m_nbLevel) 
			{ 
				pItem->m_nbOpened = 0; 
				SendMessage(m_hWndListBox, LB_DELETESTRING, dwItemNum, 0L); 
				pChildItem = (CHListItem *)SendMessage(m_hWndListBox, LB_GETITEMDATA, dwItemNum, 0L); 
				if(pChildItem == (CHListItem *)LB_ERR)
				{ 
					nbLevel = pItem->m_nbLevel; 
				} 
				else 
				if(pChildItem) 
				{ 
					nbLevel = pChildItem->m_nbLevel;  
				} 
			} 
		} 
		else 
		{
			//
			// It's closed ... Open it
			//
			pItem->m_nbOpened = 1; 

			SendMessage(m_hWndListBox, WM_SETREDRAW, FALSE, 0L);   // Disable redrawing. 
			
			DWORD dwItem = dwItemNum + 1; 

			CHListItem *pChildItem = pItem->FirstChild(); 
			if(!pChildItem) 
			{ 
				pItem->m_nbHasKids = 0; 
			} 
			else 
			{ 
				do 
				{ 
					pChildItem->m_nbLevel = pItem->m_nbLevel + 1; 
					pChildItem->m_dwMask = pItem->m_dwMask; 

					CHListItem *pNextChildItem = pItem->GetNextChild(); 
					if(pNextChildItem) 
					{ 
						pChildItem->m_dwMask |= (DWORD)pow(2, (DWORD)pChildItem->m_nbLevel - 1); 
					} 


					pChildItem->AddRef(); 
					LRESULT lResult = SendMessage(m_hWndListBox, LB_INSERTSTRING, dwItem, (LPARAM)pChildItem); 
					if(lResult == LB_ERR || lResult == LB_ERRSPACE) 
					{ 
						pChildItem->Release(); 
						break; 
					} 

					++dwItem; 
					pChildItem = pNextChildItem; 
				} 
				while(pChildItem); 
			} 

			//
			// Make sure as many child items as possible are showing
			//
			LONG wBottomIndex;
			LONG wTopIndex;
			LONG wNewTopIndex;
			LONG wExpandInView;
			RECT rc;

			LONG wKids = dwItem - dwItemNum; 

			wTopIndex = (WORD)SendMessage(m_hWndListBox, LB_GETTOPINDEX, 0, 0L);
			GetClientRect(m_hWndListBox, &rc); 
			wBottomIndex = wTopIndex + (rc.bottom + 1) / m_nLineHeight; 

			wExpandInView = (wBottomIndex - dwItemNum); 

			if(wKids >= wExpandInView) 
			{ 
				wNewTopIndex = min((LONG)dwItemNum, (wTopIndex + wKids + 1) - wExpandInView); 
				SendMessage(m_hWndListBox, LB_SETTOPINDEX, (DWORD)wNewTopIndex, 0L); 
			} 

			SendMessage(m_hWndListBox, WM_SETREDRAW, TRUE, 0L); // Enable redrawing.
			InvalidateRect(m_hWndListBox, NULL, TRUE); // Force redraw
		}
	} 

	pItem->OnAction(m_hWndCntxt); 
}

VOID CHierarchyList::OnDrawItem(DRAWITEMSTRUCT * lpDrawItem) 
{
	DWORD dwConnectLevel = 0;

	if(lpDrawItem->itemID == (UINT)-1) 
	{ 
		return; 
	} 

	CHListItem *pItem = (CHListItem *)lpDrawItem->itemData; 
	if(!pItem) 
	{ 
		return; 
	} 

	TCHAR szText[512] = { 0 }; 
	PTCHAR pszText = pItem->GetText(); 
	if(pszText) { 
		_tcsncpy(szText, pszText, 512); 
	} 

//	ACE_DEBUG((LM_DEBUG, ACE_TEXT("draw item <%s>\n"), szText)); 

	int nUnits = 0; 
	int nColumns[32]; 
	pItem->GetColumnsOffsets(nUnits, nColumns); 

	HDC hDC = lpDrawItem->hDC; 
	RECT rcTemp; 
	CopyRect(&rcTemp, &lpDrawItem->rcItem); 

	int nLevel = pItem->m_nbLevel;

//	Figure out which connecting lines are needed.
	if(nLevel == 0) 
	{
		dwConnectLevel = 0; 
	} 
	else 
	{ 
		dwConnectLevel = pItem->m_dwMask; 
	} 

	LONG wIndent = rcTemp.left + (nLevel * m_nBitmapWidth) + KHL_XBMPOFFSET; 
	LONG wAbsIndent = 0; 
	if(!m_bLines) 
	{ 
		wIndent = rcTemp.left + m_nBitmapWidth + KHL_XBMPOFFSET; 
	} 

	LONG wTopText = rcTemp.top + ((rcTemp.bottom - rcTemp.top) / 2.0) - (m_nTextHeight / 2.0); 
	LONG wTopBitmap = rcTemp.top + ((rcTemp.bottom - rcTemp.top) / 2.0) - (m_nBitmapHeight / 2.0); 

	LONG wTotalWidth = rcTemp.right - rcTemp.left - wIndent; 

	if(nUnits == 0/*Pos. in Pix.*/) 
	{ 
//		Turn absolute positions to width. 
		int j; 
		if(nColumns[0] != 0) { 
			wAbsIndent += nColumns[0]; 
			wIndent += wAbsIndent; 
			wTotalWidth = rcTemp.right - rcTemp.left - wIndent; 
		} 
		
		for(j = 0; j < 31 && (nColumns[j] != 0 || nColumns[j + 1] != 0); ++j) 
		{ 
			if(nColumns[j + 1] == 0) { 
				nColumns[j] = rcTemp.right - nColumns[j]; 
			} 
			else { 
				nColumns[j] = nColumns[j + 1] - nColumns[j]; 
			} 
		} 

		nColumns[j + 1] = 0; 
	} 
	else 
	if(nUnits == 2/*Percents*/) 
	{ 
//		Turn percents to actual width. 
		int j; 
		for(j = 0; j < 32 && nColumns[j] != 0; ++j) 
		{ 
			nColumns[j] = (int)floor(((double)nColumns[j]/100.0) * wTotalWidth + 0.5); 
			if(nColumns[j] > wTotalWidth) { 
				nColumns[j] = 0; 
			} 
		} 
	} 

	int nSelectedCell = 0; 
	{ 
//		Find selected cell. 
		int left = wIndent; 
		int j; 
		for(j = 0; j < 32 && nColumns[j] != 0; ++j) 
		{ 
			int right = left + nColumns[j]; 
			if(m_nMouseXPos >= left && m_nMouseXPos < right) { 
				nSelectedCell = j + 1; 
				break; 
			} 

			left = right; 
		} 
	} 

	BOOL bDrawLinesForOuterLevels = TRUE; 
	BOOL bDrawSelectedBitmap = TRUE; 
	BOOL bDrawText = TRUE; 

	if(lpDrawItem->itemAction == ODA_FOCUS) 
	{ 
		bDrawLinesForOuterLevels = FALSE; 
		bDrawSelectedBitmap = FALSE; 
		if((lpDrawItem->itemState & ODS_FOCUS)) 
		{ 
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("got focus Item <%s> l=%d r=%d t=%d b=%d\n"), szText, rcTemp.left, rcTemp.right, rcTemp.top, rcTemp.bottom)); 
		} 
		else 
		{ 
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("lost focus Item <%s> l=%d r=%d t=%d b=%d\n"), szText, rcTemp.left, rcTemp.right, rcTemp.top, rcTemp.bottom)); 
		} 
	} 
//	else 
	if(lpDrawItem->itemAction == ODA_SELECT)
	{ 
		bDrawLinesForOuterLevels = FALSE; 
		bDrawSelectedBitmap = FALSE; 
		if((lpDrawItem->itemState & ODS_SELECTED)) 
		{ 
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("got selection Item <%s>\n"), szText)); 
			if(m_pSelectedItem) 
			{ 
				m_pSelectedItem->OnLostSelect(m_hWndCntxt); 
			} 

			pItem->OnSelect(m_hWndCntxt, m_nSelectedCell = nSelectedCell); 
			m_pSelectedItem = pItem; 
		} 
		else 
		{ 
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("lost selection Item <%s>\n"), szText)); 
			pItem->OnLostSelect(m_hWndCntxt); 
			m_pSelectedItem = 0; 
		} 

	} 

//	Draw a series of | lines for outer levels

	if(bDrawLinesForOuterLevels && m_bLines && nLevel) 
	{
		// draw lines in text color
		SetBkColor(hDC, GetSysColor(COLOR_WINDOWTEXT)); 

		DWORD dwMask = 1; 

		//
		// Draw a series of | lines for outer levels
		//
		int x;
		x = m_nBitmapWidth / 2 + KHL_XBMPOFFSET; 

		int j; 
		for(j = 0; j < nLevel; ++j) 
		{
			if(dwConnectLevel & dwMask) 
			{ 
				FastRect(hDC, x, rcTemp.top, 1, (rcTemp.bottom - rcTemp.top)); 
			} 

			x += m_nBitmapWidth; 
			dwMask <<= 1; 
		} 

		//
		// Draw the short vert line up towards the parent
		//
		dwMask <<= 1; 

		x = (nLevel - 1) * m_nBitmapWidth + m_nBitmapWidth / 2 + KHL_XBMPOFFSET; 
		int y; 

		if(dwConnectLevel & dwMask) { 
			y = rcTemp.bottom; 
		} 
		else { 
			y = rcTemp.bottom - m_nLineHeight / 2; 
		} 

		FastRect(hDC, x, rcTemp.top, 1, (y - rcTemp.top)); 

		//
		// Draw short horiz bar to right
		//
		FastRect(hDC, x, (rcTemp.bottom - m_nLineHeight / 2), (m_nBitmapWidth / 2) + wAbsIndent, 1); 
	} 

//	Draw cells. 

	memset(m_nSelItemsInnerBoxes, 0, 32 * sizeof(RECT)); 
	memset(m_nSelItemsOuterBoxes, 0, 32 * sizeof(RECT)); 


	bool bBorders[32]; 
	pItem->GetColumnsBorders(bBorders); 

	bool bCanHighlight[32]; 
	pItem->GetCanHighlight(bCanHighlight); 

	DWORD dwBkColor[32]; 
	DWORD dwTextColor[32]; 
	pItem->GetColumnsColors(dwBkColor, dwTextColor); 

	int nBMRow = 0; 
	int nBMColumns[32] = { 0 }; 
	pItem->GetBitMapIndices(pItem->m_nbOpened, nBMRow, nBMColumns); 

	LONG wBitmapXPos = wIndent; 

	for(int j = 0; j < 32 && (nColumns[j] != 0 || nBMColumns[j] != 0); ++j) 
	{ 
		_tcsncpy(szText, pItem->GetText(j), 512); 

		BOOL bDrawBitmap = bDrawSelectedBitmap; 
		if(nBMColumns[j] == 0 || nBMRow == 0 || m_hdcMem == 0) 
		{ 
			bDrawBitmap = 0; 
		} 

		if(bDrawBitmap) { 
			BitBlt(hDC, wBitmapXPos, wTopBitmap, 
				m_nBitmapWidth, 
				m_nBitmapHeight, 
				m_hdcMem,
				m_nBitmapWidth *(nBMColumns[j] - 1), 
				m_nBitmapHeight * (nBMRow - 1), 
				SRCCOPY);
		} 

		rcTemp.left = wBitmapXPos + (nBMColumns[j]? m_nBitmapWidth  + 1: 0); 

		SIZE strSize = { 0 }; 
		if(GetTextExtentPoint(hDC, szText, lstrlen(szText), &strSize)) { 
			rcTemp.right = rcTemp.left + strSize.cx + 1; 
		} 

		if(nColumns[j] > 0) { 
			rcTemp.right = wBitmapXPos + nColumns[j]; 
		} 

		m_nSelItemsInnerBoxes[j] = rcTemp; 
		m_nSelItemsOuterBoxes[j] = rcTemp; 
		m_nSelItemsOuterBoxes[j].left = wBitmapXPos; 

		wBitmapXPos = rcTemp.right; 

		if(bCanHighlight[j] && (lpDrawItem->itemState & ODS_SELECTED)) {
			SetBkColor(hDC, GetSysColor(COLOR_HIGHLIGHT));
			SetTextColor(hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
		} 
		else { 
			if(dwBkColor[j] != 0) { 
				SetBkColor(hDC, dwBkColor[j]); 
			} 
			else { 
				SetBkColor(hDC, GetSysColor(COLOR_WINDOW)); 
			} 
			if(dwTextColor[j] != 0) { 
				SetTextColor(hDC, dwTextColor[j]); 
			} 
			else { 
				SetTextColor(hDC, GetSysColor(COLOR_WINDOWTEXT)); 
			} 
		} 

		if(bDrawText) { 
//			ACE_DEBUG((LM_DEBUG, ACE_TEXT("draw text Item <%s> l=%d r=%d t=%d b=%d\n"), szText, rcTemp.left, rcTemp.right, rcTemp.top, rcTemp.bottom)); 
			if(pItem->IsRightAlignment(j)) { 
				ExtTextOut(hDC, rcTemp.right - strSize.cx - 1, wTopText, ETO_CLIPPED|ETO_OPAQUE, 
						&rcTemp, szText, lstrlen(szText), NULL);
			} 
			else { 
				ExtTextOut(hDC, rcTemp.left + 1, wTopText, ETO_CLIPPED|ETO_OPAQUE, 
						&rcTemp, szText, lstrlen(szText), NULL);
			} 
		} 

		if(bBorders[j]) { 
			FrameRect(hDC, &rcTemp, (HBRUSH)GetStockObject(LTGRAY_BRUSH)); 
		} 
	} 
} 

DWORD CHierarchyList::m_dwTlsIndex = TlsAlloc(); 

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK CHierarchyList::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	CHierarchyList *pWnd = (CHierarchyList *)GetWindowLongPtr(hWnd, 0); 

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	int nMouseXPos = 0; 

	HWND hwndCtl = (HWND)lParam; 

	BOOL command_onselect = FALSE; 

	switch (message) 
	{
	case WM_DELETEITEM: 
		wmId = LOWORD(wParam); 
		if(wmId == KHL_IDLIST)
		{
			LPDELETEITEMSTRUCT lpDeleteItem = (LPDELETEITEMSTRUCT) lParam; 
			CHListItem *pItem = (CHListItem *)lpDeleteItem->itemData; 
			if(pItem) 
			{ 
				pItem->Release(); 
			} 
			return TRUE;
		} 
		break; 

	case WM_SYSCOLORCHANGE:
		pWnd->DrawInit();
		return TRUE;

	case WM_MOUSEMOVE: 
		nMouseXPos = LOWORD(lParam); 
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Mouse X-Pos <%d>\n"), nMouseXPos)); 
		return TRUE; 

	case WM_CHARTOITEM:
		if(wParam == VK_RETURN) 
		{
			WORD wItemNum = (WORD)SendMessage(pWnd->m_hWndListBox, LB_GETCURSEL, 0, 0L); 
			CHListItem *pItem = (CHListItem *)SendMessage(pWnd->m_hWndListBox, LB_GETITEMDATA, wItemNum, 0L);
			pWnd->OnActionItem(pItem, wItemNum); 
			return TRUE;
		}
		break; 

	case WM_MEASUREITEM:
		pWnd->OnMeasureItem((MEASUREITEMSTRUCT*)(lParam)); 
		return TRUE;

	case WM_DRAWITEM:
		pWnd->OnDrawItem((DRAWITEMSTRUCT*)(lParam)); 
	return FALSE; 

	case WM_SETFONT:
		pWnd->DrawSetTextHeight((HFONT)wParam); 
		break; 

	case WM_COMMAND:
		wmId = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		switch (wmId)
		{
		case KHL_IDLIST: 
			if(wmEvent == LBN_SELCHANGE) { 
				WORD wItemNum = (WORD) SendMessage(hwndCtl, LB_GETCURSEL, 0, 0L);
				CHListItem *pItem = (CHListItem *)SendMessage(hwndCtl, LB_GETITEMDATA, wItemNum, 0L);
				command_onselect = pWnd->OnSelectItem(pItem, wItemNum);
				
			} 
			if(wmEvent == LBN_DBLCLK || (command_onselect & HLI_DO_ACTION_ONSELECT)) { 
				WORD wItemNum = (WORD) SendMessage(hwndCtl, LB_GETCURSEL, 0, 0L);
				CHListItem *pItem = (CHListItem *)SendMessage(hwndCtl, LB_GETITEMDATA, wItemNum, 0L);
				pWnd->OnActionItem(pItem, wItemNum);
				
			} 
			if((command_onselect & HLI_DO_INVALIDATE_ONSELECT) != 0) { 
				pWnd->Invalidate(); 
			} 
			
			break; 
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
} 

LRESULT CALLBACK CHierarchyList::ListBoxSubclassWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	int wmId = 0; 
	int wmEvent = 0; 
	int nMouseXPos = 0; 
	int nMouseYPos = 0; 

	HWND hParent = (HWND)GetWindowLongPtr(hWnd, GWLP_HWNDPARENT); 
	CHierarchyList *pWnd = 0; 
	if(hParent) 
	{ 
		pWnd = (CHierarchyList *)GetWindowLongPtr(hParent, 0); 
	} 

	switch (message) 
	{
	case WM_MOUSEMOVE: 
		nMouseXPos = LOWORD(lParam); 
		nMouseYPos = HIWORD(lParam); 
//		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Mouse X-Pos <%d>, Y-Pos <%d>\n"), nMouseXPos, nMouseYPos)); 
		if(pWnd) 
		{ 
			pWnd->m_nMouseXPos = nMouseXPos; 
			pWnd->m_nMouseYPos = nMouseYPos; 
		} 

		break; 

	case WM_HSCROLL: 
	case WM_VSCROLL: 
		if(pWnd) { 
			if(pWnd->m_bEditBoxActive) { 
				ShowWindow(pWnd->m_hWndEditBox, SW_HIDE); 
			} 
		} 
		break; 

	case WM_COMMAND: 
		wmId = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		switch (wmId) 
		{ 
		case KHL_IDEDIT: 
			if(wmEvent == EN_CHANGE) 
			{ 
				ACE_DEBUG((LM_DEBUG, ACE_TEXT("Edit EN_CHANGE\n"))); 

				++pWnd->m_bEditBoxChanges; 
				if(pWnd->m_bEditBoxChanges == 2) { 
					ShowWindow(pWnd->m_hWndButton, SW_SHOWNORMAL); 
				} 
			} 
			else 
			if(wmEvent == EN_KILLFOCUS) 
			{ 
				ACE_DEBUG((LM_DEBUG, ACE_TEXT("Edit EN_KILLFOCUS\n"))); 

				if(pWnd) { 
					TCHAR szText[512]; 
					SendMessage(pWnd->m_hWndEditBox, WM_GETTEXT, (WPARAM)512, (LPARAM)szText); 
					pWnd->m_pSelectedItem->SetText(pWnd->m_nSelectedCell - 1, szText); 

					SetWindowPos(pWnd->m_hWndEditBox, HWND_BOTTOM, 0, 0, 0, 0, 0); 
//					SetWindowText(pWnd->m_hWndEditBox, TEXT("")); 
					ShowWindow(pWnd->m_hWndEditBox, SW_HIDE); 
					pWnd->m_bEditBoxActive = false; 
					pWnd->m_bEditBoxChanges = 0; 
				} 
			} 
			else 
			{ 
				ACE_DEBUG((LM_DEBUG, ACE_TEXT("Edit Event %d\n"), (int)wmEvent)); 
			} 

			return 0; 

		case IDBUTTON: 
			if(pWnd) 
			{ 
				PostMessage(GetParent(pWnd->m_hWnd), WM_COMMAND, MAKEWPARAM(pWnd->m_wCtrlId, wmEvent), 0L);

				pWnd->m_bEditBoxChanges = 0; 
				ShowWindow(pWnd->m_hWndButton, SW_HIDE); 
			} 
		} 

		break; 
	} 

	if(pWnd) 
	{ 
		if(hWnd == pWnd->m_hWndListBox && pWnd->ListBoxWndProc) 
		{ 
//			#if (defined (__BORLANDC__))
			#ifdef __BORLANDC__
			FARPROC proc = (FARPROC)pWnd->ListBoxWndProc; 
			return CallWindowProc(proc, hWnd, message, wParam, lParam); 
			#else
			return CallWindowProc(pWnd->ListBoxWndProc, hWnd, message, wParam, lParam); 
			#endif
		} 

		if(hWnd == pWnd->m_hWndCntxt && pWnd->ContextWndProc) 
		{ 
//			#if (defined (__BORLANDC__))
			#ifdef __BORLANDC__
			FARPROC proc = (FARPROC)pWnd->ContextWndProc; 
			return CallWindowProc(proc, hWnd, message, wParam, lParam); 
			#else
			return CallWindowProc(pWnd->ContextWndProc, hWnd, message, wParam, lParam); 
			#endif
		} 
	} 

	return 0; 
} 

#endif //HIERARCHYLIST_IMPL 
