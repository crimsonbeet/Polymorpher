// stdafx.cpp : source file that includes just the standard includes
// IPCGate.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

// Created by Alex Nikol Bourakov (ANB) around April/07

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "stdafx_gate.h"

#include "IPCGate.h"

const char *_g_string_can_not_open_file = "Can not open file";



BOOL Not_Isspace(char c) {
	return c != 0x20;
}

BOOL EXP XTrimString(std::string& s) {
	if(s.size()) {
		size_t x;
		for(x = s.size() - 1; x && s[x] <= 0x20; --x);
		if(++x < s.size()) {
			s.resize(x);
		}
//		std::string::reverse_iterator it = std::find_if(s.rbegin(), s.rend(), Not_Isspace);
//		if(it != s.rbegin()) {
//			s.erase(it.base(), s.end());
//			return TRUE;
//		}
	}
	return FALSE;
}

VOID EXP String_2Lower(std::string& s) {
	for(size_t x = 0; x < s.size(); ++x) {
		if(isupper(s[x])) {
			s[x] = tolower(s[x]);
		}
	}
}

VOID EXP String_2Upper(std::string& s) {
	for(size_t x = 0; x < s.size(); ++x) {
		if(islower(s[x])) {
			s[x] = toupper(s[x]);
		}
	}
}

std::string EXP XTrimString(char *str, int nsize) {
	std::string robj;
	if(str) {
		if(nsize < 0) {
			nsize = (int)strlen(str);
		}
		size_t x;
		for(x = nsize - 1; x && str[x] <= 0x20; --x);
		return std::string(str, x + 1);
	}
	return std::string();
}

std::string EXP ReadTextFile(const std::string& file_name) {
	std::string str;
	ReadTextFile(file_name, str);

	return str;
}

std::string EXP GetFileNameExtension(const std::string& name) {
	std::string ext;
	for(int x = (int)name.size() - 1; x > 0; --x) {
		if(name[x] == '.') {
			++x;
			ext = name.substr(x, name.size() - x);
			String_2Lower(ext);
			break;
		}
	}
	return ext;
}


int EXP ReadTextFile(const std::string& filename, std::string& inp_str) {
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
	else {
		std::cout << _g_string_can_not_open_file << ' ' << filename << ' ' << "for read" << std::endl;
	}

	return rc;
}

int EXP WriteTextFile(const std::string& filename, const std::string& out_str) {
	int rc = 0;
	HANDLE handle = OSWRONLYOpenFile(filename);
	if(handle != (HANDLE)-1) {
		rc = OSWriteStringToFile(handle, out_str);
		OSCloseFile(handle);
	}
	else {
		std::cout << _g_string_can_not_open_file << ' ' << filename << ' ' << "for write" << std::endl;
	}

	return rc;
}

int EXP AppendTextFile(const std::string& filename, const std::string& out_str) {
	int rc = 0;
	HANDLE handle = OSWRONLYOpenFile(filename, 1);
	if(handle != (HANDLE)-1) {
		rc = OSAppendStringToFile(handle, out_str);
		OSCloseFile(handle);
	}
	else {
		std::cout << _g_string_can_not_open_file << ' ' << filename << ' ' << "for append" << std::endl;
	}

	return rc;
}


