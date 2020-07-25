#pragma once

#include ".\win.h"
#include ".\HListItem.h" 
#include ".\Controls.h"


#define KHL_XBMPOFFSET  4

#define KHL_IDADDITEM		1
#define KHL_IDREDRAWITEM	2
#define KHL_IDADDCHILDITEM	3
#define KHL_IDDELETEITEM	4
#define KHL_IDSHOWITEM		5
#define KHL_IDLIST			101
#define KHL_IDEDIT			102
#define KHL_IDBUTTON		103

EXPORT_STL_VECTOR(CHFormatItem)
EXPORT_STL_VECTOR(CHListEntryField)

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


extern std::map<std::string, int> g_mapHtmlentities; 

BOOL EXP_KWIN DecodeHtmlentities(std::string& szText); 


class EXP_KWIN KHyperlist: public KWindow { 
	HINSTANCE _hinst; 

	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

	HWND _hwndListBox; 
	HWND _hwndEditBox; 
	static LRESULT CALLBACK ListBoxSubclassWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 
	static LRESULT CALLBACK EditBoxSubclassWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 

public: 
	LONG _preallocated_items; 
	double _line_heightfactor; 
	BOOL _debug; 
	BOOL _noheaders; 
	BOOL _nolines; 
	BOOL _noborder; 

	std::vector<CHFormatItem> _plain_text_format; 

	std::vector<CHFormatItem> _format_list; 
	std::vector<CHListEntryField> _entryfield_list; 
	std::vector<std::string> _data_type; 

	KHyperlist(); 
	virtual ~ KHyperlist(); 
	
	void Create(HINSTANCE hInst, HWND hParent); 
	void OnCreate(HWND hMe); 
	BOOL OnCommand(WPARAM wParam, LPARAM lParam); 

private: 
	KDDB _kddb; 
	BOOL _kddb_isok; 

	LONG m_nBitmapHeight;
	LONG m_nBitmapWidth;
	LONG m_nTextHeight;
	LONG m_nLineHeight;

	std::string _dbgstr1; 
	std::string _dbgstr2; 

	BOOL m_bEditBoxActive; 
	WORD m_bEditBoxChanges; 

	RECT m_rLastEditBoxRect; 
	RECT m_rLastHyperlinkRect; 

	BOOL ClearLastHyperLinkSelection(); 
	BOOL DrawInit(); 
	VOID DrawSetTextHeight(HFONT hFont); 
	VOID BuildDrawItemCtl(CHListItem *pItem/*in*/, RECT& item_rect/*in*/, int x_selpos/*in*/, CHListItemDrawCtl& ctl/*out*/); 

	VOID OnLbMouseMove(LPARAM lParam); 

	VOID OnMeasureItem(MEASUREITEMSTRUCT *lpMeasureItem); 
	VOID OnDrawItem(DRAWITEMSTRUCT * lpDrawItem); 
	VOID OnActionItem(CHListItem *pItem, DWORD wItemNum); 
	BOOL OnSelectItem(CHListItem *pItem, DWORD wItemNum); 
	BOOL OnDrawItemEditBox(CHListItem *pItem); 

	int PostItemRequest(CHListItem *, WPARAM); 

	int OnAddRootItem(CHListItem *); // Returns either index of added item or 0 if it was not added. 
	int OnAddChildItem(CHListItem **); // Returns either index of added item or 0 if it was not added. 
	int OnRedrawItem(CHListItem *); // Redraws an item, returns either index of item or 0 if it was not found. 
	int OnDeleteItem(CHListItem *); // Deletes an item, returns either index of item or 0 if it was not found. 
	int OnShowItem(CHListItem *); // Makes sure an item is showing. 

	DWORD RGB2BGR(DWORD rgb) { return RGB(GetBValue(rgb), GetGValue(rgb), GetRValue(rgb)); } 
	VOID FastRect(HDC hDC, int x, int y, int cx, int cy); 
	VOID Draw4FastLines(HDC hDC, RECT& rc, int cx, int cy); 

	CHListEntryField * GetEntryField(const char **ui, int count = 1, std::vector<CHListEntryField> *vec = 0); 

private: 
	CHListItem *m_pSelectedItem; 

	RECT m_nSelItemsInnerBoxes[32]; 
	RECT m_nSelItemsOuterBoxes[32]; 

	int m_nSelectedCell; 


public: 
	WNDPROC ListBoxWndProc; 
	WNDPROC EditBoxWndProc; 

