// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef wsiclassfactorystdafxH
#define wsiclassfactorystdafxH

#pragma once

#define autocreateserialization_not_necessaryH

#include ".\OSSpecific\OSDependent_classfactory.h"

#ifdef max 
#undef max 
#endif 

#ifdef min 
#undef min 
#endif 

#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

#define YYERROR_VERBOSE
#define YYINCLUDED_STDLIB_H

#include <ios>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <utility>
#include <stack>
#include <vector>
#include <algorithm>
#include <functional>

#if _MSC_VER > 1700
#include <unordered_map>
#endif



#include "..\IPCGate\IPCGate.h"

#ifndef IMP

#ifdef EXP
#undef EXP
#endif

#define EXP EXPORTIPCINTERFACE
#else
#define EXP IMPORTIPCINTERFACE
#endif



int ReadTextFile(const std::string& filename, std::string& str);
int WriteTextFile(const std::string& filename, const std::string& str);
int AppendTextFile(const std::string& filename, const std::string& str);



#endif //wsiclassfactorystdafxH
