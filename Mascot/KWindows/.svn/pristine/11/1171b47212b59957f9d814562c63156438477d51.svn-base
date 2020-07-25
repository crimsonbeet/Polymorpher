
#include "stdafx.h"
#include "Hyperlist.h"
#include "math.h"


std::map<std::string, int> g_mapHtmlentities; 
BOOL InitializeHtmlentities() { 
	std::string strHtmlEntities; 
	BOOL rc = FALSE; 
	if(ReadTextFile("HTML_Entity_names2dec.csv", strHtmlEntities)) { 
		for(size_t j = 0; j < strHtmlEntities.size(); ++j) { 
			if(isdigit(strHtmlEntities[j])) { 
				size_t code_begin = j; 
				size_t code_end = j; 
				while(++j < strHtmlEntities.size() && strHtmlEntities[j] > 0x20) { 
					if(strHtmlEntities[j] == ',') { 
						code_end = j; 
					} 
				} 
				if(code_begin < code_end) { 
					int code = atoi(&strHtmlEntities[code_begin]); 
					int key_pos = (int)code_end + 1; 
					std::string key = strHtmlEntities.substr(key_pos, j - key_pos); 
					g_mapHtmlentities.insert(std::map<std::string, int>::value_type(key, code)); 
					rc = TRUE; 
				} 
			} 
		} 
	} 
	return rc; 
} 

BOOL g_mapHtmlentitiesInitialized = InitializeHtmlentities(); 


BOOL EXP_KWIN DecodeHtmlentities(std::string& szText) { 
	BOOL rc = FALSE; 
	if(g_mapHtmlentitiesInitialized) { 
		for(size_t j = 0; j < szText.size(); ++j) { 
			if(szText[j] == '&') { 
				size_t entity_begin = j; 
				while(++j < szText.size() && szText[j] > 0x20 && szText[j] != ';'); 
				if(szText[j] == ';') { 
					int key_pos = (int)entity_begin + 1; 
					std::string key = szText.substr(key_pos, j - key_pos); 
					std::map<std::string, int>::iterator it = g_mapHtmlentities.find(key); 
					if(it != g_mapHtmlentities.end()) { 
						szText[entity_begin] = (((*it).second) & 0xff); 
						szText.erase(key_pos, j + 1 - key_pos); 
						j = entity_begin; 
						rc = TRUE; 
					} 
				} 
			} 
		} 
	} 
	return rc; 
} 


void MyDestroyWindow(HWND& hwnd) { 
	if(hwnd) { 
		DestroyWindow(hwnd); 
		hwnd = 0; 
	} 
} 


KHyperlist::KHyperlist() { 
	_debug = FALSE; 
	_noheaders = FALSE; 
	_nolines = FALSE; 
	_noborder = FALSE; 

	_hinst = NULL; 
	_line_heightfactor = 1.1; 
	_preallocated_items = 0; 

	m_bEditBoxActive = false; 
	m_bEditBoxChanges = 0; 

	m_pSelectedItem = 0; 

	SetRectEmpty(&m_rLastHyperlinkRect); 
	SetRectEmpty(&m_rLastEditBoxRect); 

	m_nMouseXPos = 0; 
	m_nMouseYPos = 0; 

	m_nCellSelectionXPos = 0; 

	ListBoxWndProc = 0; 

	m_nTextHeight = 0; 
	m_nLineHeight = 0; 

	_hwndListBox = NULL; 
	_hwndEditBox = NULL; 

	_hwndCntxt = NULL; 

	m_nBitmapHeight = 0; 
	m_nBitmapWidth = 0; 
} 

KHyperlist::~KHyperlist() { 
} 

VOID KHyperlist::FastRect(HDC hDC, int x, int y, int cx, int cy) { 
	RECT rc;

	rc.left = x;
	rc.right = x + cx;
	rc.top = y;
	rc.bottom = y + cy;

	ExtTextOut(hDC, x, y, ETO_OPAQUE, &rc, NULL, 0, NULL); 
} 

VOID KHyperlist::Draw4FastLines(HDC hDC, RECT& rc, int cx, int cy) { 
	COLORREF oldcolor = GetBkColor(hDC); 
	SetBkColor(hDC, GetSysColor(COLOR_WINDOWTEXT)); 
	if(cx > 0) { 
		FastRect(hDC, /*x*/rc.left - cx, /*y*/rc.top, cx, /*cy*/rc.bottom - rc.top); 
		FastRect(hDC, /*x*/rc.right - cx, /*y*/rc.top, cx, /*cy*/rc.bottom - rc.top); 
	} 

	if(cy > 0) { 
		FastRect(hDC, /*x*/rc.left, /*y*/rc.top - cy, /*cx*/rc.right - rc.left, cy); 
		FastRect(hDC, /*x*/rc.left, /*y*/rc.bottom - cy, /*cx*/rc.right - rc.left, cy); 
	} 
	SetBkColor(hDC, oldcolor); 
} 

void KHyperlist::KHL_DEBUG(const char * format, ...) { 
	if(_debug) { 
		char buffer[1024]; 

	    va_list ap; 

		va_start(ap, format); 
		_vsnprintf(buffer, 1024, format, ap); 

		std::cout << buffer; 
	} 
} 

void KHyperlist::Create(HINSTANCE hInst, HWND hParent) { 
	_hinst = hInst; 
	if(_ctl.classname.size() == 0) { 
		_ctl.classname = "KHyperlist"; 
	} 

	CreateEx(_ctl.transparent? WS_EX_TRANSPARENT: 0, WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN/* | WS_BORDER*/, hParent, _hinst); 
} 

/* 
 * called by KHyperlist::WndProc. 
 */
