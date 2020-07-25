// Created by Alex Nikol Bourakov (ANB) around April/07


#include "stdafx_classfactory.h"


#if defined(_UCC) || defined(__BORLANDC__)
#if defined(_UCC)
void IPCClassFactoryDllMain() {
}
#endif

void EXP wsi_getMember(std::string &wsi, const __int64& x) {
	if(x != 0) {
		char psz[64];
		sprintf(psz, "%Ld", x);
		wsi = psz;
	}
}


#elif defined(__GCC__)
void __attribute__((constructor)) IPCClassFactoryDllMain() {
}

#elif defined(_MSC_VER) 

void EXP wsi_getMember(std::string &wsi, const __int64& x) {
	if(x != 0) {
		char psz[64];
		sprintf(psz, "%I64d", x);
		wsi = psz;
	}
}

#endif

