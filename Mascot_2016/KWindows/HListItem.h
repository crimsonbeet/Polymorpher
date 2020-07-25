#pragma once 

#include "IPCGate.h" 

#define HLI_DO_ACTION_ONSELECT 1 
#define HLI_DO_INVALIDATE_ONSELECT 2 



struct EXP_KWIN CHFormatItem { 
	int _offsetUnit; // 0 - Pos. in Pixels, 1 - Width in Pixels, 2 - Width in Percents, 3 - Width in Percents with adjustment. 
	int _cell_action; // Indicates a cell number(1-Based) thar causes an action onclick(). 
	int _cell_uis[32]; 
	int _cell_offsets[32]; 
	int _cell_borders[32]; // 1 - Rectangle, 2 - Vertical Lines, 3 - Horizontal Lines.
	BYTE _cell_canhighlight[32]; 
	BYTE _cell_hyperlink[32]; 
	int _bitmapRow; 
	int _cell_A_bitmapindices[32]; // when active. 
	int _cell_P_bitmapindices[32]; // when passive. 
	BYTE _cell_rightAlignments[32]; 
	BYTE _cell_editflag[32]; 
	BYTE _cell_buttonflag[32]; 
	BYTE _cell_select_onedit[32]; 
	XHColor _cell_backcolor[32]; 
	XHColor _cell_textcolor[32]; 

	std::string _cell_names[32]; 

	std::string _row_name; 

	CHFormatItem(); 

	int find_ncell(int ui); 
}; 


BEGIN_WSI_SERIALIZATION_OBJECT(CHFormatItem)

CONTAINS_FLAT_MEMBER(_offsetUnit, OffsetUnit)
CONTAINS_FLAT_MEMBER(_cell_action, CellAction)
CONTAINS_FLAT_MEMBER(_cell_uis, CellUi)
CONTAINS_FLAT_MEMBER(_cell_offsets, CellOffset)
CONTAINS_FLAT_MEMBER(_cell_borders, CellBorder)
CONTAINS_FLAT_MEMBER(_cell_canhighlight, CellHighlight)
CONTAINS_FLAT_MEMBER(_cell_hyperlink, CellHyperlink)
CONTAINS_FLAT_MEMBER(_bitmapRow, BitmapRow)
CONTAINS_FLAT_MEMBER(_cell_A_bitmapindices, CellBitmapAIndex)
CONTAINS_FLAT_MEMBER(_cell_P_bitmapindices, CellBitmapPIndex)
CONTAINS_FLAT_MEMBER(_cell_rightAlignments, CellRightAlignment)
CONTAINS_FLAT_MEMBER(_cell_editflag, CellEditFlag)
CONTAINS_FLAT_MEMBER(_cell_buttonflag, CellIsButton)
CONTAINS_FLAT_MEMBER(_cell_names, CellName)
CONTAINS_FLAT_MEMBER(_row_name, RowName)
CONTAINS_OBJECT_MEMBER(_cell_backcolor, CellBackColor)
CONTAINS_OBJECT_MEMBER(_cell_textcolor, CellTextColor)
END_WSI_SERIALIZATION_OBJECT()



class EXP_KWIN CHListItem: public CRefCounter, public wsi_reenterantgate { 
	friend class EXP_KWIN KHyperlist; 
	friend class EXP_KWIN CHListHeader; 

	BYTE m_nbLevel; 
	BYTE m_nbOpened; 
	BYTE m_nbAdjusted; 
	LONG m_dwMask; 

	HWND m_hButton[32]; 

	const char * GetText(std::string& text); // All cells - mainly for debug.
	std::string GetText(int nColumn); // 0 - Based 

public: 
	CHListItem(CHFormatItem *pFormat = 0); 
	~CHListItem(); 

	virtual void FreeThis(); 

	virtual void OnAction(HWND hWndCntxt); 
	virtual void OnSelect(HWND hWndCntxt, int nCell/*1 - Based*/); 
	virtual void OnLostSelect(HWND hWndCntxt); 
	virtual BOOL OnClick(HWND hWndCntxt, int nCell/*1 - Based*/); 

	CHFormatItem *m_pFormat; 

	BOOL ScaleColumnOffset(int nColumn/*0 - Based*/, double fScale);  // Only if Units == 3. 