void KHyperlist::OnCreate(HWND hMe) { 
	_hwnd = hMe; 

	DrawInit(); 

	_hwndListBox = CreateWindow("LISTBOX", NULL, WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_VSCROLL | (_noborder? 0: WS_BORDER) | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_WANTKEYBOARDINPUT, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, _hwnd, (HMENU)KHL_IDLIST, _hinst, NULL); 

	if(_hwndListBox) { 
		_hwndEditBox = CreateWindow("EDIT", NULL, WS_CHILD | WS_CLIPSIBLINGS | ES_LEFT | ES_AUTOHSCROLL/* | WS_BORDER*/, 
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, _hwndListBox, (HMENU)KHL_IDEDIT, _hinst, NULL); 

		SetWindowPos(_hwndEditBox, NULL, 0, 0, 0, 0, SWP_NOZORDER); 

		EditBoxWndProc = (WNDPROC)SetWindowLongPtr(_hwndEditBox, GWL_WNDPROC, (LONG_PTR)EditBoxSubclassWndProc); 

		if(_preallocated_items > 0) { 
			SendMessage(_hwndListBox, LB_INITSTORAGE, _preallocated_items, 8); 
		} 

		ListBoxWndProc = (WNDPROC)SetWindowLongPtr(_hwndListBox, GWL_WNDPROC, (LONG_PTR)ListBoxSubclassWndProc); 
		Clear(); 
	} 
} 

/* 
 * called by CFrame::OnCommand. 
 */
BOOL KHyperlist::OnCommand(WPARAM wParam, LPARAM lParam) { 
	int wmId = LOWORD(wParam); 
	int wmEvent = HIWORD(wParam); 

	BOOL command_onselect = FALSE; 

	switch (wmId) { 
		case KHL_IDADDITEM: 
			OnAddRootItem((CHListItem*)lParam); 
			return 0; 

		case KHL_IDADDCHILDITEM: 
			OnAddChildItem((CHListItem**)lParam); 
			return 0; 

		case KHL_IDREDRAWITEM: 
			OnRedrawItem((CHListItem*)lParam); 
			return 0; 

		case KHL_IDDELETEITEM: 
			OnDeleteItem((CHListItem*)lParam); 
			return 0; 

		case KHL_IDSHOWITEM: 
			OnShowItem((CHListItem*)lParam); 
			return 0; 

		case KHL_IDLIST: 
			command_onselect = 0; 

			if(wmEvent == LBN_DBLCLK) { 
				command_onselect = HLI_DO_ACTION_ONSELECT; 
			} 
			if(wmEvent == LBN_SELCHANGE) { 
				WORD wItemNum = (WORD) SendMessage(_hwndListBox, LB_GETCURSEL, 0, 0L);
				CHListItem *pItem = (CHListItem *)SendMessage(_hwndListBox, LB_GETITEMDATA, wItemNum, 0L); 
				command_onselect = OnSelectItem(pItem, wItemNum); 

			} 
			if((command_onselect & HLI_DO_ACTION_ONSELECT) != 0) { 
				WORD wItemNum = (WORD) SendMessage(_hwndListBox, LB_GETCURSEL, 0, 0L); 
				CHListItem *pItem = (CHListItem *)SendMessage(_hwndListBox, LB_GETITEMDATA, wItemNum, 0L); 
				OnActionItem(pItem, wItemNum); 
				
			} 
			if((command_onselect & HLI_DO_INVALIDATE_ONSELECT) != 0) { 
				Invalidate(); 
			} 
			
			return 0; 

		case KHL_IDEDIT: 
			if(wmEvent == EN_CHANGE) { 
				++m_bEditBoxChanges; 
			} 
			else 
			if(wmEvent == EN_KILLFOCUS) { 
				//KHL_DEBUG("Edit EN_KILLFOCUS\n"); 

				CHAR szText[512]; 
				SendMessage(_hwndEditBox, WM_GETTEXT, (WPARAM)512, (LPARAM)szText); 
				m_pSelectedItem->lock(); 
				BOOL item_ok = m_pSelectedItem->SetText(m_nSelectedCell - 1, szText); 
				m_pSelectedItem->unlock(); 
				m_bEditBoxActive = false; 
				if(item_ok) { 
					RedrawItem(m_pSelectedItem); 
				} 

				SetWindowPos(_hwndEditBox, HWND_BOTTOM, 0, 0, 0, 0, 0); 
				::ShowWindow(_hwndEditBox, SW_HIDE); 
				m_bEditBoxChanges = 0; 
			} 
			else { 
				//KHL_DEBUG("Edit Event %d\n", (int)wmEvent); 
			} 

			return 0; 

		case KHL_IDBUTTON: 
			if(wmEvent == BN_CLICKED) { 
				POINT pt = { 1, 1 }; 
				if(MapWindowPoints((HWND)lParam, _hwndListBox, &pt, 1)) { 
					PostMessage(_hwndListBox, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
					PostMessage(_hwndListBox, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
				} 
			} 

			return 0; 
	} 

	return TRUE; // not processed
}




BOOL KHyperlist::DrawInit() { 
	_kddb_isok = FALSE; 

	m_nBitmapHeight = 0; 
	m_nBitmapWidth = 0; 

	if(_bitmapctl.bitmap_rows == 0) { 
		return FALSE; 
	} 

	if(_bitmapctl.bitmap_columns == 0) { 
		return FALSE; 
	} 

	TEXTMETRIC TextMetrics; 

	HDC hDC = GetDC(_hwndListBox); 
	GetTextMetrics(hDC, &TextMetrics); 
	ReleaseDC(_hwndListBox, hDC); 

	m_nTextHeight = TextMetrics.tmHeight + TextMetrics.tmExternalLeading; 

	BOOL ok = FALSE; 

	KDIB kdib; 
	if(_bitmapctl.id) { 
		HRSRC hRes = FindResource(_hinst, MAKEINTRESOURCE(_bitmapctl.id), RT_BITMAP); 
		if(hRes) { 
			ok = kdib.Attach((LPBITMAPINFO)LockResource(LoadResource(_hinst, hRes)), NULL, DIB_BMI_READONLY | DIB_BITS_READONLY); 
		} 
	} 
	else 
	if(_bitmapctl.file_name.size()) { 
		ok = kdib.Attach(KWinLoadBitmapFile(_bitmapctl.file_name.c_str()), 0, DIB_BMI_NEEDFREE); 
	} 

	if(!ok) { 
		return FALSE; 
	} 

	m_nBitmapHeight = (LONG)kdib._height / _bitmapctl.bitmap_rows; 
	m_nBitmapWidth = (LONG)kdib._width / _bitmapctl.bitmap_columns; 

	m_nLineHeight = (LONG)(std::max(m_nBitmapHeight, m_nTextHeight) * _line_heightfactor); 

	_kddb_isok = _kddb.Attach(kdib); 

	return TRUE; 
} 

VOID KHyperlist::DrawSetTextHeight(HFONT hFont) 
{
	TEXTMETRIC TextMetrics; 

	HDC hdc = GetDC(_hwndListBox);
	SelectObject(hdc, hFont); 
	GetTextMetrics(hdc, &TextMetrics);
	ReleaseDC(_hwndListBox, hdc);

	m_nTextHeight = TextMetrics.tmHeight; 
	m_nLineHeight = (LONG)(std::max(m_nBitmapHeight, m_nTextHeight) * _line_heightfactor); 

	SendMessage(_hwndListBox, LB_SETITEMHEIGHT, 0, MAKELPARAM(m_nLineHeight, 0)); 
	SendMessage(_hwndListBox, WM_SETFONT, (WPARAM)hFont, 0); 
	SendMessage(_hwndEditBox, WM_SETFONT, (WPARAM)hFont, 0); 
}

BOOL KHyperlist::ClearLastHyperLinkSelection() { 
	BOOL ok = FALSE; 
	if(!IsRectEmpty(&m_rLastHyperlinkRect)) { 
		InvalidateRect(_hwndListBox, &m_rLastHyperlinkRect, FALSE); 
		SetRectEmpty(&m_rLastHyperlinkRect); 
		ok = TRUE; 
	} 

	return ok; 
} 


void KHyperlist::Invalidate() { 
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
	InvalidateRect(_hwnd, NULL, TRUE); 
	SetWindowPos(_hwndListBox, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER); 
	InvalidateRect(_hwndListBox, NULL, TRUE); 
} 

void KHyperlist::Clear() { 
	if(_hwndListBox) { 
		m_pSelectedItem = 0; 
		SendMessage(_hwndListBox, LB_RESETCONTENT, 0, 0L); 
		if(_format_list.size()) { 
			if(!_noheaders) { 
				AddRootItem(new CHListHeader(&_format_list[0])); 
			} 
		} 
		if(_entryfield_list.size()) { 
			std::for_each(_entryfield_list.begin(), _entryfield_list.end(), XMemFunction(&KHyperlist::AddRootItem, this)); 
		} 
	} 
}

int KHyperlist::PostItemRequest(CHListItem *pItem, WPARAM wParam) { 
	if(pItem == 0) { 
		return 0; 
	} 

	int rc = 0; 
	if(_hwndListBox) { 
		if(PostMessage(_hwnd, WM_COMMAND, wParam, (LPARAM)pItem)) { 
			rc = 1; 
		} 
	} 

	return rc; 
} 


int KHyperlist::OnAddRootItem(CHListItem *pItem) { 
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
			pItem->m_pFormat = GetFormatItem(pItem); 
		} 
	} 

	return rc; 
} 

