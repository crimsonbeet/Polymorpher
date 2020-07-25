
#include "stdafx.h"
#include "./classesToplevel.h"

#include "FrameMain.h"



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

template<typename T> 
void ByteSwap(T* buffer, int iCount) { 
	for(int k = 0; k < iCount; ++k, ++buffer) { 
		PBYTE begin = (PBYTE)buffer; 
		PBYTE end = begin + sizeof(T) - 1; 
		while(end > begin) { 
			BYTE temp = *begin; 
			*begin++ = *end; 
			*end-- = temp; 
		} 
	} 
} 

void Daub_5_3_transform(short int *s0/*in*/, int s0_length, short int *s1/*out; must be at least s0_length/2 */, short int *d1/*out; must be at least s0_length/2 */);
void Daub_5_3_inverse(short int *s1/*in; must be at least s0_length/2 */, short int *d1/*in; must be at least s0_length/2 */, short int *s0/*out*/, int s0_length);
size_t streamify(const std::vector<short int>& vTransformed_values, std::vector<char>& vStreamed_values);
size_t destreamify(const std::vector<char>& vStreamed_values, std::vector<short int>& vTransformed_values);
void compress_enhanced_graphics(std::vector<short int>& short_data, std::string& vStreamed_values);
void decompress_enhanced_graphics(std::string& vStreamed_values, std::vector<short int>& short_data);


XFrameOther::XFrameOther(HINSTANCE hInstance, HWND hParent): KFrame(hInstance) { 
	Configure(ReadTextFile("TESTVectorsFrameOther.txt")); 
	Create(hInstance, hParent); 

	_delegates->_onmessage_delegates.Add(&XFrameOther::OnMessage, this); 

	std::string inpFileName = "decompressedk3.bin"/*"k4.bin"*/; 

	std::string bin_data; 
	ReadTextFile(inpFileName.c_str(), bin_data); 


	std::vector<short int> short_data; 
	short_data.resize(bin_data.size() / 2); 
	memcpy(short_data.data(), bin_data.data(), bin_data.size()); 

	ByteSwap(&short_data[0], (int)short_data.size()); 


	std::string streamed_values; 
	//compress_enhanced_graphics(short_data, streamed_values); 
	//WriteTextFile("compressed" + inpFileName, streamed_values);
	//decompress_enhanced_graphics(streamed_values, short_data); 


	KWSIPointArray *torques = new KWSIPointArray(); 
	KWSIPointArray *transformed_torques = new KWSIPointArray(); 
	KWSIPointArray *restored_torques = new KWSIPointArray(); 

	std::vector<short int> vRaw_torques; 
	std::vector<short int> vTransformed_torques; 
	std::vector<short int> vRestored_torques; 
	std::vector<short int> vDestreamified_torques; 

	size_t nSize = short_data.size() / 3; 
	size_t nRSize = (nSize & 1)? (nSize + 1): nSize; 

	torques->_points.resize(nRSize); 
	transformed_torques->_points.resize(nRSize); 
	restored_torques->_points.resize(nRSize); 

	vRaw_torques.resize(nRSize); 
	vTransformed_torques.resize(nRSize); 
	vRestored_torques.resize(nRSize); 
	

	for(size_t x = 0, z = 0; x < short_data.size(); x += 3, ++z) { 
		torques->_points[z] = KWSIPoint(z, short_data[x]); 
		vRaw_torques[z] = short_data[x]; 
	} 

	if(nSize < nRSize) { 
		torques->_points[nSize] = torques->_points[nSize - 1]; 
		vRaw_torques[nSize] = vRaw_torques[nSize - 1]; 
	} 


	double max_x = std::numeric_limits<double>::min(); 
	double min_x = std::numeric_limits<double>::max(); 
	double max_y = std::numeric_limits<double>::min(); 
	double min_y = std::numeric_limits<double>::max(); 
	for(size_t z = 0; z < torques->_points.size(); ++z) { 
		if(torques->_points[z]._x < min_x) { 
			min_x = torques->_points[z]._x; 
		} 
		if(torques->_points[z]._y < min_y) { 
			min_y = torques->_points[z]._y; 
		} 
		if(torques->_points[z]._x > max_x) { 
			max_x = torques->_points[z]._x; 
		} 
		if(torques->_points[z]._y > max_y) { 
			max_y = torques->_points[z]._y; 
		} 
	} 
	double dx = max_x-min_x; 
	double dy = max_y-min_y; 
	double ratio = 1; 
	if(dx >= 0 && dy >= 0) { 
		ratio = dx / (dy * 2); 
	} 

	//for(size_t z = 0; z < torques->_points.size(); ++z) { 
	//	torques->_points[z]._y -= min_y; 
	//	vRaw_torques[z] -= (short int)min_y; 
	//} 
	//min_y = 0; 
	//max_y -= min_y; 

	Daub_5_3_transform(&vRaw_torques[0], nRSize, &vTransformed_torques[0], &vTransformed_torques[nRSize >> 1]);
	for(size_t z = nRSize >> 1, mz = vTransformed_torques.size() - (nRSize > nSize? 4: 2); z < mz; ++z) { 
		if(vTransformed_torques[z]) { 
			if(std::abs(vTransformed_torques[z]) <= 1) { 
				vTransformed_torques[z] = 0; 
			} 
		} 
	} 

	std::vector<char> vStreamed_values; 
	streamify(vTransformed_torques, vStreamed_values); 

	destreamify(vStreamed_values, vDestreamified_torques); 
	Daub_5_3_inverse(&vTransformed_torques[0],  &vTransformed_torques[nRSize >> 1], &vRestored_torques[0], nRSize); 
	//Daub_5_3_transform(&vRestored_torques[0], nRSize, &vTransformed_torques[0], &vTransformed_torques[nRSize >> 1]);
	//Daub_5_3_inverse(&vTransformed_torques[0],  &vTransformed_torques[nRSize >> 1], &vRestored_torques[0], nRSize); 

	for(size_t z = 0; z < vTransformed_torques.size(); ++z) { 
		transformed_torques->_points[z] = KWSIPoint(z, vTransformed_torques[z]); 
		restored_torques->_points[z] = KWSIPoint(z, vRestored_torques[z]); 
	} 



	if(ratio != 1) {
		for(size_t z = 0; z < torques->_points.size(); ++z) { 
			torques->_points[z]._y *= ratio; 
			transformed_torques->_points[z]._y *= ratio; 
			restored_torques->_points[z]._y *= ratio; 
		} 
	} 


	KDrawing *pDrawing = new KDrawing("", ("torque" + inpFileName).c_str()); 
	//KDrawing *pDrawing = new KDrawing("", ("angle" + inpFileName).c_str()); 
	//KDrawing *pDrawing = new KDrawing("", ("time" + inpFileName).c_str()); 
	KDrawing *pDrawing2 = new KDrawing("", "Transformed"); 
	KDrawing *pDrawing3 = new KDrawing("", "Restored"); 

	pDrawing->_meta.AddPtr(torques); 
	pDrawing->_meta.AddPtr(restored_torques); 

	pDrawing2->_meta.AddPtr(transformed_torques); 
	pDrawing3->_meta.AddPtr(restored_torques); 

	LOGBRUSH green_brush = { BS_SOLID, RGB(0, 128, 0), 0 }; 
	restored_torques->brush = green_brush; 

	NewMDIChild(*pDrawing); 
	//NewMDIChild(*pDrawing2); 
	//NewMDIChild(*pDrawing3); 

	//NewMDIChild(NewKDrawing(ReadTextFile("KDrawing.txt"))); 

//	attach ondraw handler. 

	int nstep = 1; 
	while(nstep) switch(nstep) { 
		case 1: // make decision where. 
			nstep = _veccanvas.size()? 2: 3; 
			break; 
		case 2: // attach to canvas. 
			_veccanvas[0]._delegates->_ondraw_delegates.Add(&XFrameOther::OnPaint, this); 
			nstep = 0; 
		break; 
		case 3: // attach to itself. 
			_delegates->_ondraw_delegates.Add(&XFrameOther::OnPaint, this); 
			nstep = 0; 
		break; 
	} 
} 


