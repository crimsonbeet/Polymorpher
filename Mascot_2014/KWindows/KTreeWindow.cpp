
#include "stdafx.h"
#include "KTreeWindow.h"
#include "math.h"


KTreeWindow::KTreeWindow() { 
	_debug = FALSE; 
	_noheaders = FALSE; 

	_hinst = NULL; 
	_line_heightfactor = 1.1; 
	_preallocated_items = 0; 

	m_bEditBoxActive = false; 
	m_bEditBoxChanges = 0; 

	m_bLines = TRUE; 

	m_pSelectedItem = 0; 

	SetRectEmpty(&m_rLastHyperlinkRect); 

	m_nMouseXPos = 0; 
	m_nMouseYPos = 0; 

	ListBoxWndProc = 0; 

	m_nTextHeight = 0; 
	m_nLineHeight = 0; 

	_hwndListBox = NULL; 
	_hwndEditBox = NULL; 

	_hwndCntxt = NULL; 

	m_hdcMem = NULL; 
	m_hbmMem = NULL; 

	m_nBitmapHeight = 0; 
	m_nBitmapWidth = 0; 
} 

KTreeWindow::~KTreeWindow() { 
} 

VOID KTreeWindow::FastRect(HDC hDC, int x, int y, int cx, int cy) { 
	RECT rc;

	rc.left = x;
	rc.right = x + cx;
	rc.top = y;
	rc.bottom = y + cy;

	ExtTextOut(hDC, x, y, ETO_OPAQUE, &rc, NULL, 0, NULL); 
}

void KTreeWindow::KHL_DEBUG(const std::string& format, ...) { 
	if(_debug) { 
		char buffer[1024]; 

	    va_list ap; 

		va_start(ap, format); 
		_vsnprintf(buffer, 1024, format.c_str(), ap); 

		std::cout << buffer; 
	} 
} 

void KTreeWindow::Create(HINSTANCE hInst, HWND hParent) { 
	_hinst = hInst; 
	if(_ctl.classname.size() == 0) { 
		_ctl.classname = "KTreeWindow"; 
	} 

	CreateEx(0, WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN/* | WS_BORDER*/, hParent, NULL, _hinst); 
} 

/* 
 * called by KTreeWindow::WndProc. 
 */
void KTreeWindow::OnCreate(HWND hMe) { 
	_hwnd = hMe; 

	HDC hDC = GetDC(NULL); 
	m_hdcMem = CreateCompatibleDC(hDC); 
	ReleaseDC(NULL, hDC); 

	DrawInit(); 

	_hwndListBox = CreateWindow("LISTBOX", NULL, WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_BORDER | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_WANTKEYBOARDINPUT, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, _hwnd, (HMENU)KHL_IDLIST, _hinst, NULL); 

	if(_hwndListBox) { 
		_hwndEditBox = CreateWindow("EDIT", NULL, WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER, 
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, _hwndListBox, (HMENU)KHL_IDEDIT, _hinst, NULL); 

		SetWindowPos(_hwndEditBox, NULL, 0, 0, 0, 0, SWP_NOZORDER); 

		if(_preallocated_items > 0) { 
			SendMessage(_hwndListBox, LB_INITSTORAGE, _preallocated_items, 8); 
		} 

		ListBoxWndProc = (WNDPROC)SetWindowLongPtr(_hwndListBox, GWL_WNDPROC, (LONG_PTR)ListBoxSubclassWndProc); 
	} 

	if(_format_list.size()) { 
		if(!_noheaders) { 
			AddRootItem(new CHListHeader(&_format_list[0])); 
		} 
	} 
} 

/* 
 * called by CFrame::OnCommand. 
 */
BOOL KTreeWindow::OnCommand(WPARAM wParam, LPARAM lParam) { 
	return FALSE;	// not processed
}