int KHyperlist::AddRootItem(CHListItem& item) { 
	return PostItemRequest(&item, KHL_IDADDITEM); 
} 

int KHyperlist::AddRootItem(CHListItem *pItem) { 
	return PostItemRequest(pItem, KHL_IDADDITEM); 
} 

int KHyperlist::OnAddChildItem(CHListItem **items) { 
	CHListItem *pItemParent = items[0]; 
	CHListItem *pItemChild = items[1]; 

	delete items; 

	if(!pItemParent || !pItemChild) { 
		return 0; 
	} 

	LRESULT lResult = LB_ERR; 
	if(_hwndListBox) { 
		lResult = SendMessage(_hwndListBox, LB_FINDSTRING, (WPARAM)-1, (LPARAM)pItemParent); 
		if(lResult != LB_ERR) { 
			SendMessage(_hwndListBox, WM_SETREDRAW, FALSE, 0L);   // Disable redrawing. 

			DWORD dwItemNum = (DWORD)lResult; 
			++dwItemNum;

			BYTE nbLevel = pItemParent->m_nbLevel; 

			CHListItem *pChild = (CHListItem *)SendMessage(_hwndListBox, LB_GETITEMDATA, dwItemNum, 0L); 
			if(pChild == (CHListItem*)LB_ERR) { 
				pChild = 0; 
			} 
			if(pChild) { 
				nbLevel = pChild->m_nbLevel;  
			} 

			while(nbLevel > pItemParent->m_nbLevel) { 
				pChild = (CHListItem *)SendMessage(_hwndListBox, LB_GETITEMDATA, ++dwItemNum, 0L); 
				if(pChild == (CHListItem *)LB_ERR) { 
					nbLevel = pItemParent->m_nbLevel; 
				} 
				else 
				if(pChild) { 
					nbLevel = pChild->m_nbLevel;  
				} 
			} 

			pItemChild->m_nbLevel = pItemParent->m_nbLevel + 1; 

			if(pItemChild->m_pFormat == 0) { 
				pItemChild->m_pFormat = GetFormatItem(pItemChild); 
			} 
			if(pItemChild->m_pFormat == 0) { 
				pItemChild->m_pFormat = pItemParent->m_pFormat; 
			} 

			pItemChild->AddRef(); 
			lResult = SendMessage(_hwndListBox, LB_INSERTSTRING, dwItemNum, (LPARAM)pItemChild); 
			if(lResult == LB_ERR || lResult == LB_ERRSPACE) { 
				pItemChild->Release(); 
				lResult = LB_ERR; 
			} 

			SendMessage(_hwndListBox, WM_SETREDRAW, TRUE, 0L);   // Enable redrawing. 
		} 
	} 

	int rc = (lResult == LB_ERR)? 0: ((int)lResult + 1); 
	return rc; 
} 

