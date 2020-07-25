
#include "stdafx.h"
#include "./classesToplevel.h"

#include "FrameMain.h"


XFrameDrawing *_g_drawing_frame; 
KDrawing *_g_pDrawing;


std::string _g_sDate_time; 



const std::string _g_document_images[2] = { 
	IMG_NEWDOCUMENT, 
	IMG_DELETEDOCUMENT 
}; 

const std::string _g_document_images_disabled[2] = { 
	IMG_NEWDOCUMENT_D, 
	IMG_DELETEDOCUMENT_D 
}; 

const std::string _g_document_images_hot[2] = { 
	IMG_NEWDOCUMENT_H, 
	IMG_DELETEDOCUMENT_H 
}; 

XFrameDrawing::XFrameDrawing(HINSTANCE hInstance, HWND hParent): KFrame(hInstance) { 
	Configure(ReadTextFile("FrameDrawing.txt")); 
	Create(hInstance, hParent); 

	_delegates->_onmessage_delegates.Add(&XFrameDrawing::OnMessage, this); 

	_g_sDate_time = GetDBDateStringFromOleDate(GetOleCurrentDate(), "%02d-%02d-%02d.%02d.%02d.%02d"); 

	MyCreateDirectory(_g_sDate_time); 
	NewMDIChild(*(_g_pDrawing = new KDrawing("", _g_sDate_time))); 

//	attach ondraw handler. 

	int nstep = 1; 
	while(nstep) switch(nstep) { 
		case 1: // make decision where. 
			nstep = _veccanvas.size()? 2: 3; 
			break; 
		case 2: // attach to canvas. 
			_veccanvas[0]._delegates->_ondraw_delegates.Add(&XFrameDrawing::OnPaint, this); 
			nstep = 0; 
		break; 
		case 3: // attach to itself. 
			_delegates->_ondraw_delegates.Add(&XFrameDrawing::OnPaint, this); 
			nstep = 0; 
		break; 
	} 
} 


void XFrameDrawing::ProcessToolBarCommand(SReBar& rebar) { 
	if(rebar._lparam == (LPARAM)_toolbar->_hwndtoolbar) { 
		switch(rebar._wparam) { 
			case 203/*new document*/: 
			break; 
			case 204/*delete document*/: 
			break; 
		} 
	} 
} 

void XFrameDrawing::ProcessToolBarNotify(SReBar& rebar) { 
	NMHDR *pnmhdr = (NMHDR*)(rebar._lparam); 
	if(pnmhdr) { 
		if(pnmhdr->hwndFrom == _toolbar->_hwndtoolbar) { 
			switch(pnmhdr->code) { 
				case TBN_DROPDOWN: { 
					LPNMTOOLBAR lpnmtb = (LPNMTOOLBAR)pnmhdr; 
					if(lpnmtb->iItem == 203/*new document*/) { 
						HMENU hmenu = CreatePopupMenu(); 
						AppendMenu(hmenu, MF_STRING |MF_ENABLED, 223, "New Document 1"); 
						AppendMenu(hmenu, MF_STRING |MF_ENABLED, 224, "New Document 2"); 
//						SetMenuDefaultItem(hmenu, 223, FALSE); 

						UINT fuflags = TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD; 

						POINT point; 
						point.x = lpnmtb->rcButton.left; 
						point.y = lpnmtb->rcButton.bottom; 
						ClientToScreen(_toolbar->_hwndtoolbar, &point); 

						int rc = TrackPopupMenuEx(hmenu, fuflags, point.x, point.y, _hwnd, 0); 
						DestroyMenu(hmenu); 

						rebar._lresult = TBDDRET_DEFAULT; 

						switch(rc) { 
							case 223: 
							break; 

							case 224: 
							break; 

						} 
					} 
				} 
				break; 

				case TBN_GETINFOTIPA: { 
					LPNMTBGETINFOTIPA tipinfo = (LPNMTBGETINFOTIPA)rebar._lparam; 
					switch(tipinfo->iItem) { 
						case 203: 
							strncpy(tipinfo->pszText, " Create document ", tipinfo->cchTextMax); 
						break; 
						case 204: 
							strncpy(tipinfo->pszText, " Delete document ", tipinfo->cchTextMax); 
						break; 
					} 
				}
				break; 
			} 
		} 
	} 
} 