BOOL KTreeWindow::DrawInit() { 
	HDC hDC = NULL; 

	m_hbmMem = NULL; 
	m_nBitmapHeight = 0; 
	m_nBitmapWidth = 0; 

	if(!m_hdcMem) { 
		return FALSE; 
	} 

	if(_bitmapctl.bitmap_rows == 0) { 
		return FALSE; 
	} 

	if(_bitmapctl.bitmap_columns == 0) { 
		return FALSE; 
	} 

	TEXTMETRIC TextMetrics; 

	hDC = GetDC(_hwndListBox); 
	GetTextMetrics(hDC, &TextMetrics); 
	ReleaseDC(_hwndListBox, hDC); 

	m_nTextHeight = TextMetrics.tmHeight + TextMetrics.tmExternalLeading; 

	// Tree icons bitmap is assumed to be a 16 color DIB! 

	LPBITMAPINFOHEADER lpbi = 0; 

	if(_bitmapctl.id) { 
		HRSRC hRes = FindResource(_hinst, MAKEINTRESOURCE(_bitmapctl.id), RT_BITMAP); 
		if(hRes) { 
			HGLOBAL hResMem = LoadResource(_hinst, hRes); 
			lpbi = (LPBITMAPINFOHEADER)LockResource(hResMem); 
		} 
	} 
	else 
	if(_bitmapctl.file_name.size()) { 
		HANDLE hbm = LoadImage(NULL, _bitmapctl.file_name.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED | LR_CREATEDIBSECTION); 
		if(hbm) { 
			DIBSECTION bm; 
			if(GetObject(hbm, sizeof(DIBSECTION), &bm)) { 
				lpbi = &bm.dsBmih; 
			} 
		} 
	} 

	// Now figure out the bitmaps background color.
	// This code assumes the these are 16 color bitmaps
	// and that the lower left corner is a bit in the background
	// color.
	if(!lpbi) { 
		return FALSE; 
	} 

	m_nBitmapHeight = (LONG)lpbi->biHeight / _bitmapctl.bitmap_rows; 
	m_nBitmapWidth = (LONG)lpbi->biWidth / _bitmapctl.bitmap_columns; 

	m_nLineHeight = (LONG)(max(m_nBitmapHeight, m_nTextHeight) * _line_heightfactor); 

	PDWORD lpColorTable = (PDWORD)(lpbi + 1); 
	LPSTR lpBits = (LPSTR)(lpColorTable + 16); // ASSUMES 16 COLOR
	int bc = (lpBits[0] & 0xF0) >> 4; // ASSUMES LOWER LEFT CORNER IS BACKGROUND!!!

	BYTE lpbi_container[sizeof(BITMAPINFO) + 16 * sizeof(DWORD)]; 
	LPBITMAPINFO lpbi_newcolor = (LPBITMAPINFO)lpbi_container; 
	memcpy(lpbi_newcolor, lpbi, sizeof(lpbi_container)); 

	*(PDWORD)&lpbi_newcolor->bmiColors[bc] = RGB2BGR(GetSysColor(COLOR_WINDOW)); 

	hDC = GetDC(NULL); 
	HBITMAP hbmIcons = CreateDIBitmap(hDC, &lpbi_newcolor->bmiHeader, (DWORD)CBM_INIT, lpBits, lpbi_newcolor, DIB_RGB_COLORS); 
	ReleaseDC(NULL, hDC); 

	if(!hbmIcons) { 
		return FALSE; 
	} 

	m_hbmMem = (HBITMAP)SelectObject(m_hdcMem, hbmIcons); 
	return m_hbmMem != NULL; 
} 

VOID KTreeWindow::DrawSetTextHeight(HFONT hFont) 
{
	TEXTMETRIC TextMetrics; 

	HDC hdc = GetDC(_hwndListBox);
	SelectObject(hdc, hFont); 
	GetTextMetrics(hdc, &TextMetrics);
	ReleaseDC(_hwndListBox, hdc);

	m_nTextHeight = TextMetrics.tmHeight; 
	m_nLineHeight = (LONG)(max(m_nBitmapHeight, m_nTextHeight) * _line_heightfactor); 

	SendMessage(_hwndListBox, LB_SETITEMHEIGHT, 0, MAKELPARAM(m_nLineHeight, 0)); 
	SendMessage(_hwndListBox, WM_SETFONT, (WPARAM)hFont, 0); 
}

BOOL KTreeWindow::ClearLastHyperLinkSelection() { 
	BOOL ok = FALSE; 
	if(!IsRectEmpty(&m_rLastHyperlinkRect)) { 
		InvalidateRect(_hwndListBox, &m_rLastHyperlinkRect, FALSE); 
		SetRectEmpty(&m_rLastHyperlinkRect); 
		ok = TRUE; 
	} 

	return ok; 
} 