int KHyperlist::AddChildItem(CHListItem *pItemParent, CHListItem *pItemChild) { 
	if(!pItemParent || !pItemChild) { 
		return 0; 
	} 

	if(!pItemParent->m_nbOpened) { 
		return 0; 
	} 

	if((pItemParent->m_nbLevel + 1) == pItemChild->m_nbLevel) { 
		return 0; // It is already there. 
	} 

	int rc = 0; 
	if(_hwndListBox) { 
		CHListItem **items = (CHListItem **)new PVOID[2]; 
		items[0] = pItemParent; 
		items[1] = pItemChild; 
		if(PostMessage(_hwnd, WM_COMMAND, KHL_IDADDCHILDITEM, (LPARAM)items)) { 
			rc = 1; 
		} 
		else { 
			delete items; 
		} 
	} 

	return rc; 
} 

int KHyperlist::OnRedrawItem(CHListItem *pItem) { 
	if(pItem == 0) { 
		return 0; 
	} 

	LRESULT lResult = LB_ERR; 
	if(_hwndListBox && !(m_pSelectedItem == pItem && m_bEditBoxActive)) { 
		lResult = SendMessage(_hwndListBox, LB_FINDSTRING, (WPARAM)-1, (LPARAM)pItem); 
		if(lResult != LB_ERR) { 
			LONG dwItem = (LONG)lResult; 

			if(pItem->m_nbOpened && !pItem->m_nbHasChild) { 
				OnActionItem(pItem, dwItem); 
			} 

			LONG wBottomIndex;
			LONG wTopIndex;
			RECT rc;

			wTopIndex = (WORD)SendMessage(_hwndListBox, LB_GETTOPINDEX, 0, 0L);
			GetClientRect(_hwndListBox, &rc); 
			wBottomIndex = wTopIndex + (rc.bottom + 1) / m_nLineHeight; 
			if(dwItem < wTopIndex) { 
			} 
			else 
			if(dwItem > wBottomIndex) { 
			} 
			else { 
				rc.top += (dwItem - wTopIndex) * m_nLineHeight; 
				rc.bottom = rc.top + m_nLineHeight; 
				InvalidateRect(_hwndListBox, &rc, FALSE); // Force redraw
			} 
		} 
	} 

	int rc = (lResult == LB_ERR)? 0: ((int)lResult + 1); 
	return rc; 
} 

int KHyperlist::RedrawItem(CHListItem *pItem) { 
	return PostItemRequest(pItem, KHL_IDREDRAWITEM); 
} 

int KHyperlist::OnDeleteItem(CHListItem *pItem) { 
	if(pItem == 0) { 
		return 0; 
	} 

	LRESULT lResult = LB_ERR; 
	if(_hwndListBox) { 
		lResult = SendMessage(_hwndListBox, LB_FINDSTRING, (WPARAM)-1, (LPARAM)pItem); 
		if(lResult != LB_ERR) { 
			LONG dwItem = (LONG)lResult; 
			if(pItem->m_nbOpened) { 
				OnActionItem(pItem, dwItem); 
			} 
			if(m_pSelectedItem == pItem) { 
				m_pSelectedItem = 0; 
			} 
			SendMessage(_hwndListBox, LB_DELETESTRING, dwItem, 0L); 
		} 
	} 

	int rc = (lResult == LB_ERR)? 0: ((int)lResult + 1); 
	return rc; 
} 

int KHyperlist::DeleteItem(CHListItem *pItem) { 
	return PostItemRequest(pItem, KHL_IDDELETEITEM); 
} 

int KHyperlist::OnShowItem(CHListItem *pItem) { 
	if(pItem == 0) { 
		return 0; 
	} 

	LRESULT lResult = LB_ERR; 
	if(_hwndListBox) { 
		lResult = SendMessage(_hwndListBox, LB_FINDSTRING, (WPARAM)-1, (LPARAM)pItem); 
		if(lResult != LB_ERR) { 
			LONG dwItem = (LONG)lResult; 
			LONG dwTopIndex = (WORD)SendMessage(_hwndListBox, LB_GETTOPINDEX, 0, 0L);
			RECT rc;
			GetClientRect(_hwndListBox, &rc); 
			LONG dwBottomIndex = dwTopIndex + (rc.bottom + 1) / m_nLineHeight - 1; 

			LONG dwNewTopIndex = -1; 

			if(dwItem < dwTopIndex) { 
				dwNewTopIndex = dwItem; 
			} 
			else 
			if(dwItem > dwBottomIndex) { 
				dwNewTopIndex = dwItem + (dwTopIndex - dwBottomIndex) + 1; 
			} 

			if(dwNewTopIndex != -1) { 
				lResult = SendMessage(_hwndListBox, LB_SETTOPINDEX, (DWORD)dwNewTopIndex, 0L); 
			} 

//			SendMessage(_hwndListBox, WM_SETREDRAW, TRUE, 0L); // Enable redrawing.
			InvalidateRect(_hwndListBox, NULL, TRUE); // Force redraw
		} 
	} 

	int rc = (lResult == LB_ERR)? 0: ((int)lResult + 1); 
	return rc; 
} 

int KHyperlist::ShowItem(CHListItem *pItem) { 
	return PostItemRequest(pItem, KHL_IDSHOWITEM); 
} 

CHListItem * KHyperlist::GetSelectedItem() { 
	return m_pSelectedItem; 
} 

CHFormatItem * KHyperlist::GetFormatItem(CHListItem *pItem) { 
	BYTE nbLevel = pItem->m_nbLevel; 

	if(pItem->m_nbItemType == 1) { // Plain Text - Apply alternative format. 
		if(_plain_text_format.size()) { 
			if(nbLevel < _plain_text_format.size()) { 
				return &_plain_text_format[nbLevel]; 
			} 
			else { 
				return &(*_plain_text_format.rbegin()); 
			} 
		} 
	} 
	if(nbLevel < _format_list.size()) { 
		return &_format_list[nbLevel]; 
	} 
	if(_format_list.size()) { 
		return &(*_format_list.rbegin()); 
	} 

	return NULL; 
} 