	virtual void GetColumnsOffsets(
		int &nUnits/*0 - Pos. in Pixels, 1 - Width in Pixels, 2 - Width in Percents, 3 - Width in Percents with adjustment*/, 
		int nOffsets[32]);  
	virtual void GetColumnsBorders(int bBorders[32]); 
	virtual void GetColumnsColors(DWORD dwBkColor[32], DWORD dwTextColor[32]); 
	virtual void GetCanHighlight(BYTE bCanHighlight[32]); 
	virtual void GetIsHyperlink(BYTE bIsHyperlink[32]); 
	virtual void GetIsButton(BYTE bIsButton[32]); 
	virtual void GetBitMapIndices(BYTE nbOpened, int &nRow, int nColumns[32]); // 1 - Based. 
	virtual void GetColumnsEditFlags(BYTE nbFlags[32]); 
	virtual BOOL GetText(int nColumn, std::string& text);  // 0 - Based 
	virtual BOOL SetText(int nColumn, const std::string& text); // 0 - Based 
	virtual BOOL IsRightAlignment(int nColumn = 0); // 0 - Based 

	BYTE m_nbHasChild; 
	BYTE m_nbItemType; // 0 - Regular, 1 - Plain Text, 2 - Header(No horiz. lines).  

	virtual CHListItem *FirstChild(); 
	virtual CHListItem *GetNextChild(); 
}; 

class EXP_KWIN CHListHeader: public CHListItem { 
public: 
	void GetColumnsBorders(int bBorders[32]); 
	void GetCanHighlight(BYTE bCanHighlight[32]); 
	void GetIsHyperlink(BYTE bIsHyperlink[32]); 
	void GetIsButton(BYTE bIsButton[32]); 
	void GetBitMapIndices(BYTE nbOpened, int &nRow, int nColumns[32]); // 1 - Based. 
	void GetColumnsEditFlags(BYTE nbFlags[32]); 

	CHListHeader(CHFormatItem *pFormat = 0); 
	~CHListHeader(); 
}; 


class EXP_KWIN CHListEntryField; 

EXPORT_STL_VECTOR(CHListEntryField)
EXPORT_STL_VECTOR(std::string)

#if _MSC_VER < 1600 
#ifdef _DEBUG 
class EXP_KWIN std::_Iterator_base; 
#else 
class EXP_KWIN std::_Iterator_base_aux; 
#endif
template class EXP_KWIN std::_Vector_iterator<CHListEntryField, std::allocator<CHListEntryField>>; 
#else 
EXPORT_TEMPLATE_CLASS(std::vector<CHListEntryField>::iterator)
#endif 


class EXP_KWIN CHListEntryField: public CHListItem { 
public: 
	std::string _ui; 
	std::string _description; // CellUi = 1 
	std::string _value; // CellUi = 2 
	std::string _error; // CellUi = 3 
	std::string _default_value; 

	int _error_number; 

	std::vector<std::string> _error_messages; 

	BOOL _is_readonly; 
	BOOL _preserve_trailing_spaces; 
	BOOL _escape_default; 

	IXFunction<std::string/*arg*/, int> *_validate; 
	BOOL Validate(); 

	std::vector<CHListEntryField> _sub_fields; 
	std::vector<CHListEntryField>::iterator _iter_sub_fields; 


	void GetColumnsOffsets(
		int &nUnits/*0 - Pos. in Pixels, 1 - Width in Pixels, 2 - Width in Percents*/, 
		int nOffsets[32]);  
	void GetColumnsEditFlags(BYTE nbFlags[32]); 
	void GetBitMapIndices(BYTE nbOpened, int &nRow, int nColumns[32]); // 1 - Based. 

	void OnLostSelect(HWND hWndCntxt); 
	BOOL OnClick(HWND hWndCntxt, int nCell/*1 - Based*/); 

	BOOL GetText(int nColumn, std::string& text);  // 0 - Based 
	BOOL SetText(int nColumn, const std::string& text); // 0 - Based 

	CHListItem *FirstChild(); 
	CHListItem *GetNextChild(); 

	CHListEntryField(CHFormatItem *pFormat = 0); 
	~CHListEntryField(); 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(CHListEntryField)

CONTAINS_FLAT_MEMBER(_ui, UI)
CONTAINS_FLAT_MEMBER(_description, Description)
CONTAINS_FLAT_MEMBER(_value, Value)
CONTAINS_FLAT_MEMBER(_default_value, Default)
CONTAINS_FLAT_MEMBER(_error_messages, Error)
CONTAINS_FLAT_MEMBER(_is_readonly, Readonly)
CONTAINS_FLAT_MEMBER(_preserve_trailing_spaces, PreserveSpaces)
CONTAINS_FLAT_MEMBER(m_nbItemType, Type)
CONTAINS_OBJECT_MEMBER(_sub_fields, SubField)
END_WSI_SERIALIZATION_OBJECT()

