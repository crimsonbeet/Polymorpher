#pragma comment( user, "Created by Alex Nickol Bourakov on 2013/09/06")
#include "stdafx.h"

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