void XFrameDrawing::OnChangeFrameState(SReBar& rebar, int state) { 
	switch(state) { 
		case 1: /*active*/
			if(!_toolbar) { 
//				rebar._fbandstyle &= ~RBBS_NOGRIPPER; 
				ActivateToolBar(rebar, 203, _g_document_images, _g_document_images_hot, _g_document_images_disabled, ARRAY_NUM_ELEMENTS(_g_document_images)); 

				_toolbar->SetButtonStyleByindex(/*BTNS_DROPDOWN*/BTNS_WHOLEDROPDOWN, 0/*btn - new document*/); 
				_toolbar->SetButtonStateByindex(TBSTATE_ENABLED, 1/*btn - delete document*/, true/*remove enabled*/); 

			} 
			else 
			if(_toolbar->_rebarbandid != -1) { 
				SendMessage(rebar._hwndrebar, RB_SHOWBAND, _toolbar->_rebarbandid, 1); 
			} 
			break; 
		default: 
			KFrame::OnChangeFrameState(rebar, state); 
			break; 
	} 
} 


void XFrameDrawing::OnMessage(SOnMessageParams& params) { 
//	static bool is_sizing = false; 
	switch(params._umsg) { 
		case WM_USER: 
			switch(params._wparam) { 
				case WM_NCDESTROY: // destroy MDI child. 
					NewMDIChild(*(_g_pDrawing = new KDrawing("", GetDBDateStringFromOleDate(GetOleCurrentDate(), "%02d-%02d-%02d.%02d.%02d.%02d")))); 
					break; 
				case 0: 
					_g_main_frame->Invalidate(); 
					Invalidate(); 
					for(size_t j = 0; j < _veccanvas.size(); ++j) { 
						_veccanvas[j].Invalidate(); 
					} 
					break; 
				case 1: 
					_g_pDrawing->SetTopMost(); // it will also enable it. 
					break; 
			} 
		break; 
		case WM_CLOSE: 
		break; 
		case WM_DESTROY: 
		break; 
		case WM_SIZE: if(!IsSizing()) { 
		} 
		break; 
	} 
} 

void XFrameDrawing::OnPaint(SOnDrawParams& params) { 
} 



KDrawing::~KDrawing() { 
	_g_pDrawing = 0; 
	SaveDIBToFile((_g_sDate_time + '\\' + _ctl.name + ".bmp").c_str(), _kdib._pbmInfo, _kdib._pbmBits); 
} 


void KDrawing::Prepare() { 
	_meta.lock(); 

	_meta.Build(); 

	_meta.Perform(); 

	KWinRect& wbox = _kbox; 
	wbox = _meta._box.GetRect(); 

	double dx = wbox._x[1] - wbox._x[0]; 
	double dy = wbox._y[1] - wbox._y[0]; 

	RECT cli_rect; 
	GetClientRect(_hwnd, &cli_rect); 

	double x_ratio = (double)cli_rect.right / dx; 
	double y_ratio = (double)cli_rect.bottom / dy; 

	double ratio = std::min(x_ratio, y_ratio); 

	POINT dev_ext = { cli_rect.right, cli_rect.bottom }; 

	_kdib.CreateDIBSection32(dev_ext.x, dev_ext.y); 

	_ratio = std::max(ratio, 1.0); 

	_pen_width = std::min(dev_ext.x, dev_ext.y) * 0.001; 
	if(ratio <= 1) { 
		_pen_width /= ratio * 1.5; 
	} 

	POINT win_ext = { (int)((dx + 20/ratio) * _ratio + 0.45), (int)((dy + 20/ratio) * _ratio * _meta._y_ratio + 0.45) }; 
	POINT win_org = { (int)((wbox._x[0] - 10/ratio) * _ratio + 0.45), (int)((wbox._y[0] - 10/ratio) * _ratio * _meta._y_ratio + 0.45) }; 

	POINT view_ext = { dev_ext.x, -dev_ext.y }; 
	POINT view_org = { 0, dev_ext.y }; 

	SetMapMode(_kdib._hmemdc, MM_ANISOTROPIC); 

	SetViewportExtEx(_kdib._hmemdc, view_ext.x, view_ext.y, NULL); 
	SetWindowExtEx(_kdib._hmemdc, win_ext.x, win_ext.y, NULL); 

	SetViewportOrgEx(_kdib._hmemdc, view_org.x, view_org.y, NULL); 
	SetWindowOrgEx(_kdib._hmemdc, win_org.x, win_org.y, NULL); 

	SetGraphicsMode(_kdib._hmemdc, GM_ADVANCED); 
/*	XFORM xm = { 1, 0, 0, -1, 0, win_ext.y }; 
	SetWorldTransform(_kdib._hmemdc, &xm); 
*/
	if(_line_pen) { 
		DeleteObject(_line_pen); 
	} 
	LOGBRUSH brush = { BS_SOLID, RGB(128, 0, 0), 0 }; 
	_line_pen = ExtCreatePen(PS_GEOMETRIC | PS_SOLID, _pen_width > 1? (int)(_pen_width + 0.5): 1, &brush, 0, NULL); 

	std::cout << "pen_width=" << _pen_width << std::endl; 


	KWinDrawParams params; 
	params._hdc = _kdib._hmemdc; 
	params._box = wbox; 
	params._x_ratio = _ratio; 
	params._y_ratio = _ratio * _meta._y_ratio; 
	params._pen_width = _pen_width; 

	HPEN old_pen = (HPEN)SelectObject(_kdib._hmemdc, _line_pen); 
	_meta.Draw(params); 
	SelectObject(_kdib._hmemdc, old_pen); 


/*	std::vector<KWinPoint> line1 = _meta._krect_expl.GetRect().Intersect(_meta._kline_expl.GetExplicit()); 
	std::vector<KWinPoint> line2 = _meta._krect_expl.GetRect().Intersect(_meta._kline_impl.GetExplicit()); 

	KWinPoint point1; 
	_meta._kline_impl.GetExplicit().Intersect(_meta._kline_expl.GetImplicit(), point1); 


	if(line1.size() > 1) { 
		HPEN old_pen = (HPEN)SelectObject(_kdib._hmemdc, _line_pen); 

		DrawLine(line1); 
		DrawLine(line2); 

		DrawPoint(point1); 

		SelectObject(_kdib._hmemdc, old_pen); 
	} */ 

	_meta.unlock(); 
} 