	int m_nMouseXPos; 
	int m_nMouseYPos; 

	int m_nCellSelectionXPos; 

	HWND _hwndCntxt; 

	void Invalidate(); 
	void Clear(); 

	int AddRootItem(CHListItem *); // Returns 1 if request was posted, or 0 if not. 
	int AddRootItem(CHListItem &); 
	int AddChildItem(/*parent*/CHListItem *, /*child*/CHListItem *); 
	int RedrawItem(CHListItem *); // Returns 1 if request was posted, or 0 if not. 
	int DeleteItem(CHListItem *); // Returns 1 if request was posted, or 0 if not. 
	int ShowItem(CHListItem *); // Returns 1 if request was posted, or 0 if not. 

	CHFormatItem * GetFormatItem(CHListItem *pItem); 
	CHListItem * GetSelectedItem(); 

	BOOL PushEntryField(IXFunction<std::string/*arg*/, int> *validate, const std::string& value, const char **ui, int count = 1); 
	BOOL ChangeEntryField(const std::string& value, const char **ui, int count = 1); 
	BOOL EntryFieldsAreValid(); 

	void KHL_DEBUG(const char * format, ...); 

}; 


BEGIN_WSI_SERIALIZATION_OBJECT(KHyperlist)

CONTAINS_FLAT_MEMBER(_debug, Debug)
CONTAINS_FLAT_MEMBER(_data_type, DataType)
CONTAINS_FLAT_MEMBER(_noheaders, NoHeaders)
CONTAINS_FLAT_MEMBER(_nolines, NoLines)
CONTAINS_FLAT_MEMBER(_noborder, NoBorder)
CONTAINS_FLAT_MEMBER(_preallocated_items, PreallocatedItems)
CONTAINS_FLAT_MEMBER(_line_heightfactor, LineHeightFactor)
CONTAINS_OBJECT_MEMBER(_plain_text_format, PlainTextFormat)
CONTAINS_OBJECT_MEMBER(_format_list, ItemFormat)
CONTAINS_OBJECT_MEMBER(_entryfield_list, EntryField)
//CONTAINS_INHERITED_OBJECT_MEMBER(KWindow, _ctl, Situation)
//CONTAINS_INHERITED_OBJECT_MEMBER(KWindow, _bitmapctl, Bitmap)
CONTAINS_OBJECT_MEMBER(_ctl, Situation)
CONTAINS_OBJECT_MEMBER(_bitmapctl, Bitmap)
END_WSI_SERIALIZATION_OBJECT()


/*
 * Helper templates automating activation of EntryField in the HyperList.
 */

template <typename R, typename T, typename Base> 
BOOL HListPushEntryField(KHyperlist *pHList, R& record, const T& field, int (Base::*validator)(std::string&), Base *base) { 
	const char *ui = ((CWsiSerializerBase&)(GetRootSerializer((R*)0))).find_member_id_byoffset((char*)&field - (char*)&record); 
	if(ui == 0) { 
		return FALSE; 
	} 
	std::string value; 
	value += field; 
	return pHList->PushEntryField(AllocXMemFunction(validator, base), value, &ui); 
} 

template <typename R, typename T> 
BOOL HListChangeEntryField(KHyperlist *pHList, R& record, const T& field) { 
	const char *ui = ((CWsiSerializerBase&)(GetRootSerializer((R*)0))).find_member_id_byoffset((char*)&field - (char*)&record); 
	if(ui == 0) { 
		return FALSE; 
	} 
	std::string value; 
	value += field; 
	return pHList->ChangeEntryField(value, &ui); 
} 


inline 
std::string& operator+=(std::string& str, const double& x) {
	if(x == 0.0) {
		return str = std::string();
	}
	std::ostringstream ostr;
	ostr << x;
	return str = ostr.str();
}
template <typename T> inline
std::string& operator+=(std::string& str, const T& x) {
	std::ostringstream ostr;
	ostr << x;
	return str = ostr.str();
}
template <typename T, int S> inline
std::string& operator+=(std::string& str, T(&X)[S]) {
	std::ostringstream ostr;
	for(auto x : X) {
		if(ostr.str().size()) {
			ostr << ' ';
		}
		ostr << x;
	}
	return str = ostr.str();
}
template <typename T> inline
std::string& operator+=(std::string& str, const std::vector<T>& X) {
	std::ostringstream ostr;
	for(auto x : X) {
		if(ostr.str().size()) {
			ostr << ' ';
		}
		ostr << x;
	}
	return str = ostr.str();
}

