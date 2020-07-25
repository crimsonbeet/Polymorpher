
#include "TESTCompression_stdafx.h"


HANDLE OSRDONLYOpenFile(const std::string& filename) {
	HANDLE handle = (HANDLE)-1;
	handle = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return handle;
}


HANDLE OSWRONLYOpenFile(const std::string& filename) {
	HANDLE handle = (HANDLE)-1;
	handle = CreateFileA(filename.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL/*|FILE_FLAG_WRITE_THROUGH*/, NULL);
	return handle;
}

int OSSEEKFileSize(HANDLE handle) {
	int len = 0;
	DWORD lDistLow = GetFileSize(handle, 0);
	len = (int)lDistLow;
	return len;
}

int OSReadStringFromFile(HANDLE handle, std::string& inp_str) {
	int len = 0;
	DWORD dw;
	ReadFile(handle, (void*)inp_str.data(), (DWORD)inp_str.size(), &dw, 0);
	len = (int)dw;
	if(len != (int)inp_str.size()) {
		if(len > 0 && len < (int)inp_str.size()) {
			inp_str.resize((int)len);
		}
		else {
			inp_str.resize(0);
			len = 0;
		}
	}

	return len;
}

int OSCloseFile(HANDLE handle) {
	int rc = -1;
	rc = CloseHandle(handle);
	return rc;
}

int OSWriteStringToFile(HANDLE handle, const std::string& out_str) {
	int rc = -1;
	int len = 0;
	DWORD dw;
	WriteFile(handle, out_str.c_str(), (DWORD)out_str.size(), &dw, NULL);
	SetEndOfFile(handle);
	len = (int)dw;
	if(len == (int)out_str.size()) {
		rc = len;
	}

	return rc;
}

int ReadTextFile(const std::string& filename, std::string& inp_str) {
	int rc = 0;
	HANDLE handle = OSRDONLYOpenFile(filename);
	if(handle != (HANDLE)-1) {
		int len = OSSEEKFileSize(handle);
		if(len > 0) {
			inp_str.reserve(len + 8);
			inp_str.resize(len);
			rc = OSReadStringFromFile(handle, inp_str);
		}
		OSCloseFile(handle);
	}

	return rc;
}

int WriteTextFile(const std::string& filename, const std::string& out_str) {
	int rc = 0;
	HANDLE handle = OSWRONLYOpenFile(filename);
	if(handle != (HANDLE)-1) {
		rc = OSWriteStringToFile(handle, out_str);
		OSCloseFile(handle);
	}

	return rc;
}

