
#include "stdafx.h" 

#include "Hyperlist.h"



CHFormatItem::CHFormatItem() { 
	memset(this, 0, (char*)_cell_names - (char*)this); 
} 

int CHFormatItem::find_ncell(int ui) { 
	int *x = _cell_uis; 
	int *y = _cell_uis + ARRAY_NUM_ELEMENTS(_cell_uis); 
	while(x < y && *x != ui) ++x; 
	if(x == y) { 
		return -1; 
	} 
	return x - _cell_uis; 
} 




void CHListHeader::GetColumnsBorders(int bBorders[32]) { 
	std::for_each(&bBorders[0], &bBorders[32], _set_value<int, 1>); 
	if(m_pFormat) { 
		if(m_pFormat->_cell_names[0].size() == 0) { 
			bBorders[0] = 0; 
		} 
	} 
} 

void CHListHeader::GetCanHighlight(BYTE bCanHighlight[32]) { 
	memset(bCanHighlight, 0, 32 * sizeof(BYTE)); 
} 

void CHListHeader::GetIsHyperlink(BYTE bIsHyperlink[32]) { 
	memset(bIsHyperlink, 0, sizeof(BYTE) * 32); 
} 

void CHListHeader::GetIsButton(BYTE bIsButton[32]) { 
	memset(bIsButton, 0, sizeof(BYTE) * 32); 
} 

void CHListHeader::GetBitMapIndices(BYTE nbOpened, int &nRow, int nColumns[32]) {  // 1 - Based. 
	nRow = 0; 
	memset(nColumns, 0, 32 * sizeof(int)); 
} 

void CHListHeader::GetColumnsEditFlags(BYTE nbFlags[32]) { 
	memset(nbFlags, 0, sizeof(BYTE) * 32); 
} 

/* 
 * CHListHeader has zero ref-counter. 
 * A pair of AddRef(), Release() will destroy it. 
 */ 
CHListHeader::CHListHeader(CHFormatItem *pFormat): CHListItem(pFormat) { 
	CRefCounter::_cref = 0; 
	m_nbItemType = 2; // Header - no horizontal lines. 
} 

CHListHeader::~CHListHeader() { 
} 



CHListItem::CHListItem(CHFormatItem *pFormat) : CRefCounter() { 
	m_pFormat = pFormat; 
	m_dwMask = 0; 
	m_nbLevel = 0; 
	m_nbOpened = 0; 
	m_nbHasChild = 0; 
	m_nbItemType = 0; 
	m_nbAdjusted = 0; 

	memset(m_hButton, 0, sizeof(m_hButton)); 
} 

CHListItem::~CHListItem() { 
	FreeThis(); 
} 


void CHListItem::GetColumnsOffsets(
		int &nUnits/*0 - Pos. in Pixels, 1 - Width in Pixels, 2 - Width in Percents*/, 
		int nOffsets[32]) { 
	if(m_pFormat) { 
		memcpy(nOffsets, m_pFormat->_cell_offsets, sizeof(int) * 32); 
		nUnits = m_pFormat->_offsetUnit; 
		return; 
	} 
	nUnits = 0; 
	memset(nOffsets, 0, sizeof(int) * 32); 
} 

BOOL CHListItem::ScaleColumnOffset(int nColumn/*0 - Based*/, double fScale) { 
	if(m_pFormat && !m_nbAdjusted) { 
		if(nColumn > -1 && nColumn < 32) { 
			if(m_pFormat->_offsetUnit == 3) { 
				int nOffset = m_pFormat->_cell_offsets[nColumn]; 
				int nMaxOffset = 100; 
				for(int j = 0; j < 32; ++j) { 
					if(m_pFormat->_cell_offsets[j]) { 
						nMaxOffset -= 1; 
					} 
				} 
				nOffset = (int)((nOffset * fScale) + 0.5) + 1; 
				if(nOffset < nMaxOffset) { 
					double fDelta = nOffset - m_pFormat->_cell_offsets[nColumn]; 
					if(fDelta > 0) { 
						fScale = (100 - fDelta) / 100; 
					} 
					else { 
						fScale = 100 / (100 + fDelta); 
					} 
					m_pFormat->_cell_offsets[nColumn] = nOffset; 
					for(int j = 0; j < 32; ++j) { 
						if(j != nColumn) { 
							int nOffset = (int)((m_pFormat->_cell_offsets[j] * fScale) + 0.5); 
							if(nOffset > 1) { 
								m_pFormat->_cell_offsets[j] = nOffset; 
							} 
						} 
					} 
					m_nbAdjusted = 1; 
					return TRUE; 
				} 
			} 
		} 
	} 
	return FALSE; 
} 

void CHListItem::FreeThis() { 
} 

void CHListItem::GetColumnsBorders(int bBorders[32]) { 
	if(m_pFormat) { 
		memcpy(bBorders, m_pFormat->_cell_borders, sizeof(int) * 32); 
		return; 
	} 
	memset(bBorders, 0, sizeof(bool) * 32); 
} 

