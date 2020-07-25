#ifndef XFrameMainH
#define XFrameMainH

#include "FrameWnd.h" 

#include "FrameOther.h" 



class XFrameMain: public KFrame { 
public: 
	XFrameMain(HINSTANCE hInstance, HICON hIcon = NULL); 

	void Instantiate(); 

	SImageList _iml; 

	std::auto_ptr<SReBar> _prebar; 

	void ProcessToolBarCommand(SReBar& rebar); 

	IXFunction<SOnMessageParams, void> *_messagehandler; 
	void OnMessage(SOnMessageParams& params); 

	IXFunction<SOnDrawParams, void> *_painthandler; 
	void OnPaint(SOnDrawParams& params); 
}; 




extern XFrameMain *_g_main_frame; 
extern KFrameHistory *_g_frame_history; 



#endif //XFrameMainH

