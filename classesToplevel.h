
#pragma once


//#include "OleDate.h" 

#include <string>

extern std::string _g_program_files; 
extern std::string _g_boot_drive; 


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



