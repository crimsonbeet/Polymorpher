
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#ifndef NO_WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif 

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>

#include <io.h>
#include <time.h>
#include <sys\timeb.h>
#include <tchar.h>

typedef int socklen_t; 
typedef DWORD return_t; 
typedef HANDLE sem_t; 
typedef HANDLE pthread_t; 

#define EXPORTIPCINTERFACE _declspec(dllexport)
#define IMPORTIPCINTERFACE 

#define EXPORTDLLVARIABLES _declspec(dllexport)
#define IMPORTDLLVARIABLES _declspec(dllimport)

