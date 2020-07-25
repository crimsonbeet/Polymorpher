#pragma once

#pragma comment( user, "Created by Alex Nickol Bourakov on 2008/11/10")
#include <commctrl.h>   // includes the common control header
#include "IPCGate.h"


#include "dib.h"


PBITMAPINFO EXP_KWIN KWinLoadBitmapFile(const char *pFileName); 


typedef struct _BitmapDim { 
	HANDLE _hbitmap/*in*/; 

	int _isize/*out*/; 
	int _numbitmaps/*out*/; 

	int _width/*out*/; 
	int _height/*out*/; 

	COLORREF _backcolor; // lower left corner is a pixel in the background. 

	_BitmapDim() { 
		memset(this, 0, sizeof(_BitmapDim)); 
	} 

} SBitmapDim, *PBitmapDim; 

BOOL EXP_KWIN KWinEvaluateBitmapDimensions(SBitmapDim& params); 



// structure that controls creation of an image list. 

VOID EXP_KWIN KWinDestroyImageList(HIMAGELIST& himl); 


typedef struct _ImageList { 
	std::vector<std::string> _images; // file names that contain bit-maps. 
	HIMAGELIST _himl; 
	std::vector<int> _indices; // indices of bit maps loaded to image list. 

	int _imgwidth; 
	int _imgheight; 


	_ImageList() { 
		_himl = 0; 
		_imgwidth = 0; 
		_imgheight = 0; 
	} 

	~_ImageList() { 
		KWinDestroyImageList(_himl); 
	} 

} SImageList, *PSImageList; 


typedef struct _CreateImageList { 
	std::vector<std::string> _images/*in*/; // file names that contain bit-maps. 
	int _imgwidth/*in*/; // Width, in pixels, of each image. If zero, then it will be calculated from bitmaps to load. 
	int _imgheight/*in*/; 

	UINT _creationflags/*in*/; 
	bool _masked/*in*/; 

	_CreateImageList() { 
		_imgwidth = 0; 
		_imgheight = 0; 
		_creationflags = 0; 
		_masked = 1; 
	} 

} SCreateImageList, *PSCreateImageList; 

BOOL EXP_KWIN KWinCreateImageList(const SCreateImageList& params, SImageList& iml); 

BOOL EXP_KWIN KWinChangeMaskedImageList(const std::string names[], SImageList& iml, int names_cnt); 




typedef struct _ReBar { 
	HWND _hwndrebar; 
	int _idrebar; 

	WPARAM _wparam; 
	LPARAM _lparam; 
	LRESULT _lresult; 
	IXDelegate<_ReBar, void> _wmcommand_delegates; 
	IXDelegate<_ReBar, void> _wmnotify_delegates; 

	int _fbandstyle; // current style for rebar band. It is used to create individual style rebar band. 

	_ReBar() { 
		_hwndrebar = 0; 
		_idrebar = 0; 

		_wparam = 0; 
		_lparam = 0; 
		_lresult = 0; 

		_fbandstyle = RBBS_NOVERT/*do not display in vertical orientation*/ /*| RBBS_NOGRIPPER */| RBBS_CHILDEDGE/*band has an edge at the top and bottom of the child window*/; 
	} 

} SReBar, *PSReBar; 


typedef struct _CreateRebar { 
	PSImageList _piml/*in*/; 
	HWND _hwndparent/*in*/; 
	int _idrebar/*in*/; // rebar window identifier. 

	HWND _hwndrebar/*out*/; 

	_CreateRebar() { 
		memset(this, 0, sizeof(_CreateRebar)); 
	} 
} SCreateRebar, *PSCreateRebar; 


//BOOL EXP_KWIN KWinCreateRebar(SCreateRebar& params, PSReBar *pprebar = 0); 
BOOL EXP_KWIN KWinCreateRebar(SCreateRebar& params, std::auto_ptr<SReBar> *pprebar = 0); 





typedef struct _CreateRebarBand { 
	HWND _hwndrebar/*in*/; 
	HWND _hwndchild/*in*/; 

	int _fstyle; 

	std::string _backgroundimg/*in*/; 
	COLORREF _clrfore/*in*/; 
	COLORREF _clrback/*in*/; 

	int _childminwidth/*in*/; 
	int _childminheight/*in*/; 
	int _cxideal/*in*/; 

	std::string _text/*in*/; 
	int _idximage/*in*/; 

	int _rebarbandid/*out*/; 

	_CreateRebarBand() { 
		_fstyle = 0; 
		_clrfore = GetSysColor(COLOR_BTNTEXT); 
		_clrback = GetSysColor(COLOR_BTNFACE); 
		_hwndrebar = NULL; 
		_hwndchild = NULL; 
		_childminwidth = 0; 
		_childminheight = 0; 
		_cxideal = 0; 
		_idximage = -1; 
		_rebarbandid = -1; 
	} 

} SCreateRebarBand, *PSCreateRebarBand; 


BOOL EXP_KWIN KWinCreateRebarBand(SCreateRebarBand& params); 




typedef struct _ComboBox { 
	HWND _hwndcombo; 

	int _minwidth; 
	int _minheight;

	int _iselect; 
	std::string _strselect; 
	IXDelegate<_ComboBox, void> _selchanged_delegates; 

	_ComboBox() { 
		_hwndcombo = 0; 
		_iselect = -1; 
	} 

} SComboBox, *PSComboBox; 


