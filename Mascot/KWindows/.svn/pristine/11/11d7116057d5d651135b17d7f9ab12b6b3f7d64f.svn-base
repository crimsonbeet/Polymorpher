#pragma once
// Adopted from sources of Yuan, Feng 
// Created by Alex Bourakov. 

#include <commctrl.h>

#include ".\win.h"
#include ".\Status.h"
#include ".\Canvas.h"
#include ".\LogWindow.h"
#include ".\Hyperlist.h"
#include ".\Controls.h"

EXPORT_STL_VECTOR(KCanvas)
EXPORT_STL_VECTOR(KStatusWindow)
EXPORT_STL_VECTOR(KLogWindow)
EXPORT_STL_VECTOR(KHyperlist)

// SDI frame window, can have a toolbar, canvas window and a status window
class EXP_KWIN KFrame : public KWindow { 
protected:
	KCanvas *_pCanvas; 
	KStatusWindow  *_pStatuswin; 
	KLogWindow *_pLogwin; 
	KHyperlist *_pHyperlist; 

	int _width; 
	int _height; 

	void CreateKWindow(KWindow& item); 
	void ResizeKWindow(KWindow& kwin); 

	void AlignChildRectangles(); 

	IXFunction<SOnMessageParams, void> *_messagehandler; 
	void OnMessage(SOnMessageParams& params); 

	BOOL OnCopyData(LPARAM lparam); 
	BOOL OnUserMessage(WPARAM wparam, LPARAM lparam); 
	BOOL OnSize(int width, int height); 

	std::vector<KStatusWindow>  _vecstatuswin; 

	bool CreateMDIChild(KCanvas *pCanvas); 

public: 
	void Create(HINSTANCE hInst, HWND hParent); 

	template<class MDIChildNewOperator> bool NewMDIChild(MDIChildNewOperator& NewOperator) { 
//		NewOperator can be a function with no parameters, or a class with operator (). 
//		In any case the return value must be a pointer to KCanvas object. 
//		Function/class must crate KCanvas object using new operator, because 
//		The object will be deleted later by the framework using delete operator. 
//		KCanvas object should not be initialized 
//		with the exception of member _data_type. 
//		_data_type should match datatype name of one of the MDICLIENT canvas. 
		return CreateMDIChild(NewOperator()); 
	} 

	void Configure(const std::string& config); 

	std::vector<KCanvas> _veccanvas; 
	std::vector<KHyperlist> _vechyperlist; 
	std::vector<KLogWindow> _veclogwin; 

	KHyperlist * FindHListObject(const std::string& data_type); 
	KCanvas * FindCanvasObject(const std::string& data_type); 

	BOOL ShowWindow(int nCmdShow = SW_SHOW); 
	void SetTopMost(HWND hWndInsertAfter = HWND_NOTOPMOST); 

	void SetTopMostTreeObject(KHyperlist& obj); 
	void SetTopMostCanvasObject(KCanvas& obj); 

	BOOL _toolbar_delegates; 

	SToolBar *_toolbar; 
	BOOL ActivateToolBar(SReBar& rebar, const int toolbar_id, const std::string img[], const std::string img_h[], const std::string img_d[], const int img_cnt); 
	virtual void ProcessToolBarCommand(SReBar& rebar); 
	virtual void ProcessToolBarNotify(SReBar& rebar); 
	virtual void OnChangeFrameState(SReBar& rebar, int state/* 0 - inactive, 1 - active, -1 - out of scope */); 


	std::string _data_type; 

	HINSTANCE _hinst;
	HACCEL _haccel;	

	KFrame(HINSTANCE hInstance, HICON hIcon = NULL); 
	
	virtual ~KFrame();
	
	virtual WPARAM MessageLoop(void);
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(KFrame)

CONTAINS_FLAT_MEMBER(_data_type, DataType)
CONTAINS_OBJECT_MEMBER(_veccanvas, Canvas)
CONTAINS_OBJECT_MEMBER(_vechyperlist, HyperList)
CONTAINS_OBJECT_MEMBER(_veclogwin, LogWindow)
//CONTAINS_INHERITED_OBJECT_MEMBER(KWindow, _ctl, Situation)
CONTAINS_OBJECT_MEMBER(_ctl, Situation)
END_WSI_SERIALIZATION_OBJECT()



class EXP_KWIN KFrameHistory { 
protected:
	std::auto_ptr<SReBar> _prebar; 
	SToolBar *_toolbar; 

	std::vector<KFrame*> _frameshistory; 
	int _frameshistory_pos; 

	void SynchronizeToolBar(); 

public: 
	KFrameHistory(std::auto_ptr<SReBar>& prebar, SToolBar *toolbar); 

	void InsertHistory(KFrame*); 

	BOOL StepBackHistory(); 
	BOOL StepForthHistory(); 

	BOOL ShowCurrent(); 

	const PSReBar GetRebarPtr(); 
	std::auto_ptr<SReBar>& TakeRebar(); 
}; 

