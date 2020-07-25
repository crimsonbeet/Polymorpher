#pragma comment( user, "Created by Alex Nickol Bourakov on 2008/11/07")
#include "stdafx.h"
#include "controls.h"  


BOOL EXP_KWIN KWinEvaluateBitmapDimensions(SBitmapDim& params) { 
	if(params._hbitmap == NULL) { 
		return FALSE; 
	} 

//	If LoadImage was used, then it had to be used with LR_CREATEDIBSECTION. 
	DIBSECTION dibsec; 
	if(!GetObject(params._hbitmap, sizeof(DIBSECTION), &dibsec)) { 
		return FALSE; 
	} 

	KDIB kdib; 
	kdib.Attach((LPBITMAPINFO)&dibsec.dsBmih, (LPBYTE)dibsec.dsBm.bmBits, DIB_BMI_READONLY | DIB_BITS_READONLY); 

	params._height = kdib._height; 
	params._width = kdib._width; 

	params._isize = params._width; 
	if(params._width < params._height) { 
		params._isize = params._height; 
	} 

	params._numbitmaps = params._height? (params._isize / params._height): 0; 

	params._backcolor = kdib.GetPixelRGB(0, params._height - 1); 

	return TRUE; 
} 

PBITMAPINFO EXP_KWIN KWinLoadBitmapFile(const char *pFileName) { 
	if(!pFileName) { 
		return 0; 
	} 

	HANDLE handle = CreateFile(pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
	if(handle == INVALID_HANDLE_VALUE) { 
		return 0; 
	} 

	PBITMAPINFO pbminfo = 0; 

    BITMAPFILEHEADER bmfileheader;

	DWORD dw = 0;
	ReadFile(handle, &bmfileheader, sizeof(bmfileheader), &dw, NULL); 

	if((bmfileheader.bfType == 0x4D42) && (bmfileheader.bfSize <= GetFileSize(handle, NULL))) { 
		pbminfo = (PBITMAPINFO) new BYTE[bmfileheader.bfSize]; 
		if(pbminfo) {
			ReadFile(handle, pbminfo, bmfileheader.bfSize, &dw, NULL);
		}
	}
	CloseHandle(handle);

	return pbminfo;
}




// structure that controls creation of an image list. 

BOOL EXP_KWIN KWinCreateImageList(const SCreateImageList& params, SImageList& iml) { 
	int imgwidth = params._imgwidth; 
	int imgheight = params._imgheight; 

	std::vector<HANDLE> bitmaps; 
	std::vector<COLORREF> backcolors; 

	bitmaps.reserve(iml._images.size()); 
	backcolors.reserve(iml._images.size()); 

	for(size_t x = 0; x < params._images.size(); ++x) { 
		HANDLE hbmp = LoadImage(NULL, params._images[x].c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED | LR_CREATEDIBSECTION); 
		SBitmapDim bitmapdim; 
		if(hbmp) { 
			bitmapdim._hbitmap = hbmp; 
			if(KWinEvaluateBitmapDimensions(bitmapdim)) { 
				if(params._imgwidth <= 0 && imgwidth < bitmapdim._width) { 
					imgwidth = bitmapdim._width; 
				} 
				if(params._imgheight <= 0 && imgheight < bitmapdim._height) { 
					imgheight = bitmapdim._height; 
				} 
			} 
		} 

		bitmaps.push_back(hbmp); 
		backcolors.push_back(bitmapdim._backcolor); 
	} 

	iml._himl = ImageList_Create ( 
		imgwidth, 
		imgheight, 
		params._creationflags | (params._masked? ILC_MASK: 0), 
		(int)params._images.size(),	// Number of images that the image list initially contains 
		1); // Number of images by which the image list can grow 

	if(iml._himl == NULL) { 
		return FALSE; 
	} 

	iml._images = params._images; 
	iml._indices.reserve(params._images.size()); 
	iml._imgwidth = imgwidth; 
	iml._imgheight = imgheight; 

	for(size_t x = 0; x < bitmaps.size(); ++x) { 
		if(params._masked) { 
			iml._indices.push_back(ImageList_AddMasked(iml._himl, (HBITMAP)(bitmaps[x]), backcolors[x])); 
		} 
		else { 
			iml._indices.push_back(ImageList_Add(iml._himl, (HBITMAP)(bitmaps[x]), NULL)); 
		} 
		DeleteObject(bitmaps[x]); 
	} 

	return TRUE; 
} 

VOID EXP_KWIN KWinDestroyImageList(HIMAGELIST& himl){ 
	if(himl) { 
		ImageList_Destroy(himl); 
		himl = 0; 
	} 
} 


BOOL EXP_KWIN KWinChangeMaskedImageList(const std::string names[], SImageList& iml, int names_cnt) {
	if(iml._himl == NULL) {
		return FALSE;
	}

	std::vector<HANDLE> bitmaps;
	std::vector<COLORREF> backcolors;

	bitmaps.reserve(names_cnt);
	backcolors.reserve(names_cnt);

	ImageList_Remove(iml._himl, -1);
	iml._indices.resize(0);
	iml._images.resize(0); 

	for(size_t x = 0; x < (size_t)names_cnt; ++x) {
		HANDLE hbmp = LoadImage(NULL, names[x].c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED | LR_CREATEDIBSECTION);
		SBitmapDim bitmapdim;
		if(hbmp) {
			bitmapdim._hbitmap = hbmp;
			KWinEvaluateBitmapDimensions(bitmapdim);
		}

		bitmaps.push_back(hbmp);
		backcolors.push_back(bitmapdim._backcolor);

		iml._images.push_back(names[x]);
	}

	for(size_t x = 0; x < bitmaps.size(); ++x) {
		iml._indices.push_back(ImageList_AddMasked(iml._himl, (HBITMAP)(bitmaps[x]), backcolors[x]));
		DeleteObject(bitmaps[x]);
	}

	return TRUE;
}





// Window procedure for rebar subclassing
WNDPROC lpfnDefRebar;

LRESULT CALLBACK KWinRebarWndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam) { 
	if(uMessage == WM_COMMAND) { 
		PSReBar prebar = (PSReBar)(LONG_PTR)GetWindowLongPtr(hwnd, GWLP_USERDATA); 
		if(prebar) { 
			prebar->_wparam = wParam; 
			prebar->_lparam = lParam; 
			prebar->_wmcommand_delegates(*prebar); 
			return 0; 
		} 
	} 

	if(uMessage == WM_NOTIFY) { 
		PSReBar prebar = (PSReBar)(LONG_PTR)GetWindowLongPtr(hwnd, GWLP_USERDATA); 
		if(prebar) { 
			prebar->_wparam = wParam; 
			prebar->_lparam = lParam; 
			prebar->_wmnotify_delegates(*prebar); 
			return prebar->_lresult; 
		} 
	} 

	if(uMessage == WM_NOTIFYFORMAT) { 
#if defined(_UNICODE)
		return NFR_UNICODE; 
#else
		return NFR_ANSI; 
#endif
	} 

	if(uMessage == WM_SIZE) { 
		RECT clrect; 
		HWND hparent = GetParent(hwnd); 
		if(GetClientRect(hparent, &clrect)) { 
			RECT winrect; 
			GetWindowRect(hwnd, &winrect); 
			POINT actual_origin = { winrect.left, winrect.top }; 
			ScreenToClient(hparent, &actual_origin); 

			int inewheight = actual_origin.y + /*HIWORD(lParam)*/winrect.bottom - winrect.top; 
			int iheight = clrect.bottom - clrect.top; 
			if(inewheight > iheight) { 
				int iwidth = clrect.right - clrect.left; 
				iheight = inewheight; 
				
				PostMessage(hparent, WM_SIZE, wParam, MAKELPARAM(iwidth, iheight)); 
			} 
		} 
	} 

	return (CallWindowProc(lpfnDefRebar, hwnd, uMessage, wParam, lParam));
}


