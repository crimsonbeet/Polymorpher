#ifndef XFrameOtherH
#define XFrameOtherH


#include "FrameWnd.h" 



class XFrameOther: public KFrame { 
public: 
	XFrameOther(HINSTANCE hInstance, HWND hParent); 

	void ProcessToolBarCommand(SReBar& rebar); 
	void ProcessToolBarNotify(SReBar& rebar); 

	void OnChangeFrameState(SReBar& rebar, int state); 


	IXFunction<SOnMessageParams, void> *_messagehandler; 
	void OnMessage(SOnMessageParams& params); 

	IXFunction<SOnDrawParams, void> *_painthandler; 
	void OnPaint(SOnDrawParams& params); 
}; 



class KDrawing: public KCanvas { 
	KDIBSection _kdib; 
	KWinRect _kbox; 

	double _ratio; 
	double _inv_ratio; 

	double _pen_width; 

	HPEN _line_pen; 

public: 
	KWSIMetaFile _meta; 

	KDrawing(const std::string& config, const std::string& name = "TO_BE_DETERMINED"): _line_pen(0), _pen_width(1) { 
		_meta.Configure(config); 

		_data_type = _meta._name; 

		if(!_data_type.size()) { 
			_data_type = "KDrawing"; 
		} 

		_ratio = 0; 
		_inv_ratio = 0; 

		_ctl.name = name; 
	} 

	KDrawing * operator ()() { 
		return this; 
	} 

	void Prepare(); 
	void Resize(HWND hParent, int width, int height); 

	void OnPaint(SOnDrawParams& params); 

	BOOL DrawLine(const std::vector<KWinPoint>& line); 
	BOOL DrawPoint(const KWinPoint& point); 
}; 


class NewKDrawing { 
	const std::string& _config; 

public: 
	NewKDrawing(const std::string& config): _config(config) { 
	} 

	KDrawing * operator ()() { 
		return new KDrawing(_config); 
	} 
}; 




extern XFrameOther *_g_other_frame; 



#endif //XFrameOtherH