void KTreeWindow::Invalidate() { 
	if(!_hwnd) 
	{ 
		return; 
	} 

	if(!_hwndListBox) 
	{ 
		return; 
	} 

	RECT rc;
	GetClientRect(_hwnd, &rc); 
	SetWindowPos(_hwndListBox, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER); 
	InvalidateRect(_hwndListBox, NULL, TRUE); 
} 

void KTreeWindow::Clear() { 
	if(_hwndListBox) { 
		SendMessage(_hwndListBox, LB_RESETCONTENT, 0, 0L); 
	} 
}

int KTreeWindow::AddRootItem(CHListItem *pItem) { 
	if(pItem == 0) { 
		return 0; 
	} 

	LRESULT lResult = LB_ERR; 
	if(_hwndListBox) { 
		pItem->AddRef(); 
		lResult = SendMessage(_hwndListBox, LB_ADDSTRING, 0, (LPARAM)pItem); 
		if(lResult == LB_ERR || lResult == LB_ERRSPACE) { 
			pItem->Release(); 
		} 
	} 

	int rc = (lResult == LB_ERR || lResult == LB_ERRSPACE)? 0: ((int)lResult + 1); 
	if(rc > 0) { 
		if(pItem->m_pFormat == 0) { 
			pItem->m_pFormat = GetFormatItem(0); 
		} 
	} 

	return rc; 
} 

int KTreeWindow::RedrawItem(CHListItem *pItem) { 
	if(pItem == 0) { 
		return 0; 
	} 

	LRESULT lResult = LB_ERR; 
	if(_hwndListBox) { 
		lResult = SendMessage(_hwndListBox, LB_FINDSTRING, (WPARAM)-1, (LPARAM)pItem); 
		if(lResult != LB_ERR) { 
			LONG wItem = (LONG)lResult; 

			LONG wBottomIndex;
			LONG wTopIndex;
			RECT rc;

			wTopIndex = (WORD)SendMessage(_hwndListBox, LB_GETTOPINDEX, 0, 0L);
			GetClientRect(_hwndListBox, &rc); 
			wBottomIndex = wTopIndex + (rc.bottom + 1) / m_nLineHeight; 
			if(wItem < wTopIndex) { 
			} 
			else 
			if(wItem > wBottomIndex) { 
			} 
			else { 
				rc.top += (wItem - wTopIndex) * m_nLineHeight; 
				rc.bottom = rc.top + m_nLineHeight; 
				InvalidateRect(_hwndListBox, &rc, FALSE); // Force redraw
			} 
		} 
	} 

	int rc = (lResult == LB_ERR)? 0: ((int)lResult + 1); 
	return rc; 
} 

CHFormatItem * KTreeWindow::GetFormatItem(BYTE nbLevel) { 
	if(nbLevel < _format_list.size()) { 
		return &_format_list[nbLevel]; 
	} 

	return NULL; 
} 


VOID KTreeWindow::OnMeasureItem(MEASUREITEMSTRUCT *lpMeasureItem) { 
	lpMeasureItem->itemHeight = m_nLineHeight; 
} 

VOID KTreeWindow::OnLbMouseMove(LPARAM lParam) { 
	bool hoover_ok = false; 

	m_nMouseXPos = LOWORD(lParam); 
	m_nMouseYPos = HIWORD(lParam); 

//	KHL_DEBUG("List Box Mouse X-Pos <%d>, Y-Pos <%d>\n", (int)LOWORD(lParam), (int)HIWORD(lParam)); 

	LRESULT lResult = SendMessage(_hwndListBox, LB_ITEMFROMPOINT, NULL, lParam); 
	if(HIWORD(lResult) == 0) { 
		WORD wIndex = LOWORD(lResult); 
		RECT item_rect; 
		if(LB_ERR != SendMessage(_hwndListBox, LB_GETITEMRECT, wIndex, (LPARAM)&item_rect)) { 
			CHListItem *pItem = (CHListItem *)SendMessage(_hwndListBox, LB_GETITEMDATA, wIndex, 0L); 
			if(LB_ERR != (int)pItem) { 

				CHListItemDrawCtl ctl; 
				BuildDrawItemCtl(pItem/*in*/, item_rect/*in*/, ctl/*out*/); 
				if(ctl.nSelectedCell != 0) { 

					BYTE bIsHyperlink[32]; 
					pItem->GetIsHyperlink(bIsHyperlink); 
					if(bIsHyperlink[ctl.nSelectedCell - 1]) { 

						if(!EqualRect(&m_rLastHyperlinkRect, &item_rect)) { 
							ClearLastHyperLinkSelection(); 
							InvalidateRect(_hwndListBox, &item_rect, FALSE); // Force redraw
						} 
						hoover_ok = true; 
					} 
				} 
			} 
		} 
	} 

	if(!hoover_ok) { 
		ClearLastHyperLinkSelection(); 
	} 
} 