void XFrameOther::ProcessToolBarCommand(SReBar& rebar) { 
	if(rebar._lparam == (LPARAM)_toolbar->_hwndtoolbar) { 
		switch(rebar._wparam) { 
			case 203/*new document*/: 
			break; 
			case 204/*delete document*/: 
			break; 
		} 
	} 
} 

void XFrameOther::ProcessToolBarNotify(SReBar& rebar) { 
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

void XFrameOther::OnChangeFrameState(SReBar& rebar, int state) { 
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


void XFrameOther::OnMessage(SOnMessageParams& params) { 
//	static bool is_sizing = false; 
	switch(params._umsg) { 
		case WM_CLOSE: 
		break; 
		case WM_DESTROY: 
		break; 
		case WM_SIZE: if(!IsSizing()) { 
		} 
		break; 
	} 
} 

void XFrameOther::OnPaint(SOnDrawParams& params) { 
} 



void KDrawing::Prepare() { 
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

	POINT dev_ext = { (int)(dx * ratio + 0.45), (int)(dy * ratio + 0.45) }; 

	_kdib.CreateDIBSection32(dev_ext.x, dev_ext.y); 

	_pen_width = std::min(dev_ext.x, dev_ext.y) * 0.0025; 

	if(ratio > 1) { 
		_ratio = ratio; 
	} 
	else { 
		_ratio = 1; 

		_pen_width /= ratio * 1.5; 
	} 

	POINT win_ext = { (int)(dx * _ratio + 0.45), (int)(dy * _ratio + 0.45) }; 
	POINT win_org = { (int)(wbox._x[0] * _ratio + 0.45), (int)(wbox._y[0] * _ratio + 0.45) }; 

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
	params._ratio = _ratio; 
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
} 

BOOL KDrawing::DrawLine(const std::vector<KWinPoint>& line) { 
	if(line.size() != 2) { 
		std::cout << '{' << "KDrawing::DrawLine bad param: line.size()==" << line.size() << std::endl; 
		return FALSE; 
	} 

	std::cout << '{' << line[0]._x << ',' << line[0]._y << '}' << '{' << line[1]._x << ',' << line[1]._y << '}' << std::endl; 

	POINT pt; 

	line[0].GDIPoint(pt, _ratio); 
	MoveToEx(_kdib._hmemdc, pt.x, pt.y, NULL); 
	line[1].GDIPoint(pt, _ratio); 
	LineTo(_kdib._hmemdc, pt.x, pt.y); 

	return TRUE; 
} 

BOOL KDrawing::DrawPoint(const KWinPoint& point) { 
	std::cout << '{' << point._x << ',' << point._y << '}' << std::endl; 

	POINT pt; 

	point.GDIPoint(pt, _ratio); 

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