void CHListItem::GetColumnsColors(DWORD dwBkColor[32], DWORD dwTextColor[32]) { 
	if(m_pFormat) { 
		memcpy(dwBkColor, m_pFormat->_cell_backcolor, sizeof(DWORD) * 32); 
		memcpy(dwTextColor, m_pFormat->_cell_textcolor, sizeof(DWORD) * 32); 
		return; 
	} 
	memset(dwBkColor, 0, sizeof(DWORD) * 32); 
	memset(dwTextColor, 0, sizeof(DWORD) * 32); 
} 

void CHListItem::GetCanHighlight(BYTE bCanHighlight[32]) { 
	if(m_pFormat) { 
		memcpy(bCanHighlight, m_pFormat->_cell_canhighlight, sizeof(BYTE) * 32); 
		return; 
	} 
	memset(bCanHighlight, 0, sizeof(BYTE) * 32); 
} 

void CHListItem::GetIsHyperlink(BYTE bIsHyperlink[32]) { 
	if(m_pFormat) { 
		memcpy(bIsHyperlink, m_pFormat->_cell_hyperlink, sizeof(BYTE) * 32); 
		return; 
	} 
	memset(bIsHyperlink, 0, sizeof(BYTE) * 32); 
} 

void CHListItem::GetIsButton(BYTE bIsButton[32]) { 
	if(m_pFormat) { 
		memcpy(bIsButton, m_pFormat->_cell_buttonflag, sizeof(BYTE) * 32); 
		return; 
	} 
	memset(bIsButton, 0, sizeof(BYTE) * 32); 
} 

void CHListItem::GetBitMapIndices(BYTE nbOpened, int &nRow, int nColumns[32]) { 
	nRow = m_nbLevel + 1; 

	if(m_pFormat) { 
		if(nbOpened) { 
			memcpy(nColumns, m_pFormat->_cell_A_bitmapindices, sizeof(int) * 32); 
		} 
		else { 
			memcpy(nColumns, m_pFormat->_cell_P_bitmapindices, sizeof(int) * 32); 
		} 
		if(!m_nbHasChild) { 
			int cell_action = m_pFormat->_cell_action; 
			if(cell_action > 0 && cell_action <= 32) { 
				nColumns[cell_action - 1] = 0; 
			} 
		} 
		return; 
	} 
	memset(nColumns, 0, 32 * sizeof(int)); 
} 

void CHListItem::GetColumnsEditFlags(BYTE nbFlags[32]) { 
	if(m_pFormat) { 
		memcpy(nbFlags, m_pFormat->_cell_editflag, sizeof(BYTE) * 32); 
		return; 
	} 
	memset(nbFlags, 0, sizeof(BYTE) * 32); 
} 

BOOL CHListItem::IsRightAlignment(int nColumn) { 
	if(m_pFormat) { 
		return  m_pFormat->_cell_rightAlignments[nColumn]; 
	} 
	return false; 
} 


const char * CHListItem::GetText(std::string& otext) { // All cells 
	std::ostringstream stext; 
	std::string text; 
	for(int j = 0; j < 32; ++j) { 
		if(GetText(j, text)) { 
			stext << text << ' '; 
		} 
	} 

	if(stext.str().size()) { 
		stext.str().swap(otext); 
	} 
	else { 
		otext.resize(0); 
	} 

	return otext.c_str(); 
} 

std::string CHListItem::GetText(int nColumn) { // 0 - Based 
	std::string text; 
	GetText(nColumn, text); 
	return text; 
} 

BOOL CHListItem::GetText(int nColumn, std::string& text) { // 0 - Based. 
	if(m_pFormat) { 
		text = m_pFormat->_cell_names[nColumn]; 
	} 
	else { 
		text.resize(0); 
	} 

	return text.size() != 0; 
} 

BOOL CHListItem::SetText(int nColumn, const std::string& text) { 
	return FALSE; 
} 

void CHListItem::OnAction(HWND hWndCntxt) { 
} 

void CHListItem::OnSelect(HWND hWndCntxt, int nCell) { // 1 - Based 
} 

void CHListItem::OnLostSelect(HWND hWndCntxt) { 
} 

BOOL CHListItem::OnClick(HWND hWndCntxt, int nCell) { // 1 - Based 
	BOOL rc = FALSE; 
	if(m_pFormat) { 
		if(m_pFormat->_cell_action == nCell) { 
			rc = TRUE; 
		} 
	} 
	return rc; 
} 



CHListItem * CHListItem::FirstChild() { 
	return NULL; 
} 

CHListItem * CHListItem::GetNextChild() { 
	return NULL; 
} 



/*
 * nUnits = 0 - Pos. in Pixels, 
 * nUnits = 1 - Width in Pixels, 
 * nUnits = 2 - Width in Percents. 
 */