BOOL KTreeWindow::OnSelectItem(CHListItem *pItem, DWORD dwItemNum) { 
	if(pItem == 0) { 
		return FALSE; 
	} 

	BOOL do_action = FALSE; 

	if(m_pSelectedItem == pItem) { 
//		KHL_DEBUG("OnSelect Item <%s>\n", pItem->GetText().c_str()); 
		for(int j = 0; j < 32; ++j) { 
			if(	m_nMouseXPos >= m_nSelItemsOuterBoxes[j].left &&
				m_nMouseXPos <  m_nSelItemsOuterBoxes[j].right) { 
					do_action = pItem->OnClick(_hwndCntxt, j + 1); 

					if(	m_nSelectedCell != j + 1) { 
						pItem->OnSelect(_hwndCntxt, m_nSelectedCell = j + 1); 
					} 

					BYTE nbEditFlags[32]; 
					pItem->GetColumnsEditFlags(nbEditFlags); 
					if(nbEditFlags[j] > 0) { 
						SetWindowPos(_hwndEditBox, HWND_TOP, 
							m_nSelItemsInnerBoxes[j].left, 
								m_nSelItemsInnerBoxes[j].top, 
									m_nSelItemsInnerBoxes[j].right - m_nSelItemsInnerBoxes[j].left, 
										m_nSelItemsInnerBoxes[j].bottom - m_nSelItemsInnerBoxes[j].top, 0); 
						SetWindowText(_hwndEditBox, pItem->GetText(m_nSelectedCell - 1).c_str()); 
						::ShowWindow(_hwndEditBox, SW_SHOWNORMAL); 
						PostMessage(_hwndEditBox, WM_LBUTTONDOWN, MK_LBUTTON, m_nMouseXPos - m_nSelItemsInnerBoxes[j].left);
						PostMessage(_hwndEditBox, WM_LBUTTONUP, MK_LBUTTON, m_nMouseXPos - m_nSelItemsInnerBoxes[j].left);
//						SetFocus(_hwndEditBox); 
						m_bEditBoxActive = true; 
					} 
			} 

			if(	m_nSelItemsOuterBoxes[j].right == 0) { 
				break; 
			} 

			InvalidateRect(_hwndListBox, &m_nSelItemsOuterBoxes[j], TRUE);
		} 
	} 
	else { 
		KHL_DEBUG("OnSelect Item <%s> is called for the item not being selected\n;current selected item is <%s>", pItem->GetText().c_str(), m_pSelectedItem? m_pSelectedItem->GetText().c_str(): "NULL"); 
	} 

	return do_action; 
} 