CHListEntryField * KHyperlist::GetEntryField(const char **ui, int count, std::vector<CHListEntryField> *vec) { 
	if(!vec) { 
		vec = &_entryfield_list; 
	} 
	std::vector<CHListEntryField>::iterator iter = vec->begin(); 
	while(ui[0] && iter != vec->end()) { 
		if((*iter)._ui == ui[0]) { 
			if(--count > 0) { 
				return GetEntryField(&ui[1], count, &(*iter)._sub_fields); 
			} 
			return &(*iter); 
		} 
		++iter; 
	} 

	return NULL; 
} 


BOOL KHyperlist::PushEntryField(IXFunction<std::string/*arg*/, int> *validate, const std::string& value, const char **ui, int count) { 
	BOOL ok = FALSE; 
	CHListEntryField *pField = GetEntryField(ui, count); 
	if(pField) { 
		pField->lock(); 

		pField->_error.resize(0); 
		pField->_error_number = 0; 
		if(pField->_validate) { 
			delete pField->_validate; 
		} 
		pField->_value = value; 
		pField->_validate = validate; 

		pField->unlock(); 

		ok = TRUE; 
	} 
	return ok; 
} 

BOOL KHyperlist::ChangeEntryField(const std::string& value, const char **ui, int count) { 
	BOOL ok = FALSE; 
	CHListEntryField *pField = GetEntryField(ui, count); 
	if(pField) { 
		pField->lock(); 

		pField->_error.resize(0); 
		pField->_error_number = 0; 
		pField->_value = value; 
		if(pField->_validate) { 
			pField->Validate(); 
		} 

		pField->unlock(); 

		RedrawItem(pField); 

		ok = TRUE; 
	} 
	return ok; 
} 

BOOL KHyperlist::EntryFieldsAreValid() { 
	BOOL ok = TRUE; 
	SetFocus(_hwnd); 
	std::vector<CHListEntryField>::iterator iter = _entryfield_list.begin(); 
	while(iter != _entryfield_list.end()) { 
		//if((*iter).._error_number) { 
		if(!(*iter).Validate()) { 
			ok = FALSE; 
			break; 
		} 
		++iter; 
	} 

	return ok; 
} 


VOID KHyperlist::OnMeasureItem(MEASUREITEMSTRUCT *lpMeasureItem) { 
	lpMeasureItem->itemHeight = m_nLineHeight; 
} 

VOID KHyperlist::OnLbMouseMove(LPARAM lParam) { 
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
				BuildDrawItemCtl(pItem/*in*/, item_rect/*in*/, m_nMouseXPos, ctl/*out*/); 
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

BOOL KHyperlist::OnDrawItemEditBox(CHListItem *pItem) { 
	BOOL ok = FALSE; 
	if(m_pSelectedItem == pItem && m_bEditBoxActive) { 
		if(m_nSelectedCell) { 
			RECT& rect = m_rLastEditBoxRect; 
			rect = m_nSelItemsInnerBoxes[m_nSelectedCell - 1]; 

			double ratio = (double)m_nTextHeight / (double)m_nLineHeight; 
			double height = rect.bottom - rect.top; 
			double adj_height = height * ratio; 
			rect.top += (int)((height - adj_height) * 0.5); 
			rect.bottom = (int)(rect.top + adj_height + 0.5); 

			POINT origin = { rect.left + 1, rect.top }; 
			POINT ptsize = { rect.right - rect.left - 2, rect.bottom - rect.top }; 

			POINT actual_origin = { 0, 0 }; 
			MapWindowPoints(_hwndEditBox, _hwndListBox, &actual_origin, 1); 
			if(actual_origin.x != origin.x || actual_origin.y != origin.y) { 
				SetWindowPos(_hwndEditBox, HWND_TOP, origin.x, origin.y, ptsize.x, ptsize.y, 0); 
			} 
			ok = TRUE; 
		} 
	} 

	return ok; 
} 

BOOL KHyperlist::OnSelectItem(CHListItem *pItem, DWORD dwItemNum) { 
	if(pItem == 0) { 
		return FALSE; 
	} 

	BOOL do_action = FALSE; 

	if(m_pSelectedItem == pItem) { 
		//KHL_DEBUG("OnSelect Item <%s>\n", pItem->GetText(_dbgstr1)); 
		for(int j = 0; j < 32; ++j) { 
			if(	m_nCellSelectionXPos >= m_nSelItemsOuterBoxes[j].left &&
				m_nCellSelectionXPos <  m_nSelItemsOuterBoxes[j].right) { 
					do_action = pItem->OnClick(_hwndCntxt, j + 1); 

					if(m_nSelectedCell != j + 1) { 
						pItem->OnSelect(_hwndCntxt, m_nSelectedCell = j + 1); 
					} 

					BYTE nbEditFlags[32]; 
					pItem->GetColumnsEditFlags(nbEditFlags); 
					if(nbEditFlags[j] > 0) { 
						m_bEditBoxActive = true; 

						if(OnDrawItemEditBox(pItem)) { 
							pItem->lock(); 
							std::string szText =  pItem->GetText(m_nSelectedCell - 1); 
							DecodeHtmlentities(szText); 
							SetWindowText(_hwndEditBox, szText.c_str()); 
							pItem->unlock(); 
							::ShowWindow(_hwndEditBox, SW_SHOWNORMAL); 
							int nclicks = pItem->m_pFormat->_cell_select_onedit[m_nSelectedCell - 1]? 2: 1; 
							for(int x = 0; x < nclicks; ++x) { 
								PostMessage(_hwndEditBox, x > 0? WM_LBUTTONDBLCLK: WM_LBUTTONDOWN, MK_LBUTTON, m_nCellSelectionXPos - m_nSelItemsInnerBoxes[j].left);
								PostMessage(_hwndEditBox, WM_LBUTTONUP, MK_LBUTTON, m_nCellSelectionXPos - m_nSelItemsInnerBoxes[j].left);
							} 
						} 
						else { 
							m_bEditBoxActive = false; 
						} 
					} 
			} 

			if(m_nSelItemsOuterBoxes[j].right == 0) { 
				break; 
			} 

			InvalidateRect(_hwndListBox, &m_nSelItemsOuterBoxes[j], TRUE);
		} 
	} 
	else { 
		//KHL_DEBUG("OnSelect Item <%s> is called for the item not being selected\n;current selected item is <%s>", pItem->GetText(_dbgstr1), m_pSelectedItem? m_pSelectedItem->GetText(_dbgstr2): "NULL"); 
	} 

	return do_action; 
} 

VOID KHyperlist::OnActionItem(CHListItem *pItem, DWORD dwItemNum) { 
	if(pItem == 0) { 
		return; 
	} 

	if(pItem->m_nbHasChild || pItem->m_nbOpened) { 
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
			if(pChildItem == (CHListItem*)(-1)) { 
				pChildItem = 0; 
			} 
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
						if(_format_list.size() > (size_t)(pItem->m_nbLevel + 1)) { 
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
						pChildItem->m_dwMask |= (DWORD)pow(2.0, (double)pChildItem->m_nbLevel - 1); 
					} 

					if(pChildItem->m_pFormat == 0) { 
						pChildItem->m_pFormat = GetFormatItem(pChildItem); 
					} 
					if(pChildItem->m_pFormat == 0) { 
						pChildItem->m_pFormat = pItem->m_pFormat; 
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
				wNewTopIndex = std::min((LONG)dwItemNum, (wTopIndex + wKids + 1) - wExpandInView); 
				SendMessage(_hwndListBox, LB_SETTOPINDEX, (DWORD)wNewTopIndex, 0L); 
			} 

			SendMessage(_hwndListBox, WM_SETREDRAW, TRUE, 0L); // Enable redrawing.
			InvalidateRect(_hwndListBox, NULL, TRUE); // Force redraw
		} 
	} 

	pItem->OnAction(_hwndCntxt); 
} 