BOOL KDrawing::DrawLine(const std::vector<KWinPoint>& line) { 
	if(line.size() != 2) { 
		std::cout << '{' << "KDrawing::DrawLine bad param: line.size()==" << line.size() << std::endl; 
		return FALSE; 
	} 

	std::cout << '{' << line[0]._x << ',' << line[0]._y << '}' << '{' << line[1]._x << ',' << line[1]._y << '}' << std::endl; 

	POINT pt; 

	line[0].GDIPoint(pt, _ratio, _ratio * _meta._y_ratio); 
	MoveToEx(_kdib._hmemdc, pt.x, pt.y, NULL); 
	line[1].GDIPoint(pt, _ratio, _ratio * _meta._y_ratio); 
	LineTo(_kdib._hmemdc, pt.x, pt.y); 

	return TRUE; 
} 

BOOL KDrawing::DrawPoint(const KWinPoint& point) { 
	std::cout << '{' << point._x << ',' << point._y << '}' << std::endl; 

	POINT pt; 

	point.GDIPoint(pt, _ratio, _ratio * _meta._y_ratio); 

	HPEN old_pen = (HPEN)SelectObject(_kdib._hmemdc, GetStockObject(DC_PEN)); 
	SetDCPenColor(_kdib._hmemdc, RGB(128, 128, 128)); 

	MoveToEx(_kdib._hmemdc, pt.x, pt.y, NULL); 
	AngleArc(_kdib._hmemdc, pt.x, pt.y, (DWORD)(_pen_width * 4 + 0.5), 0, 360); 

/*	int pen_width = (int)(_pen_width * 4 + 0.5); 

	POINT box_pt[2]; 
	box_pt[0].x = pt.x - pen_width; 
	box_pt[1].x = pt.x + pen_width; 
	box_pt[0].y = pt.y - pen_width; 
	box_pt[1].y = pt.y + pen_width; 

	MoveToEx(_kdib._hmemdc, pt.x, pt.y, NULL); 
	ArcTo(_kdib._hmemdc, box_pt[0].x, box_pt[0].y, box_pt[1].x, box_pt[1].y, box_pt[1].x, pt.y, box_pt[1].x, pt.y); */

	SelectObject(_kdib._hmemdc, old_pen); 

	return TRUE; 

} 


void KDrawing::Resize(HWND hParent, int width, int height) { 
//	Invalidate entire drawing. 
	_ratio = 0; 

	KCanvas::Resize(hParent, width, height); 
} 

void KDrawing::OnPaint(SOnDrawParams& params) { 
	if(_ratio == 0) { 
		Prepare(); 
	} 

	RECT rect; 
	GetClientRect(params._pwin->_hwnd, &rect); 

	_kdib.Draw(params._hdc, (rect.right - _kdib._width)/2, (rect.bottom - _kdib._height)/2, _kdib._width, _kdib._height, 0, 0, _kdib._width, _kdib._height); 

//	std::string file_name; 
//	if(_ctl.name.size()) { 
//		file_name = _ctl.name + ".bmp"; 
//	} 
//	else { 
//		file_name = "KDrawing.bmp"; 
//	} 
//
//	SaveWindowToDIB(file_name.c_str(), params._pwin->_hwnd, 0, 0); 
} 