void CHListEntryField::GetColumnsOffsets(int &nUnits, int nOffsets[32]) { 
	if(m_pFormat) { 
		memcpy(nOffsets, m_pFormat->_cell_offsets, sizeof(int) * 32); 
		nUnits = m_pFormat->_offsetUnit; 
		return; 
	} 
	nUnits = 0; 
	memset(nOffsets, 0, sizeof(int) * 32); 
} 

void CHListEntryField::GetBitMapIndices(BYTE nbOpened, int &nRow, int nColumns[32]) { // 1 - Based. 
	if(_sub_fields.size()) { 
		m_nbHasChild = 1; 
	} 

	CHListItem::GetBitMapIndices(nbOpened, nRow, nColumns); 
} 

void CHListEntryField::GetColumnsEditFlags(BYTE nbFlags[32]) { 
	if(!_is_readonly && m_pFormat) { 
		memcpy(nbFlags, m_pFormat->_cell_editflag, sizeof(BYTE) * 32); 
		return; 
	} 
	memset(nbFlags, 0, sizeof(BYTE) * 32); 
} 

BOOL CHListEntryField::Validate() { 
	int err = 0; 
	if(_validate) { 
		if(_escape_default && _value == _default_value) { 
			std::string value; 
			err = (*_validate)(value); 
		} 
		else { 
			if(!_preserve_trailing_spaces) { 
				XTrimString(_value); 
			} 
			err = (*_validate)(_value); 
		} 
	} 
	if(err) { 
		if(_error_messages.size() >= (size_t)err) { 
			_error = _error_messages[err -1]; 
		} 
	} 
	else { 
		_error.resize(0); 
	} 

	_error_number = err; 

	return err == 0; 
} 

void CHListEntryField::OnLostSelect(HWND hWndCntxt) { 
	Validate(); 
} 

BOOL CHListEntryField::OnClick(HWND hWndCntxt, int nCell/*1 - Based*/) { 
	BOOL rc = FALSE; 
	if(m_pFormat) { 
		if(m_pFormat->_cell_action == nCell) { 
			rc = TRUE; 
		} 
	} 

	if(!Validate()) {  
		rc = TRUE; 
	} 

	return rc; 
} 

CHListItem *CHListEntryField::FirstChild() { 
	_iter_sub_fields = _sub_fields.begin(); 
	return GetNextChild(); 
} 

CHListItem *CHListEntryField::GetNextChild() { 
	CHListItem *pItem = 0; 
	if(_iter_sub_fields != _sub_fields.end()) { 
		pItem = &(*_iter_sub_fields); 
		_iter_sub_fields++; 
	} 
	return pItem; 
} 

BOOL CHListEntryField::GetText(int nColumn, std::string& text) { // 0 - Based 
	if(m_pFormat) { 
		m_pFormat->_cell_select_onedit[nColumn] = 0; 

		if(!_escape_default && _default_value.size() && _default_value[0] == '#') { 
			_escape_default = TRUE; 
			_default_value = _default_value.c_str() + 1; 
			DecodeHtmlentities(_default_value); 
		} 
		if(m_pFormat->_cell_uis[nColumn] == 1/*Description*/) { 
			text = _description; 
		} 
		else 
		if(m_pFormat->_cell_uis[nColumn] == 2/*Value*/) { 
			text = _value; 
			if(text.size() == 0) { 
				text = _default_value; 
				if(!_escape_default) { 
					_value = text; 
				} 
				Validate(); 
			} 
			if(_escape_default && text == _default_value) { 
				m_pFormat->_cell_select_onedit[nColumn] = 1; 
			} 
		} 
		else 
		if(m_pFormat->_cell_uis[nColumn] == 3/*Error*/) { 
			text = _error; 
		} 
		else { 
			text = m_pFormat->_cell_names[nColumn]; 
		} 
	} 
	else { 
		text.resize(0); 
	} 

	return text.size() != 0; 
} 

BOOL CHListEntryField::SetText(int nColumn, const std::string& text) { // 0 - Based 
	BOOL rc = FALSE; 
	if(m_pFormat) { 
		if(m_pFormat->_cell_uis[nColumn] == 1/*Description*/) { 
			_description = text; 
			rc = TRUE; 
		} 
		else 
		if(m_pFormat->_cell_uis[nColumn] == 2/*Value*/) { 
			if(_value != text) { 
				_value = text; 
				Validate(); 
				rc = TRUE; 
			} 
		} 
		else 
		if(m_pFormat->_cell_uis[nColumn] == 3/*Error*/) { 
			_error = text; 
			rc = TRUE; 
		} 
	} 

	return rc; 
} 

CHListEntryField::CHListEntryField(CHFormatItem *pFormat): CHListItem(pFormat) { 
	_validate = 0; 
	_is_readonly = FALSE; 
	_preserve_trailing_spaces = FALSE; 
	_error_number = 0; 
	_escape_default = FALSE; 
} 

CHListEntryField::~CHListEntryField() { 
} 