//BOOL EXP_KWIN KWinCreateRebar(SCreateRebar& params, PSReBar *pprebar) { 
BOOL EXP_KWIN KWinCreateRebar(SCreateRebar& params, std::auto_ptr<SReBar> *pprebar) { 
	int iwidth = 0; 
	RECT clrect; 
	if(GetClientRect(params._hwndparent, &clrect)) { 
		iwidth = clrect.right - clrect.left; 
	} 
	params._hwndrebar = CreateWindowEx(0, REBARCLASSNAME, NULL, 
		WS_VISIBLE | 
		WS_BORDER | 
		WS_CHILD | 
		WS_CLIPCHILDREN | 
		WS_CLIPSIBLINGS | 
		CCS_NODIVIDER | 
		CCS_NOPARENTALIGN | 
//		RBS_BANDBORDERS | 
		RBS_AUTOSIZE | 
		RBS_FIXEDORDER | 
		RBS_VARHEIGHT, 
		0, 0, iwidth/2, 0, // do not take the entire client width of the parent, otherwise the rebar band assumes maximum width. 
		params._hwndparent, 
		(HMENU)params._idrebar, 
		(HINSTANCE)GetModuleHandle(0), 
		NULL 
	); 

	if(params._hwndrebar == NULL) {
		return FALSE; 
	} 

	if(params._piml) { 
		REBARINFO rbi;
		rbi.cbSize = sizeof(rbi);
		rbi.fMask = RBIM_IMAGELIST;
		rbi.himl = params._piml->_himl;
		SendMessage(params._hwndrebar, RB_SETBARINFO, 0L, (LPARAM)&rbi); 
	} 

	if(pprebar) { 
		(*pprebar) = std::auto_ptr<SReBar>(new SReBar()); 
		(*pprebar)->_hwndrebar = params._hwndrebar; 
		(*pprebar)->_idrebar = params._idrebar; 
		SetWindowLongPtr(params._hwndrebar, GWLP_USERDATA, (LONG_PTR)(*pprebar).get());

		lpfnDefRebar = (WNDPROC)GetWindowLongPtr(params._hwndrebar, GWLP_WNDPROC); 
		SetWindowLongPtr(params._hwndrebar, GWLP_WNDPROC, (LONG_PTR)KWinRebarWndProc); 
	} 

	return TRUE; 
}





