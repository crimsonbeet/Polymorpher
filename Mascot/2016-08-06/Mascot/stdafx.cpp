#pragma comment( user, "Created by Alex Nickol Bourakov on 2013/09/06")

#pragma once
#include "stdafx.h"


#define OLE_DATE_IMPL 

#define WSICLASSFACTORY_IMPL

#include "./classesToplevel.h"


const char *IMG_ARROW_LEFT = "Bitmaps\\arrow_left_24.bmp"; 
const char *IMG_ARROW_LEFT_D = "Bitmaps\\arrow_left_24_d.bmp"; 
const char *IMG_ARROW_LEFT_H = "Bitmaps\\arrow_left_24_h.bmp"; 
const char *IMG_ARROW_RIGHT = "Bitmaps\\arrow_right_24.bmp"; 
const char *IMG_ARROW_RIGHT_D = "Bitmaps\\arrow_right_24_d.bmp"; 
const char *IMG_ARROW_RIGHT_H = "Bitmaps\\arrow_right_24_h.bmp"; 

const char *IMG_NEWDOCUMENT = "Bitmaps\\new_document_lined_24.bmp"; 
const char *IMG_NEWDOCUMENT_D = "Bitmaps\\new_document_lined_24_d.bmp"; 
const char *IMG_NEWDOCUMENT_H = "Bitmaps\\new_document_lined_24_h.bmp"; 
const char *IMG_DELETEDOCUMENT = "Bitmaps\\delete_24.bmp"; 
const char *IMG_DELETEDOCUMENT_D = "Bitmaps\\delete_24_d.bmp"; 
const char *IMG_DELETEDOCUMENT_H = "Bitmaps\\delete_24_h.bmp"; 
const char *IMG_SAVEDOCUMENT = "Bitmaps\\save_24.bmp"; 
const char *IMG_SAVEDOCUMENT_D = "Bitmaps\\save_24_d.bmp"; 
const char *IMG_SAVEDOCUMENT_H = "Bitmaps\\save_24_h.bmp"; 
const char *IMG_STOPDOCUMENT = "Bitmaps\\stop_24.bmp"; 
const char *IMG_STOPDOCUMENT_D = "Bitmaps\\stop_24_d.bmp"; 
const char *IMG_STOPDOCUMENT_H = "Bitmaps\\stop_24_h.bmp"; 
const char *IMG_FINISHDOCUMENT = "Bitmaps\\finish_document_24.bmp"; 
const char *IMG_FINISHDOCUMENT_D = "Bitmaps\\finish_document_24_d.bmp"; 
const char *IMG_FINISHDOCUMENT_H = "Bitmaps\\finish_document_24_h.bmp"; 



int MyCreateDirectory(const std::string& dir_name) { 
	int rc = 0; 
	HANDLE hdir = CreateFile(dir_name.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ|FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL); 
	if(hdir == INVALID_HANDLE_VALUE) { 
		rc = 1; 
		if(!CreateDirectory(dir_name.c_str(), NULL)) { 
			std::cout << "cannot create directory" << ' ' << dir_name << std::endl; 
			rc = -1; 
		} 
	} 
	else { 
		CloseHandle(hdir); 
	} 
	return rc; 
} 