VOID KHyperlist::BuildDrawItemCtl(CHListItem *pItem/*in*/, RECT& item_rect/*in*/, int x_selpos/*in*/, CHListItemDrawCtl& ctl/*out*/) { 
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

	ctl.wIndent = ctl.rcTemp.left + (ctl.nLevel * (m_nBitmapWidth + KHL_XBMPOFFSET)) + (m_nBitmapWidth? KHL_XBMPOFFSET: 0); 
	ctl.wAbsIndent = 0; 
	if(_nolines) { 
		ctl.wIndent = ctl.rcTemp.left + m_nBitmapWidth + KHL_XBMPOFFSET; 
	} 

	ctl.wTopText = (LONG)(ctl.rcTemp.top + ((ctl.rcTemp.bottom - ctl.rcTemp.top) / 2.0) - (m_nTextHeight / 2.0)); 
	ctl.wTopBitmap = (LONG)(ctl.rcTemp.top + ((ctl.rcTemp.bottom - ctl.rcTemp.top) / 2.0) - (m_nBitmapHeight / 2.0)); 

	ctl.wTotalWidth = ctl.rcTemp.right - ctl.rcTemp.left - ctl.wIndent; 

	switch(ctl.nUnits) { 
	case 0/*Pos. in Pix.*/: 
//		Turn absolute positions to width. 
		if(ctl.nColumns[0] != 0) { 
			ctl.wAbsIndent += ctl.nColumns[0]; 
			ctl.wIndent += ctl.wAbsIndent; 
			ctl.wTotalWidth = ctl.rcTemp.right - ctl.rcTemp.left - ctl.wIndent; 
		} 
		
		int j; 
		for(j = 0; j < 31 && (ctl.nColumns[j] != 0 || ctl.nColumns[j + 1] != 0); ++j) { 
			if(ctl.nColumns[j + 1] == 0) { 
				ctl.nColumns[j] = ctl.rcTemp.right - ctl.nColumns[j]; 
			} 
			else { 
				ctl.nColumns[j] = ctl.nColumns[j + 1] - ctl.nColumns[j]; 
			} 
		} 

		ctl.nColumns[j + 1] = 0; 
	break; 
	case 2/*Percents*/: 
	case 3/*Percents*/: 
//		Turn percents to actual width. 
		for(int j = 0; j < 32 && ctl.nColumns[j] != 0; ++j) { 
			ctl.nColumns[j] = (int)floor(((double)ctl.nColumns[j]/100.0) * ctl.wTotalWidth + 0.5); 
			if(ctl.nColumns[j] < (m_nBitmapWidth + KHL_XBMPOFFSET)) { 
				ctl.nColumns[j] = m_nBitmapWidth + KHL_XBMPOFFSET; 
			} 
			if(ctl.nColumns[j] > ctl.wTotalWidth) { 
				ctl.nColumns[j] = 0; 
			} 
		} 
	} 

//	KHL_DEBUG("Mouse x-pos on draw item: %d\n", (int)x_selpos); 

//	Find selected cell. 
	int left = ctl.wIndent; 
	int j; 
	for(j = 0; j < 32 && ctl.nColumns[j] != 0; ++j) { 
		int right = left + ctl.nColumns[j]; 
		if(x_selpos >= left && x_selpos < right) { 
			ctl.nSelectedCell = j + 1; 
			break; 
		} 

		left = right; 
	} 
} 