BOOL EXP_KWIN KWinCreateRebarBand(SCreateRebarBand& params) { 
	if(params._hwndrebar == NULL) { 
		return FALSE; 
	} 

	HANDLE hbackground = NULL; 
	if(params._backgroundimg.size()) { 
		hbackground = LoadImageA(NULL, params._backgroundimg.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_SHARED | LR_CREATEDIBSECTION); 
	} 

	REBARBANDINFO rbBand; 
	rbBand.cbSize = sizeof(REBARBANDINFO); 
	rbBand.fMask = 
		RBBIM_COLORS |				// clrFore and clrBack are valid
		RBBIM_CHILD |				// hwndChild is valid
		RBBIM_CHILDSIZE |			// cxMinChild and cyMinChild are valid
		RBBIM_IDEALSIZE |			// cxIdeal is valid 
		RBBIM_STYLE |				// fStyle is valid
		RBBIM_ID |					// wID is valid
//		RBBIM_TEXT |				// lpText is valid
		(hbackground? RBBIM_BACKGROUND: 0) | // hbmBack is valid
		(params._idximage >= 0? RBBIM_IMAGE: 0); // iImage is valid
	rbBand.clrFore = params._clrfore; 
	rbBand.clrBack = params._clrback; 
	rbBand.fStyle = 
		(hbackground? RBBS_FIXEDBMP: 0) | // background bitmap does not move when the band is resized 
		params._fstyle; 
	rbBand.hbmBack = (HBITMAP)hbackground; 
	rbBand.lpText = (LPSTR)params._text.c_str(); 
	rbBand.cch = 0; // Size of the buffer at lpText, in bytes. If information is not being requested from the control, this member is ignored. 
	rbBand.hwndChild = params._hwndchild; 
	rbBand.cxMinChild = params._childminwidth; 
	rbBand.cyMinChild = params._childminheight; 
	rbBand.cyChild = params._childminheight; 
	rbBand.cxIdeal = params._cxideal > 0? params._cxideal: params._childminwidth; 
	rbBand.cyIntegral = 10; 
	rbBand.iImage = params._idximage; 

	// Add the combo box band to the end
	if(!SendMessage(params._hwndrebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand)) { 
		return FALSE; 
	} 

	int band_count = (int)SendMessage(params._hwndrebar, RB_GETBANDCOUNT, 0, 0); 
	params._rebarbandid = band_count - 1; 

	if(params._cxideal) { 
//		SendMessage(params._hwndrebar, RB_MINIMIZEBAND, (WPARAM)params._rebarbandid, (LPARAM)0); 
		SendMessage(params._hwndrebar, RB_MAXIMIZEBAND, (WPARAM)params._rebarbandid, (LPARAM)TRUE/*use ideal size*/); 
	} 

	return TRUE;
}





