
#pragma once


#include "OleDate.h" 

#include "IPCInterface.h"
#include "WSIParser.h" 

#include "FrameWnd.h" 

#include "KVectors.h"


// FF904E - Adobe 
// 4169E1 - Royal Blue
// B0C4DE - Light Steel Blue
// 4682B4 - Steel Blue 
// 6495ED - Corn Flower Blue
// 3CB371 - Midium Sea Blue
// 87CEFA - Light Sky Blue
// D3D3D3 - Light Gray

extern std::string _g_program_files; 
extern std::string _g_boot_drive; 


extern const char *IMG_ARROW_LEFT; 
extern const char *IMG_ARROW_LEFT_D; 
extern const char *IMG_ARROW_LEFT_H; 
extern const char *IMG_ARROW_RIGHT; 
extern const char *IMG_ARROW_RIGHT_D; 
extern const char *IMG_ARROW_RIGHT_H; 

extern const char *IMG_NEWDOCUMENT; 
extern const char *IMG_NEWDOCUMENT_D; 
extern const char *IMG_NEWDOCUMENT_H; 
extern const char *IMG_DELETEDOCUMENT; 
extern const char *IMG_DELETEDOCUMENT_D; 
extern const char *IMG_DELETEDOCUMENT_H; 
extern const char *IMG_SAVEDOCUMENT; 
extern const char *IMG_SAVEDOCUMENT_D; 
extern const char *IMG_SAVEDOCUMENT_H; 
extern const char *IMG_STOPDOCUMENT; 
extern const char *IMG_STOPDOCUMENT_D; 
extern const char *IMG_STOPDOCUMENT_H; 
extern const char *IMG_FINISHDOCUMENT; 
extern const char *IMG_FINISHDOCUMENT_D; 
extern const char *IMG_FINISHDOCUMENT_H; 



struct ISInvalidFSCharacter { 
	bool operator () (char val) { 
		switch(val) { 
			case '\\':
			case '/':
			case ':':
			case '*':
			case '?':
			case '"':
			case '<':
			case '>':
			case '|':
			return true; 
		} 
		return val < ' '; 
	} 
}; 



