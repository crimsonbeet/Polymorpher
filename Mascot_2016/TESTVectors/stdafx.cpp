// stdafx.cpp : source file that includes just the standard includes
// stdafx.obj will contain the pre-compiled type information

#pragma comment( user, "Created by Alex Nickol Bourakov on 2009/02/12")
#include "stdafx.h"


#define OLE_DATE_IMPL 

#define WSICLASSFACTORY_IMPL

#include "./classesToplevel.h"


std::string itostdstring(int j) { 
	std::ostringstream ostr; 
	ostr << j; 
	return ostr.str(); 
} 

std::string trim2stdstring(const std::string& str) { 
	size_t nsize = str.find_last_not_of(' '); 
	if(nsize < str.size()) { 
		return std::string(str, 0, nsize + 1); 
	} 
	return std::string(); 
//	std::string::reverse_iterator last = std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun(isspace))); 
//	int nsize = last.base() - str.begin(); 
//	str.resize(nsize); 
} 


std::string filenameFromCommandline() { 
	std::string file_name; 
	int nArgs;
	LPWSTR *szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs); 
	if(szArglist != 0) { 
		file_name.resize((int)wcslen(szArglist[nArgs - 1])); 
		WideCharToMultiByte(CP_ACP, NULL, 
				szArglist[nArgs - 1], 
					(int)wcslen(szArglist[nArgs - 1]), 
						(PCHAR)file_name.c_str(), file_name.size(), NULL, NULL); 
		LocalFree(szArglist); 
	} 

	return file_name; 
} 


std::vector<std::string> argumentsFromCommandline() { 
	std::vector<std::string> arguments; 
	int nArgs;
	LPWSTR *szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs); 
	if(szArglist != 0) { 
		for(int x = 1; x < nArgs; ++x) { 
			arguments.push_back(std::string()); 
			std::string& sArg = arguments[arguments.size() - 1]; 
			sArg.resize((int)wcslen(szArglist[x])); 
			WideCharToMultiByte(CP_ACP, NULL, szArglist[x], (int)wcslen(szArglist[x]), (PCHAR)sArg.data(), sArg.size(), 0, 0); 
		} 
		LocalFree(szArglist); 
	} 

	return arguments; 
} 

__int64 IEEE754Double2Int(double f2Int, BYTE scale_by_exp2) { 
	if(scale_by_exp2 > 52) { 
		scale_by_exp2 = 52; 
	} 

	__int64& ddwHexImage = *(__int64*)((PVOID)(&f2Int));  

	bool nMinus = (ddwHexImage & 0x8000000000000000) != 0; 
	__int64 nExp = ((ddwHexImage & 0x7ff0000000000000) >> 52); 
	__int64 nMantissa = (ddwHexImage & 0x000fffffffffffff); 

//	Check special cases.
	if(nExp == 2047 && nMantissa != 0) // NaN (Not a number) 
		return 0; 
	if(nExp == 2047 && nMantissa == 0) // +- Infinity 
		return nMinus? (__int64)0x8000000000000000: (__int64)0x7fffffffffffffff; 
	if(nExp == 0 && nMantissa != 0) // Denormalized number: nMantissa * exp(-1022)
		return 0; 
	if(nExp == 0 && nMantissa == 0) // Zero.
		return 0; 

	__int64 nExp1 = nExp - 1023; 
	__int64 nExp2 = 0; 
	nExp1 += scale_by_exp2; 
	if(nExp1 < 0) { 
		nExp1 = 0; 
		nExp2 = 1023 - nExp; 
	} 

	__int64 nInt1 = (1 << nExp1); 
	__int64 nInt2 = (nMantissa >> (52 - nExp1)); 
	__int64 nMask = (__int64)0xfff0000000000000 >> (nExp1); 
	__int64 nInt3 = (nMantissa & ~nMask); 

	__int64 nTotal = 0; 
	__int64 nScale = 5; 
	__int64 nZeros = 0; 

	if(nExp2) { 
		while(--nExp2) { 
			nScale *= 5; 
			++nZeros;
		} 
		nTotal = nScale; 
		nScale *= 5; 
		nZeros /= 3; 
	} 

	int nCount = 0; 

	nInt3 <<= nExp1; 
	while(nTotal < 10000000000000000 && (nInt3 & 0x000fffffffffffff) != 0) { 
		nTotal *= 10; 

		if(nInt3 & 0x0008000000000000) { 
			if(nTotal == 0) { 
				nZeros += nCount/3; 
			} 
			nTotal += nScale; 
		} 

		nScale *= 5; 
		nInt3 <<= 1; 

		++nCount; 
	} 

	__int64 nInt = nInt1 + nInt2; 

	return (nMinus? (-1 * nInt): nInt); 
} 

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


std::string _g_program_files; 
std::string _g_boot_drive; 