// Window procedure for combo box subclassing
WNDPROC lpfnDefCombo; 

LRESULT CALLBACK KWinComboWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	if((uMessage == WM_COMMAND) && (HIWORD(wParam) == CBN_SELCHANGE))	{ 
		int iselect = (int)SendMessage(hWnd, CB_GETCURSEL, 0, 0); 
		if(iselect != CB_ERR) { 
			PSComboBox pcombo = (PSComboBox)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA); 
			if(pcombo) { 
				int ilength = (int)SendMessage(hWnd, CB_GETLBTEXTLEN, (WPARAM)iselect, 0); 
				if(ilength != CB_ERR) { 
					pcombo->_iselect = iselect; 
					pcombo->_strselect.resize(ilength + 1); 
					SendMessage(hWnd, CB_GETLBTEXT, (WPARAM)iselect, (LPARAM)(LPTSTR)pcombo->_strselect.data()); 
					pcombo->_selchanged_delegates(*pcombo); 
					return 0; 
				} 
			} 
		} 
	}

	return (CallWindowProc(lpfnDefCombo, hWnd, uMessage, wParam, lParam));
}


//BOOL EXP_KWIN KWinCreateComboBox(SCreateComboBox& params, PSComboBox *ppcombo) { 
BOOL EXP_KWIN KWinCreateComboBox(SCreateComboBox& params, std::auto_ptr<SComboBox> *ppcombo) { 
	int minwidth = params._minwidth; 
	if(minwidth <= 0) { 
		if(params._piml) { 
			minwidth = params._piml->_imgwidth; 
		} 
	} 

	params._hwndcombo = CreateWindowEx(
		0,									// extended styles
		"ComboBoxEx32",			            // extended combo box
		NULL, 								// default text
		WS_VISIBLE | 
		WS_CHILD | 
		WS_TABSTOP |
		WS_VSCROLL | 
		WS_CLIPCHILDREN | 
		WS_CLIPSIBLINGS | 
		CCS_NORESIZE | 
		CBS_AUTOHSCROLL | 
		CBS_DROPDOWNLIST,
		CW_USEDEFAULT,						// x 
		CW_USEDEFAULT,						// y 
		minwidth, 
		params._dropdownheight, 
		params._hwndparent,					// parent window 
		(HMENU)params._idcombo, 
		(HINSTANCE)GetModuleHandle(0), 
		NULL);								// no class data 

	if(params._hwndcombo == NULL) { 
		return FALSE; 
	} 

	if(params._piml) { 
		SendMessage(params._hwndcombo, CBEM_SETIMAGELIST, 0L, (LPARAM)params._piml->_himl); 
	} 

	std::vector<std::string>::iterator iter = params._strings.begin(); 
	while(iter != params._strings.end()) { 
		__int64 idxString = iter - params._strings.begin(); 
		int idxImage = -1; 
		if(params._piml) { 
			if(params._piml->_indices.size() > (size_t)idxString) { 
				idxImage = params._piml->_indices[(size_t)idxString]; 
			} 
		} 

		COMBOBOXEXITEMA cbI; 
		cbI.mask = CBEIF_TEXT | CBEIF_LPARAM; 
		if(idxImage > -1) { 
			cbI.mask |= CBEIF_IMAGE | CBEIF_SELECTEDIMAGE; 
		} 
		cbI.pszText = (LPSTR)(*iter).c_str();
		cbI.cchTextMax = (int)(*iter).size();
		cbI.lParam = (LPARAM)(*iter).c_str(); // A value specific to the item
		cbI.iItem = -1;
		cbI.iImage = idxImage;	// Image to display
		cbI.iSelectedImage = idxImage;	// Image to display

		// Add the item to the combo box drop-down list
		SendMessageA(params._hwndcombo, CBEM_INSERTITEM, 0L,(LPARAM)&cbI); 
		++iter; 
	} 

	int minheight = 0; 
	RECT rc; 
	if(GetClientRect(params._hwndcombo, &rc)) { 
		minheight = rc.bottom - rc.top; 
	} 

	// Select a default item in the combo box 
	SendMessage(params._hwndcombo, CB_SETCURSEL, (WPARAM)0, 0L); 

	if(ppcombo) { 
		(*ppcombo) = std::auto_ptr<SComboBox>(new SComboBox()); 
		(*ppcombo)->_hwndcombo = params._hwndcombo; 
		(*ppcombo)->_minwidth = minwidth; 
		(*ppcombo)->_minheight = minheight; 
		SetWindowLongPtr(params._hwndcombo, GWLP_USERDATA, (LONG_PTR)(*ppcombo).get()); 

		// Set the window procedure for the combo box. 
	    lpfnDefCombo = (WNDPROC)GetWindowLongPtr(params._hwndcombo, GWLP_WNDPROC); 
		SetWindowLongPtr(params._hwndcombo, GWLP_WNDPROC, (LONG_PTR)KWinComboWndProc); 
	} 

	return TRUE; 
} 