VOID KHyperlist::OnDrawItem(DRAWITEMSTRUCT * lpDrawItem) 
{
	if(lpDrawItem->itemID == (UINT)-1) { 
		return; 
	} 

	CHListItem *pItem = (CHListItem *)lpDrawItem->itemData; 
	if(!pItem) { 
		return; 
	} 

//	pItem->GetText(_dbgstr1); 
//	KHL_DEBUG("draw item <%s>\n", _dbgstr1.c_str()); 

	std::string szText; 

	HDC hDC = lpDrawItem->hDC; 


	CHListItemDrawCtl ctl; 
	BuildDrawItemCtl(pItem/*in*/, lpDrawItem->rcItem/*in*/, m_nCellSelectionXPos, ctl/*out*/); 

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
//			KHL_DEBUG("got focus Item <%s> l=%d r=%d t=%d b=%d\n", pItem->GetText(_dbgstr1), rcTemp.left, rcTemp.right, rcTemp.top, rcTemp.bottom); 
		} 
		else { 
//			KHL_DEBUG("lost focus Item <%s> l=%d r=%d t=%d b=%d\n", pItem->GetText(_dbgstr1), rcTemp.left, rcTemp.right, rcTemp.top, rcTemp.bottom); 
		} 
	} 
//	else 
	if(lpDrawItem->itemAction == ODA_SELECT) { 
		bDrawLinesForOuterLevels = FALSE; 
		bDrawSelectedBitmap = FALSE; 
		if((lpDrawItem->itemState & ODS_SELECTED)) { 
			//KHL_DEBUG("got selection Item <%s>\n", pItem->GetText(_dbgstr1)); 
			if(m_pSelectedItem) { 
				m_pSelectedItem->OnLostSelect(_hwndCntxt); 
			} 

			pItem->OnSelect(_hwndCntxt, m_nSelectedCell = nSelectedCell); 
			m_pSelectedItem = pItem; 
		} 
		else { 
			//KHL_DEBUG("lost selection Item <%s>\n", pItem->GetText(_dbgstr1)); 
			pItem->OnLostSelect(_hwndCntxt); 
			m_pSelectedItem = 0; 
		} 

	} 

//	Draw a series of | lines for outer levels

	if(bDrawLinesForOuterLevels && !_nolines && nLevel) {
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
		if(pItem->m_nbItemType != 2) { 
			FastRect(hDC, x, (rcTemp.bottom - m_nLineHeight / 2), (m_nBitmapWidth / 2) + wAbsIndent, 1); 
		} 
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

	BYTE bIsButton[32]; 
	pItem->GetIsButton(bIsButton); 

	DWORD dwBkColor[32]; 
	DWORD dwTextColor[32]; 
	pItem->GetColumnsColors(dwBkColor, dwTextColor); 

	int nBMRow = 0; 
	int nBMColumns[32] = { 0 }; 
	pItem->GetBitMapIndices(pItem->m_nbOpened, nBMRow, nBMColumns); 

	LONG wBitmapXPos = wIndent; 

	BOOL bRedrawItem = FALSE; 

	for(int j = 0; j < 32 && (nColumns[j] != 0 || nBMColumns[j] != 0); ++j) { 
		pItem->lock(); 
		pItem->GetText(j, szText); 
		pItem->unlock(); 

		DecodeHtmlentities(szText); 

		BOOL bDrawBitmap = bDrawSelectedBitmap; 
		if(nBMColumns[j] == 0 || nBMRow == 0 || !_kddb_isok) { 
			bDrawBitmap = 0; 
		} 

		if(bDrawBitmap) { 
			_kddb.TransparentDraw(hDC, 
				wBitmapXPos, wTopBitmap, m_nBitmapWidth, m_nBitmapHeight, 
				m_nBitmapWidth *(nBMColumns[j] - 1), m_nBitmapHeight * (nBMRow - 1), m_nBitmapWidth, m_nBitmapHeight); 
		} 

		rcTemp.left = wBitmapXPos + (nBMColumns[j]? m_nBitmapWidth  + 1: 0); 

		SIZE strSize = { 0 }; 
		if(GetTextExtentPoint(hDC, szText.c_str(), (int)szText.size(), &strSize)) { 
			rcTemp.right = rcTemp.left + strSize.cx + 1; 
		} 

		if(nColumns[j] > 0) { 
			rcTemp.right = wBitmapXPos + nColumns[j]; 
		} 

		BOOL bTruncated = (strSize.cx > 0) && (strSize.cx > (rcTemp.right - rcTemp.left)); 

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
			else 
			if(_ctl.transparent) { 
				SetBkColor(hDC, _ctl.backcolor); 
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

			SetBkMode(hDC, _ctl.transparent? TRANSPARENT: OPAQUE); 

//			KHL_DEBUG("draw text Item <%s> l=%d r=%d t=%d b=%d\n", szText.c_str(), rcTemp.left, rcTemp.right, rcTemp.top, rcTemp.bottom); 

			if(bIsButton[j]) { 
				POINT origin = { rcTemp.left + 2, rcTemp.top + 1 }; 
				POINT ptsize = { rcTemp.right - rcTemp.left - 4, rcTemp.bottom - rcTemp.top - 2 }; 
				if(pItem->m_hButton[j]) { 
					POINT actual_origin = { 0, 0 }; 
					MapWindowPoints(pItem->m_hButton[j], _hwndListBox, &actual_origin, 1); 
					if(actual_origin.x != origin.x || actual_origin.y != origin.y) { 
						SetWindowPos(pItem->m_hButton[j], HWND_TOP, origin.x, origin.y, ptsize.x, ptsize.y, 0); 
					} 
				} 
				else { 
					pItem->m_hButton[j] = CreateWindow("BUTTON", szText.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | (bDrawBitmap? BS_BITMAP: 0) | BS_CENTER, 
						origin.x, origin.y, ptsize.x, ptsize.y, _hwndListBox, (HMENU)KHL_IDBUTTON, _hinst, NULL); 
					SendMessage(pItem->m_hButton[j], WM_SETFONT, (WPARAM)GetCurrentObject(hDC, OBJ_FONT), 0); 
				} 
			} 
			else { 
				if(pItem->IsRightAlignment(j)) { 
					ExtTextOut(hDC, rcTemp.right - strSize.cx - 1, wTopText, ETO_CLIPPED|ETO_OPAQUE, 
						&rcTemp, szText.c_str(), (int)szText.size(), NULL);
				} 
				else { 
					ExtTextOut(hDC, rcTemp.left + 1, wTopText, ETO_CLIPPED|ETO_OPAQUE, 
						&rcTemp, szText.c_str(), (int)szText.size(), NULL);
				} 

				if(nUnits == 3 && bTruncated) { 
					double fScale = ((double)strSize.cx + 1) / ((double)(rcTemp.right - rcTemp.left)); 
					if(pItem->ScaleColumnOffset(j, fScale)) { 
						bRedrawItem = TRUE; 
					} 
				} 
			} 

			if(holdfont) { 
			    SelectObject(hDC, holdfont); 
			} 
			if(hfont) { 
			    DeleteObject(hfont); 
			} 
		} 

		if(m_nSelectedCell == (j + 1)) { 
			if(m_bEditBoxActive) { 
				if(OnDrawItemEditBox(pItem)) { 
					FrameRect(hDC, &rcTemp, (HBRUSH)GetStockObject(LTGRAY_BRUSH)); 
				} 
			} 
		} 

		switch(bBorders[j]) { 
			case 1: // Box 
				FrameRect(hDC, &rcTemp, (HBRUSH)GetStockObject(LTGRAY_BRUSH)); 
				break; 
			case 2: // Vertical Lines 
				Draw4FastLines(hDC, rcTemp, 1, 0); 
				break; 
			case 3: // Horizontal Lines 
				Draw4FastLines(hDC, rcTemp, 0, 1); 
				break; 
		} 
	} 

	if(bRedrawItem) { 
		Invalidate(); 
	} 
} 