typedef struct _CreateComboBox { 
	PSImageList _piml; 
	std::vector<std::string> _strings; // strings that correspond to file names in imagelist. 
	HWND _hwndparent/*in*/; 

	int _idcombo/*in*/; // combobox window identifier. 

	int _minwidth; // Minimal Width, in pixels. If zero, then it will be taken from the image list.
	int _dropdownheight; // height when it dropped down. 

	HWND _hwndcombo/*out*/; 

	_CreateComboBox() { 
		_hwndparent = 0; 
		_idcombo = 0; 
		_minwidth = 0; 
		_dropdownheight = 0; 
	} 

} SCreateComboBox, *PSCreateComboBox; 


//BOOL EXP_KWIN KWinCreateComboBox(SCreateComboBox& params, PSComboBox *ppcombo = 0); 
BOOL EXP_KWIN KWinCreateComboBox(SCreateComboBox& params, std::auto_ptr<SComboBox> *ppcombo = 0); 



// _ToolBarButton works together with _CreateToolBar. 

typedef struct _ToolBarButton { 
	std::string _text; 
	std::string _tiptext; 

	BYTE _state; 

	_ToolBarButton() { 
		_state = TBSTATE_ENABLED; 
	} 

} SToolBarButton, *PSToolBarButton; 


typedef struct _ToolBar { 
	HWND _hwndtoolbar; 
	int _rebarbandid; 

	std::auto_ptr<SImageList> _piml; 
	std::auto_ptr<SImageList> _pimlhot; 
	std::auto_ptr<SImageList> _pimldisabled; 

	int _idtoolbar; // window identifier. 
	int _idfirstbutton; // id of first button in _buttons array. Consiquent button ids are incremented. 
	int _nbuttons; 

	std::vector<SToolBarButton> _buttons/*in*/; 

	BOOL GetButtonInfoByindex(size_t buttonindex, TBBUTTONINFO& buttoninfo) { 
		BOOL rc = FALSE; 
		if(_hwndtoolbar) { 
			if(buttonindex < _buttons.size()) { 
				buttoninfo.cbSize = sizeof(buttoninfo); 
				buttoninfo.dwMask = TBIF_BYINDEX | TBIF_COMMAND | TBIF_IMAGE | TBIF_LPARAM | TBIF_SIZE | TBIF_STATE | TBIF_STYLE; 
				if(buttonindex == SendMessage(_hwndtoolbar, TB_GETBUTTONINFO, buttonindex, (LPARAM)&buttoninfo)) { 
					rc = TRUE; 
				} 
			} 
		} 
		return rc; 
	} 

	BOOL SetButtonInfoByindex(TBBUTTONINFO& buttoninfo, size_t buttonindex) { 
		BOOL rc = FALSE; 
		if(_hwndtoolbar) { 
			if(buttonindex < _buttons.size()) { 
				if(buttoninfo.cbSize == sizeof(buttoninfo)) { 
					buttoninfo.dwMask |= TBIF_BYINDEX; 
					if(SendMessage(_hwndtoolbar, TB_SETBUTTONINFO, buttonindex, (LPARAM)&buttoninfo)) { 
						rc = TRUE; 
					} 
				} 
			} 
		} 
		return rc; 
	} 

	BOOL SetButtonStateByindex(BYTE fstate, size_t buttonindex, bool remove_style = false) { 
		BOOL rc = FALSE; 
		if(_hwndtoolbar) { 
			if(buttonindex < _buttons.size()) { 
				TBBUTTONINFO buttoninfo; 
				memset(&buttoninfo, 0, sizeof(buttoninfo)); 
				if(GetButtonInfoByindex(buttonindex, buttoninfo)) { 
					buttoninfo.dwMask = TBIF_STATE; 
					if(remove_style) { 
						buttoninfo.fsState &= ~fstate; 
					} 
					else { 
						buttoninfo.fsState |= fstate; 
					} 
					SetButtonInfoByindex(buttoninfo, buttonindex); 
				} 
			} 
		} 
		return rc; 
	} 

	BOOL SetButtonStyleByindex(int fstyle, size_t buttonindex, bool remove_style = false) { 
		BOOL rc = FALSE; 
		if(_hwndtoolbar) { 
			if(buttonindex < _buttons.size()) { 
				TBBUTTONINFO buttoninfo; 
				memset(&buttoninfo, 0, sizeof(buttoninfo)); 
				if(GetButtonInfoByindex(buttonindex, buttoninfo)) { 
					buttoninfo.dwMask = TBIF_STYLE; 
					if(remove_style) { 
						buttoninfo.fsStyle &= ~fstyle; 
					} 
					else { 
						buttoninfo.fsStyle |= fstyle; 
					} 
					SetButtonInfoByindex(buttoninfo, buttonindex); 
				} 
			} 
		} 
		return rc; 
	} 

	int _buttonwidth; 
	int _buttonheight; 

	_ToolBar() { 
		_hwndtoolbar = 0; 
		_rebarbandid = -1; 
		_idtoolbar = 0; 
		_idfirstbutton = 0; 
		_nbuttons = 0; 
		_buttonwidth = 0; 
		_buttonheight = 0; 
	} 

} SToolBar, *PSToolBar; 


typedef struct _CreateToolBar { 
	HWND _hwndparent/*in*/; 

	std::vector<std::string> _siml/*in*/; 
	std::vector<std::string> _simlhot/*in*/; 
	std::vector<std::string> _simldisabled/*in*/; 

	std::vector<SToolBarButton> _buttons/*in*/; 

	int _idtoolbar/*in*/; // window identifier. 
	int _idfirstbutton/*in*/; // id of first button in _buttons array. Consiquent button ids are incremented. 

	_CreateToolBar() { 
		_hwndparent = 0; 
		_idtoolbar = 0; 
		_idfirstbutton = 0; 
	} 

} SCreateToolBar, *PSCreateToolBar; 


BOOL EXP_KWIN KWinCreateToolBar(SCreateToolBar& params, SToolBar& toolbar); 