BOOL EXP_KWIN KWinCreateToolBar(SCreateToolBar& params, SToolBar& toolbar) { 
	if(!params._siml.size()) { 
		return FALSE; 
	} 

	toolbar._buttons.resize(0); 
	toolbar._buttons.insert(toolbar._buttons.begin(), params._buttons.begin(), params._buttons.end()); 

	toolbar._idtoolbar = params._idtoolbar; 
	toolbar._idfirstbutton = params._idfirstbutton; 


	SCreateImageList cr_iml; 
	cr_iml._images.insert(cr_iml._images.begin(), params._siml.begin(), params._siml.end()); 

	toolbar._piml = std::auto_ptr<SImageList>(new SImageList()); 
	if(!KWinCreateImageList(cr_iml, *(toolbar._piml.get()))) { 
		toolbar._piml.release(); 
		return FALSE; 
	} 

	if(params._simlhot.size()) { 
		cr_iml._images.resize(0); 
		cr_iml._images.insert(cr_iml._images.begin(), params._simlhot.begin(), params._simlhot.end()); 
		toolbar._pimlhot = std::auto_ptr<SImageList>(new SImageList()); 
		if(!KWinCreateImageList(cr_iml, *(toolbar._pimlhot.get()))) { 
			toolbar._pimlhot.release(); 
		} 
	} 

	if(params._simldisabled.size()) { 
		cr_iml._images.resize(0); 
		cr_iml._images.insert(cr_iml._images.begin(), params._simldisabled.begin(), params._simldisabled.end()); 
		toolbar._pimldisabled = std::auto_ptr<SImageList>(new SImageList()); 
		if(!KWinCreateImageList(cr_iml, *(toolbar._pimldisabled.get()))) { 
			toolbar._pimldisabled.release(); 
		} 
	} 

	size_t isize = toolbar._piml->_imgheight; 
	size_t numbitmaps = toolbar._piml->_indices.size(); 

	while(numbitmaps > toolbar._buttons.size()) { 
		SToolBarButton bt; 
//		bt._state = 0; 
		toolbar._buttons.push_back(bt); 
	} 

	std::vector<TBBUTTON> tbbuttons; 
	tbbuttons.reserve(toolbar._buttons.size()); 

	std::vector<SToolBarButton>::iterator iter = toolbar._buttons.begin(); 
	while(iter != toolbar._buttons.end()) { 
		TBBUTTON button; 
		memset(&button, 0, sizeof(button)); 
		button.iBitmap = (int)(iter - toolbar._buttons.begin()); 
		button.idCommand = toolbar._idfirstbutton + button.iBitmap; 
		button.fsState = (*iter)._state; 
		button.fsStyle = BTNS_AUTOSIZE; 
		button.dwData = (DWORD)(*iter)._tiptext.data(); 

		tbbuttons.push_back(button); 
		++iter; 
	} 

	toolbar._hwndtoolbar = CreateWindowEx(
		0, 
		TOOLBARCLASSNAME, 
		NULL, 
		WS_CHILD | 
		WS_VISIBLE | 
		WS_CLIPCHILDREN | 
		WS_CLIPSIBLINGS | 
		TBSTYLE_LIST | 
		TBSTYLE_FLAT | 
		TBSTYLE_TOOLTIPS | 
//		TBSTYLE_TRANSPARENT | 
		CCS_NODIVIDER | 
		CCS_NORESIZE | 
		CCS_ADJUSTABLE, 
		0, 
		0, 
		(int)isize * (int)numbitmaps/*CW_USEDEFAULT*/, 
		(int)isize, 
		params._hwndparent, 
        (HMENU)params._idtoolbar, 
		(HINSTANCE)GetModuleHandle(0), 
		NULL); // no class data 

	if(toolbar._hwndtoolbar == NULL ) { 
		return FALSE; 
	} 

	toolbar._nbuttons = (int)numbitmaps; 

	SendMessage(toolbar._hwndtoolbar, TB_SETIMAGELIST, 0, (LPARAM)toolbar._piml->_himl); 
	if(toolbar._pimlhot.get()) { 
		SendMessage(toolbar._hwndtoolbar, TB_SETHOTIMAGELIST, 0, (LPARAM)toolbar._pimlhot->_himl); 
	} 
	if(toolbar._pimldisabled.get()) { 
		SendMessage(toolbar._hwndtoolbar, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)toolbar._pimldisabled->_himl); 
	} 

	SendMessage(toolbar._hwndtoolbar, TB_AUTOSIZE, 0, 0); 
	SendMessage(toolbar._hwndtoolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0); 
	SendMessage(toolbar._hwndtoolbar, TB_ADDBUTTONS, (WPARAM)numbitmaps, (LPARAM)(LPTBBUTTON)&tbbuttons[0]); 
	SendMessage(toolbar._hwndtoolbar, TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_DRAWDDARROWS); 

	RECT dimrect; 
	if(SendMessage(toolbar._hwndtoolbar, TB_GETRECT, toolbar._idfirstbutton, (LPARAM)&dimrect)) { 
		toolbar._buttonwidth = dimrect.right - dimrect.left; 
		toolbar._buttonheight = dimrect.bottom - dimrect.top; 
	} 

	return TRUE;
}