VOID KTreeWindow::OnActionItem(CHListItem *pItem, DWORD dwItemNum) { 
	if(pItem == 0) { 
		return; 
	} 

	if(pItem->m_nbHasChild) { 
		if(pItem->m_nbOpened) { // It's open ... Close it 
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

			CHListItem *pChildItem = (CHListItem *)SendMessage(_hwndListBox, LB_GETITEMDATA, dwItemNum, 0L); 
			if(pChildItem) { 
				nbLevel = pChildItem->m_nbLevel;  
			} 

			while(nbLevel > pItem->m_nbLevel) { 
				pItem->m_nbOpened = 0; 
				SendMessage(_hwndListBox, LB_DELETESTRING, dwItemNum, 0L); 
				pChildItem = (CHListItem *)SendMessage(_hwndListBox, LB_GETITEMDATA, dwItemNum, 0L); 
				if(pChildItem == (CHListItem *)LB_ERR) { 
					nbLevel = pItem->m_nbLevel; 
				} 
				else 
				if(pChildItem) { 
					nbLevel = pChildItem->m_nbLevel;  
				} 
			} 
		} 
		else {
			//
			// It's closed ... Open it
			//
			pItem->m_nbOpened = 1; 

			SendMessage(_hwndListBox, WM_SETREDRAW, FALSE, 0L);   // Disable redrawing. 

			BOOL header_done = _noheaders; 
			
			DWORD dwItem = dwItemNum + 1; 

			CHListItem *pChildItem = pItem->FirstChild(); 
			if(!pChildItem) { 
				pItem->m_nbHasChild = 0; 
			} 
			else { 
				do { 
					CHListItem *pNextChildItem = 0; 
					if(!header_done) { 
						header_done = TRUE; 
						if(_format_list.size() < (size_t)(pItem->m_nbLevel + 1)) { 
							pNextChildItem = pChildItem; 
							pChildItem = new CHListHeader(&_format_list[pItem->m_nbLevel + 1]); 
						} 
					} 
					if(!pNextChildItem) { 
						pNextChildItem = pItem->GetNextChild(); 
					} 

					pChildItem->m_nbLevel = pItem->m_nbLevel + 1; 
					pChildItem->m_dwMask = pItem->m_dwMask; 

					if(pNextChildItem) { 
						pChildItem->m_dwMask |= (DWORD)pow(2, (DWORD)pChildItem->m_nbLevel - 1); 
					} 

					if(pChildItem->m_pFormat == 0) { 
						pChildItem->m_pFormat = GetFormatItem(pChildItem->m_nbLevel); 
					} 

					pChildItem->AddRef(); 
					LRESULT lResult = SendMessage(_hwndListBox, LB_INSERTSTRING, dwItem, (LPARAM)pChildItem); 
					if(lResult == LB_ERR || lResult == LB_ERRSPACE) { 
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

			wTopIndex = (WORD)SendMessage(_hwndListBox, LB_GETTOPINDEX, 0, 0L);
			GetClientRect(_hwndListBox, &rc); 
			wBottomIndex = wTopIndex + (rc.bottom + 1) / m_nLineHeight; 

			wExpandInView = (wBottomIndex - dwItemNum); 

			if(wKids >= wExpandInView) { 
				wNewTopIndex = min((LONG)dwItemNum, (wTopIndex + wKids + 1) - wExpandInView); 
				SendMessage(_hwndListBox, LB_SETTOPINDEX, (DWORD)wNewTopIndex, 0L); 
			} 

			SendMessage(_hwndListBox, WM_SETREDRAW, TRUE, 0L); // Enable redrawing.
			InvalidateRect(_hwndListBox, NULL, TRUE); // Force redraw
		} 
	} 

	pItem->OnAction(_hwndCntxt); 
} 



VOID KTreeWindow::BuildDrawItemCtl(CHListItem *pItem/*in*/, RECT& item_rect/*in*/, CHListItemDrawCtl& ctl/*out*/) { 
	pItem->GetColumnsOffsets(ctl.nUnits, ctl.nColumns); 

	CopyRect(&ctl.rcTemp, &item_rect); 

	ctl.nLevel = pItem->m_nbLevel;

//	Figure out which connecting lines are needed.
	if(ctl.nLevel == 0) {
		ctl.dwConnectLevel = 0; 
	} 
	else { 
		ctl.dwConnectLevel = pItem->m_dwMask; 
	} 

	ctl.wIndent = ctl.rcTemp.left + (ctl.nLevel * m_nBitmapWidth) + KHL_XBMPOFFSET; 
	ctl.wAbsIndent = 0; 
	if(!m_bLines) { 
		ctl.wIndent = ctl.rcTemp.left + m_nBitmapWidth + KHL_XBMPOFFSET; 
	} 

	ctl.wTopText = (LONG)(ctl.rcTemp.top + ((ctl.rcTemp.bottom - ctl.rcTemp.top) / 2.0) - (m_nTextHeight / 2.0)); 
	ctl.wTopBitmap = (LONG)(ctl.rcTemp.top + ((ctl.rcTemp.bottom - ctl.rcTemp.top) / 2.0) - (m_nBitmapHeight / 2.0)); 

	ctl.wTotalWidth = ctl.rcTemp.right - ctl.rcTemp.left - ctl.wIndent; 

	if(ctl.nUnits == 0/*Pos. in Pix.*/) { 
//		Turn absolute positions to width. 
		int j; 
		if(ctl.nColumns[0] != 0) { 
			ctl.wAbsIndent += ctl.nColumns[0]; 
			ctl.wIndent += ctl.wAbsIndent; 
			ctl.wTotalWidth = ctl.rcTemp.right - ctl.rcTemp.left - ctl.wIndent; 
		} 
		
		for(j = 0; j < 31 && (ctl.nColumns[j] != 0 || ctl.nColumns[j + 1] != 0); ++j) { 
			if(ctl.nColumns[j + 1] == 0) { 
				ctl.nColumns[j] = ctl.rcTemp.right - ctl.nColumns[j]; 
			} 
			else { 
				ctl.nColumns[j] = ctl.nColumns[j + 1] - ctl.nColumns[j]; 
			} 
		} 

		ctl.nColumns[j + 1] = 0; 
	} 
	else 
	if(ctl.nUnits == 2/*Percents*/) { 
//		Turn percents to actual width. 
		int j; 
		for(j = 0; j < 32 && ctl.nColumns[j] != 0; ++j) { 
			ctl.nColumns[j] = (int)floor(((double)ctl.nColumns[j]/100.0) * ctl.wTotalWidth + 0.5); 
			if(ctl.nColumns[j] > ctl.wTotalWidth) { 
				ctl.nColumns[j] = 0; 
			} 
		} 
	} 

//	Find selected cell. 
	int left = ctl.wIndent; 
	int j; 
	for(j = 0; j < 32 && ctl.nColumns[j] != 0; ++j) { 
		int right = left + ctl.nColumns[j]; 
		if(m_nMouseXPos >= left && m_nMouseXPos < right) { 
			ctl.nSelectedCell = j + 1; 
			break; 
		} 

		left = right; 
	} 
} 


VOID KTreeWindow::OnDrawItem(DRAWITEMSTRUCT * lpDrawItem) 
{
	if(lpDrawItem->itemID == (UINT)-1) { 
		return; 
	} 

	CHListItem *pItem = (CHListItem *)lpDrawItem->itemData; 
	if(!pItem) { 
		return; 
	} 

//	KHL_DEBUG("draw item <%s>\n", pItem->GetText().c_str()); 

	std::string szText; 

	HDC hDC = lpDrawItem->hDC; 


	CHListItemDrawCtl ctl; 
	BuildDrawItemCtl(pItem/*in*/, lpDrawItem->rcItem/*in*/, ctl/*out*/); 

	int& nUnits = ctl.nUnits; 
	int nColumns[32]; 
	memcpy(nColumns, ctl.nColumns, sizeof(int) * 32); 
	RECT& rcTemp = ctl.rcTemp; 
	int& nLevel = ctl.nLevel;
	DWORD& dwConnectLevel = ctl.dwConnectLevel; 
	LONG& wIndent = ctl.wIndent; 
	LONG& wAbsIndent = ctl.wAbsIndent; 
	LONG& wTopText = ctl.wTopText; 
	LONG& wTopBitmap = ctl.wTopBitmap; 
	LONG& wTotalWidth = ctl.wTotalWidth; 
	int& nSelectedCell = ctl.nSelectedCell; 


	BOOL bDrawLinesForOuterLevels = TRUE; 
	BOOL bDrawSelectedBitmap = TRUE; 
	BOOL bDrawText = TRUE; 

	if(lpDrawItem->itemAction == ODA_FOCUS) { 
		bDrawLinesForOuterLevels = FALSE; 
		bDrawSelectedBitmap = FALSE; 
		if((lpDrawItem->itemState & ODS_FOCUS)) { 
//			KHL_DEBUG("got focus Item <%s> l=%d r=%d t=%d b=%d\n", pItem->GetText().c_str(), rcTemp.left, rcTemp.right, rcTemp.top, rcTemp.bottom); 
		} 
		else { 
//			KHL_DEBUG("lost focus Item <%s> l=%d r=%d t=%d b=%d\n", pItem->GetText().c_str(), rcTemp.left, rcTemp.right, rcTemp.top, rcTemp.bottom); 
		} 
	} 
//	else 
	if(lpDrawItem->itemAction == ODA_SELECT) { 
		bDrawLinesForOuterLevels = FALSE; 
		bDrawSelectedBitmap = FALSE; 
		if((lpDrawItem->itemState & ODS_SELECTED)) { 
//			KHL_DEBUG("got selection Item <%s>\n", pItem->GetText().c_str()); 
			if(m_pSelectedItem) { 
				m_pSelectedItem->OnLostSelect(_hwndCntxt); 
			} 

			pItem->OnSelect(_hwndCntxt, m_nSelectedCell = nSelectedCell); 
			m_pSelectedItem = pItem; 
		} 
		else { 
//			KHL_DEBUG("lost selection Item <%s>\n", pItem->GetText().c_str()); 
			pItem->OnLostSelect(_hwndCntxt); 
			m_pSelectedItem = 0; 
		} 

	} 

//	Draw a series of | lines for outer levels

	if(bDrawLinesForOuterLevels && m_bLines && nLevel) {
		// draw lines in text color
		SetBkColor(hDC, GetSysColor(COLOR_WINDOWTEXT)); 

		DWORD dwMask = 1; 

		//
		// Draw a series of | lines for outer levels
		//
		int x;
		x = m_nBitmapWidth / 2 + KHL_XBMPOFFSET; 

		int j; 
		for(j = 0; j < nLevel; ++j) {
			if(dwConnectLevel & dwMask) { 
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


	int bBorders[32]; 
	pItem->GetColumnsBorders(bBorders); 

	BYTE bCanHighlight[32]; 
	pItem->GetCanHighlight(bCanHighlight); 

	BYTE bIsHyperlink[32]; 
	pItem->GetIsHyperlink(bIsHyperlink); 

	DWORD dwBkColor[32]; 
	DWORD dwTextColor[32]; 
	pItem->GetColumnsColors(dwBkColor, dwTextColor); 

	int nBMRow = 0; 
	int nBMColumns[32] = { 0 }; 
	pItem->GetBitMapIndices(pItem->m_nbOpened, nBMRow, nBMColumns); 

	LONG wBitmapXPos = wIndent; 

	for(int j = 0; j < 32 && (nColumns[j] != 0 || nBMColumns[j] != 0); ++j) { 
		pItem->GetText(j, szText); 

		BOOL bDrawBitmap = bDrawSelectedBitmap; 
		if(nBMColumns[j] == 0 || nBMRow == 0 || m_hdcMem == 0) { 
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
		if(GetTextExtentPoint(hDC, szText.c_str(), szText.size(), &strSize)) { 
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
			HFONT hfont = 0; 
		    HGDIOBJ holdfont = 0;
			if(bIsHyperlink[j]) { 
				POINT pt; 
				pt.x = m_nMouseXPos; 
				pt.y = m_nMouseYPos; 
				if(PtInRect(&rcTemp, pt)) { 
					hfont = MyCreateFont(hDC, _ctl.font_face, _ctl.font_height, FALSE, TRUE, TRUE); 
					SetTextColor(hDC, GetSysColor(COLOR_HOTLIGHT)); 

					m_rLastHyperlinkRect = rcTemp; 
				} 
			} 
			if(hfont) { 
				holdfont = SelectObject(hDC, hfont);
			} 
//			KHL_DEBUG("draw text Item <%s> l=%d r=%d t=%d b=%d\n", szText.c_str(), rcTemp.left, rcTemp.right, rcTemp.top, rcTemp.bottom); 
			if(pItem->IsRightAlignment(j)) { 
				ExtTextOut(hDC, rcTemp.right - strSize.cx - 1, wTopText, ETO_CLIPPED|ETO_OPAQUE, 
					&rcTemp, szText.c_str(), szText.size(), NULL);
			} 
			else { 
				ExtTextOut(hDC, rcTemp.left + 1, wTopText, ETO_CLIPPED|ETO_OPAQUE, 
					&rcTemp, szText.c_str(), szText.size(), NULL);
			} 

			if(holdfont) { 
			    SelectObject(hDC, holdfont); 
			} 
			if(hfont) { 
			    DeleteObject(hfont); 
			} 
		} 

		if(bBorders[j]) { 
			FrameRect(hDC, &rcTemp, (HBRUSH)GetStockObject(LTGRAY_BRUSH)); 
		} 
	} 
} 

/* 
 * CALLED: By KWindow::WindowProc. 
 * PURPOSE: Processes messages for the main window. 
 */
LRESULT KTreeWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	KTreeWindow *pWnd = this; 

	int wmId; 
	int wmEvent; 
	PAINTSTRUCT ps;
	HDC hdc;

	HWND hwndCtl = (HWND)lParam; 

	BOOL command_onselect = FALSE; 

	switch (message) {
		case WM_CREATE: { 
			pWnd->OnCreate(hWnd); 
		} 
		return 0; 

		case WM_SIZE:
			MoveWindow(pWnd->_hwndListBox, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);	
		return 0; 

		case WM_NCDESTROY: 
		return 0; 

		case WM_DELETEITEM: 
			wmId = LOWORD(wParam); 
			if(wmId == KHL_IDLIST) {
				LPDELETEITEMSTRUCT lpDeleteItem = (LPDELETEITEMSTRUCT) lParam; 
				CHListItem *pItem = (CHListItem *)lpDeleteItem->itemData; 
				if(pItem) { 
					pItem->Release(); 
				} 
				return TRUE;
			} 
		break; 

		case WM_SYSCOLORCHANGE:
			pWnd->DrawInit();
		return TRUE;

		case WM_MOUSEMOVE: 
//			KHL_DEBUG("Mouse X-Pos <%d>\n", (int)LOWORD(lParam)); 
		return TRUE; 

		case WM_CHARTOITEM:
			if(wParam == VK_RETURN) { 
				WORD wItemNum = (WORD)SendMessage(pWnd->_hwndListBox, LB_GETCURSEL, 0, 0L); 
				CHListItem *pItem = (CHListItem *)SendMessage(pWnd->_hwndListBox, LB_GETITEMDATA, wItemNum, 0L);
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
			switch (wmId) { 
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

		default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
} 

LRESULT CALLBACK KTreeWindow::ListBoxSubclassWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	int wmId = 0; 
	int wmEvent = 0; 

	HWND hParent = (HWND)GetWindowLongPtr(hWnd, GWLP_HWNDPARENT); 
	KTreeWindow *pWnd = 0; 
	if(hParent) { 
		pWnd = (KTreeWindow *)(LONG_PTR)GetWindowLong(hParent, GWL_USERDATA); 
//		pWnd = (KTreeWindow *)GetWindowLongPtr(hParent, 0); 
	} 

	switch (message) {
	case WM_MOUSEMOVE: 
		if(pWnd) { 
			pWnd->OnLbMouseMove(lParam); 
		} 

		break; 

	case WM_HSCROLL: 
	case WM_VSCROLL: 
		if(pWnd) { 
			if(pWnd->m_bEditBoxActive) { 
				::ShowWindow(pWnd->_hwndEditBox, SW_HIDE); 
			} 
		} 
		break; 

	case WM_COMMAND: 
		wmId = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		switch (wmId) { 
		case KHL_IDEDIT: 
			if(wmEvent == EN_CHANGE) { 
				pWnd->KHL_DEBUG("Edit EN_CHANGE\n"); 

				++pWnd->m_bEditBoxChanges; 
			} 
			else 
			if(wmEvent == EN_KILLFOCUS) { 
				pWnd->KHL_DEBUG("Edit EN_KILLFOCUS\n"); 

				if(pWnd) { 
					CHAR szText[512]; 
					SendMessage(pWnd->_hwndEditBox, WM_GETTEXT, (WPARAM)512, (LPARAM)szText); 
					pWnd->m_pSelectedItem->SetText(pWnd->m_nSelectedCell - 1, szText); 

					SetWindowPos(pWnd->_hwndEditBox, HWND_BOTTOM, 0, 0, 0, 0, 0); 
//					SetWindowText(pWnd->_hwndEditBox, TEXT("")); 
					::ShowWindow(pWnd->_hwndEditBox, SW_HIDE); 
					pWnd->m_bEditBoxActive = false; 
					pWnd->m_bEditBoxChanges = 0; 
				} 
			} 
			else { 
				pWnd->KHL_DEBUG("Edit Event %d\n", (int)wmEvent); 
			} 

			return 0; 

		} 

		break; 
	} 

	if(pWnd) { 
		if(hWnd == pWnd->_hwndListBox && pWnd->ListBoxWndProc) { 
//			#if (defined (__BORLANDC__))
			#ifdef __BORLANDC__
			FARPROC proc = (FARPROC)pWnd->ListBoxWndProc; 
			return CallWindowProc(proc, hWnd, message, wParam, lParam); 
			#else
			return CallWindowProc(pWnd->ListBoxWndProc, hWnd, message, wParam, lParam); 
			#endif
		} 
	} 

	return 0; 
} 