/* 
 * CALLED: By KWindow::WindowProc. 
 * PURPOSE: Process messages for the main window. 
 */
LRESULT KHyperlist::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	KHyperlist *pWnd = this; 

	switch (message) {
		case WM_CTLCOLORLISTBOX: 
			if(pWnd && pWnd->_ctl.hbrBackground) { 
				if(pWnd->_ctl.transparent) { 
					return (LRESULT)pWnd->_ctl.hbrBackground; 
				} 
			} 
		break; 

		case WM_CREATE: { 
			pWnd->OnCreate(hWnd); 
		} 
		return 0; 

		case WM_SIZE: if(!_is_sizing) { 
			MoveWindow(pWnd->_hwndListBox, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);	
		} 
		return 0; 

		case WM_NCDESTROY: 
		return 0; 

		case WM_DELETEITEM: 
			if(LOWORD(wParam) == KHL_IDLIST) {
				LPDELETEITEMSTRUCT lpDeleteItem = (LPDELETEITEMSTRUCT) lParam; 
				CHListItem *pItem = (CHListItem *)lpDeleteItem->itemData; 
				if(pItem) { 
					std::for_each(pItem->m_hButton, pItem->m_hButton + 32, MyDestroyWindow); 
//					std::for_each(pItem->m_hButton, pItem->m_hButton + 32, _set_value<HWND, 0>); 
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
			if(!pWnd->OnCommand(wParam, lParam)) { 
				return 0; 
			} 
		break; 

//		default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
	}

//	return 0;
	return DefWindowProc(hWnd, message, wParam, lParam);
} 

LRESULT CALLBACK KHyperlist::ListBoxSubclassWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { 
	HWND hParent = (HWND)GetWindowLongPtr(hWnd, GWLP_HWNDPARENT); 
	KHyperlist *pWnd = 0; 
	if(hParent) { 
		pWnd = (KHyperlist *)GetWindowLongPtr(hParent, GWL_USERDATA);
	} 

	switch (message) {
	case WM_LBUTTONDOWN: 
		if(pWnd) { 
			pWnd->m_nCellSelectionXPos = LOWORD(lParam); 
		} 

		break; 

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
		if(pWnd) { 
			if(!pWnd->OnCommand(wParam, lParam)) { 
				return 0; 
			} 
		} 
		break; 

	case WM_ERASEBKGND: 
		if(pWnd->_is_sizing) { 
			return 1; 
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

LRESULT CALLBACK KHyperlist::EditBoxSubclassWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { 
	HWND hLbParent = (HWND)GetWindowLongPtr(hWnd, GWLP_HWNDPARENT); 
	HWND hMainParent = (HWND)GetWindowLongPtr(hLbParent, GWLP_HWNDPARENT); 
	KHyperlist *pWnd = 0; 
	if(hMainParent) { 
		pWnd = (KHyperlist *)(LONG_PTR)GetWindowLongPtr(hMainParent, GWL_USERDATA); 
	} 

	bool processed = false; 

	switch (message) {
	case WM_KEYDOWN:
		if(wParam == VK_DOWN) { 
			processed = true; 
		} 
		if(wParam == VK_UP) { 
			processed = true; 
		} 
		if(wParam == VK_TAB) { 
			wParam = (GetKeyState(VK_SHIFT) & ~1)? VK_UP: VK_DOWN; 
			processed = true; 
		} 
		if(wParam == VK_RETURN) { 
			processed = true; 
		}  
		break; 
	} 

	if(pWnd) { 
		if(processed) { 
			POINT pt; 
			if(GetCaretPos(&pt)) { 
				pWnd->m_nCellSelectionXPos = pWnd->m_rLastEditBoxRect.left + pt.x; 
//				pWnd->KHL_DEBUG("New x-pos from Caret-pos: %d\n", (int)pWnd->m_nCellSelectionXPos); 
				PostMessage(hWnd, WM_KILLFOCUS, 0, 0); 
				PostMessage(hLbParent, message, wParam, lParam); 
			} 
		} 
		else 
		if(hWnd == pWnd->_hwndEditBox && pWnd->EditBoxWndProc) { 
			#ifdef __BORLANDC__
			FARPROC proc = (FARPROC)pWnd->EditBoxWndProc; 
			return CallWindowProc(proc, hWnd, message, wParam, lParam); 
			#else
			return CallWindowProc(pWnd->EditBoxWndProc, hWnd, message, wParam, lParam); 
			#endif
		} 
	} 
	else { 
		return DefWindowProc(hWnd, message, wParam, lParam); 
	} 

	return 0; 
} 

