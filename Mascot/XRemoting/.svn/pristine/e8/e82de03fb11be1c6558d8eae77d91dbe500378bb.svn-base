// Windows version

// Created by Alex Nikol Bourakov (ANB) around April/07

#include "stdafx_gate.h" 
#include "WSIStorage.h" 

const char *g_zeromarker = "0000000010X"; 
const int g_zeromarkerlen = 10; 
const char *g_zeromarkerformat = "%010d"; 
const char *g_markerformat = "%06d"; 
const int g_markerlen = 6; 

PIWsiStorage EXP CREATEIWsiStorageObject() { 
	return new CWsiStorage(); 
} 
VOID EXP DESTROYIWsiStorageObject(PIWsiStorage *ppObj) { 
	if(ppObj && *ppObj) { 
		delete *ppObj; 
		*ppObj = 0; 
	} 
} 

PIWsiStorageClient EXP CREATEIWsiStorageClientObject() { 
	return new CWsiStorageClient(); 
} 
VOID EXP DESTROYIWsiStorageClientObject(PIWsiStorageClient *ppObj) { 
	if(ppObj && *ppObj) { 
		delete *ppObj; 
		*ppObj = 0; 
	} 
} 

void BuildPath(const std::string& folder, const std::string& handle, std::string& path) { 
	path.reserve(folder.size() + handle.size() + 16); 
	path = folder + "\\" + handle; 
} 

CWsiStorage::CWsiStorage() { 
	_hcurrent = 0; 
	_pcurrent = g_zeromarkerlen; 
	_maxsize = 0; 

	_set_dir = true; 
} 

CWsiStorage::~CWsiStorage() { 
} 

void CWsiStorage::Init(const std::string& folder, const std::string& pattern, int maxsize) { 
	_folder = folder; 
	_pattern = pattern; 
	_maxsize = maxsize; 

	_set_dir = true; 

	HANDLE hDir = INVALID_HANDLE_VALUE;
	try { 
		hDir = CreateFileA(_folder.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ|FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL); 
		if(hDir == INVALID_HANDLE_VALUE) { 
			if(!CreateDirectoryA(_folder.c_str(), NULL)) { 
				_set_dir = false; 
			} 
		} 
	} 
	catch(...) { 
	} 

	if(hDir != INVALID_HANDLE_VALUE) { 
		CloseHandle(hDir); 
		hDir = INVALID_HANDLE_VALUE; 
	} 

	SortByLastWriteDate(); 

	if(_handles.size()) { 
		_handle = _handles[_handles.size() - 1]; 
	} 
} 

void CWsiStorage::SortByLastWriteDate() { 
	CHAR szCurrDir[_MAX_DIR]; 
	GetCurrentDirectoryA(_MAX_DIR, szCurrDir); 
	if(_set_dir) { 
		SetCurrentDirectoryA(_folder.c_str()); 
	} 

	_agate.lock(); 

	std::multimap<__int64, std::string> files_map; 

	WIN32_FIND_DATAA strFindData;
	HANDLE hFind = FindFirstFileA(_pattern.c_str(), &strFindData); 
	BOOL fOk = (hFind != INVALID_HANDLE_VALUE);
	while(fOk) { 
		if(!(strFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) { 
			files_map.insert(std::make_pair(*(__int64*)(&strFindData.ftLastWriteTime), std::string(strFindData.cFileName)));
		} 

		fOk = FindNextFileA(hFind, &strFindData); 
	} 

	if(hFind != INVALID_HANDLE_VALUE) { 
		FindClose(hFind);
	} 

	_handles.resize(0); 

	std::multimap<__int64, std::string>::const_iterator citer = files_map.begin(); 
	while(citer != files_map.end()) { 
		_handles.push_back(citer->second); 
		++citer; 
	} 

	_agate.unlock(); 

	if(_set_dir) { 
		SetCurrentDirectoryA(szCurrDir); 
	} 
} 


int CWsiStorage::FindHandlePosition(const std::string& filename) { 
	int pos = 0; 
	if(filename.size() != 0) {
		for(size_t i = 0, j = _handles.size(); i < j; ++i) {
			std::string &name = _handles[i];
			if(name == filename) {
				pos = (int)i;
				break;
			}
		}
	}
	return pos; 
} 


void CWsiStorage::SetPosition(const std::string& filename, int infile_position) { 
	_agate.lock(); 

	_hcurrent = FindHandlePosition(filename); 
	if(_hcurrent) { 
		_pcurrent = infile_position < g_zeromarkerlen? g_zeromarkerlen: infile_position; 
	} 
	else { 
		_pcurrent = g_zeromarkerlen; 

		_hcurrent = _handles.size() > 1? 1: 0; 
	} 

	if(_hcurrent < (int)_handles.size()) { 
		_handle = _handles[_hcurrent]; 
	} 

	_agate.unlock(); 
} 

void CWsiStorage::Rewind() { 
	Synch(); 
	SetPosition(std::string(), g_zeromarkerlen); 
} 


DWORD CWsiStorage::MyGetStorageFileSize(HANDLE hFile) { 
	char marker[16]; 
	DWORD dw; 
	SetFilePointer(hFile, 0, 0, FILE_BEGIN); 
	if(ReadFile(hFile, (void*)marker, g_zeromarkerlen, &dw, 0)) { 
		if(dw == g_zeromarkerlen && std::count_if(marker, marker + dw, isdigit) == (int)dw) { 
			marker[g_zeromarkerlen] = 0; 
			return atoi(marker); 
		} 
	} 
	return 0; 
} 

DWORD CWsiStorage::MyInitializeStorageFile(HANDLE hFile) { 
	LONG minimal_length = (LONG)_maxsize + (LONG)_maxsize/10; 
	if(GetFileSize(hFile, NULL) < (DWORD)minimal_length) { 
		SetFilePointer(hFile, minimal_length, 0, FILE_BEGIN); 
		SetEndOfFile(hFile); 
	} 

	SetFilePointer(hFile, 0, 0, FILE_BEGIN); 
	DWORD dw = 0; 
	if(WriteFile(hFile, g_zeromarker, g_zeromarkerlen + 1, &dw, NULL)) { 
		dw = g_zeromarkerlen; 
	} 
	return dw; 
} 

bool CWsiStorage::Read(std::string& row) { 
	bool fOk = false; 
	bool fDone = false; 

	_agate.lock(); 

	while(!fOk && !fDone && _hcurrent < (int)_handles.size()) { 
		DWORD dw; 
		HANDLE hFile = INVALID_HANDLE_VALUE; 
		dw = ERROR_SHARING_VIOLATION; 
		while(dw == ERROR_SHARING_VIOLATION) { 
			BuildPath(_folder, _handle, _handlepath); 

			hFile = CreateFileA(_handlepath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
			dw = GetLastError(); 
			if(dw == ERROR_SHARING_VIOLATION) { 
				Sleep(10); 
			} 
		} 
		if(hFile == INVALID_HANDLE_VALUE) { 
			break; 
		} 

		DWORD dwFileSize = MyGetStorageFileSize(hFile); 

		if(SetFilePointer(hFile, _pcurrent, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER) { 
			_pcurrent = dwFileSize; // position to the end, so read fails. 
		} 

		if((DWORD)_pcurrent < dwFileSize) { 
			char szBuf[8]; 
			memset(szBuf, 0, sizeof(szBuf)); 
			ReadFile(hFile, (void*)szBuf, g_markerlen, &dw, 0); 
			if(dw == g_markerlen) { 
				if(std::count_if(szBuf, szBuf + dw, isdigit) == (int)dw) {
					int size = atoi(szBuf); 
					row.resize(size); 
					if(ReadFile(hFile, (void*)row.data(), (DWORD)row.size(), &dw, 0)) { 
						if(dw == (DWORD)size) { 
							fOk = true; 
							_pcurrent += g_markerlen; 
							_pcurrent += size; 
						} 
					} 
				} 
				else { 
					_pcurrent = dwFileSize; 
				} 
			} 
		} 

		CloseHandle(hFile); 

		if(!fOk) { 
			_agate.unlock(); 
			Synch(); 
			_agate.lock(); 

			_hcurrent = FindHandlePosition(_handle); 
			if(_hcurrent < (int)(_handles.size() - 1)) { 
				_handle = _handles[++_hcurrent]; 
				_pcurrent = g_zeromarkerlen; 
			} 
			else { 
				fDone = true; 
			} 
		} 
	} 
	_agate.unlock(); 

	return fOk; 
} 

bool CWsiStorage::Append(const std::string& row) { 
	DWORD dw = 0; // number of bytes of last write. 
	bool fOk = false; 

	_agate.lock(); 

	size_t ilast = _handles.size(); 
	if(ilast > 0 && row.size() < 524288) { 
		DWORD dwFileSize = 0; 
		HANDLE hFile = INVALID_HANDLE_VALUE; 
		bool fErase = false; 
		while(hFile == INVALID_HANDLE_VALUE) { 
			BuildPath(_folder, _handles[ilast - 1], _handlepath); 

			hFile = CreateFileA(_handlepath.c_str(), GENERIC_WRITE | GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL/* | FILE_FLAG_WRITE_THROUGH*/, NULL); 
			if(hFile != INVALID_HANDLE_VALUE) { 
				if(fErase) { 
					dwFileSize = 0; 
				} 
				else { 
					dwFileSize = MyGetStorageFileSize(hFile); 
				} 
				if(dwFileSize == 0) { 
					dwFileSize = MyInitializeStorageFile(hFile); 
				} 
				if(dwFileSize > (DWORD)_maxsize) { 
					if(ilast > 1) { 
						_handles.push_back(*_handles.begin()); 
						_handles.erase(_handles.begin()); 
					} 
					fErase = true; 
					CloseHandle(hFile); 
					hFile = INVALID_HANDLE_VALUE; 
				} 
			} 
			else 
			if(GetLastError() == ERROR_SHARING_VIOLATION) { 
				Sleep(10); 
			} 
			else { 
				break; 
			} 
		} 

		if(hFile != INVALID_HANDLE_VALUE) { 
			SetFilePointer(hFile, dwFileSize, 0, FILE_BEGIN); 

			int size = (int)row.size(); 
			char szBuf[16]; 
			sprintf(szBuf, g_markerformat, size); 
			if(WriteFile(hFile, szBuf, (DWORD)g_markerlen, &dw, NULL)) { 
				dwFileSize += g_markerlen; 
				if(WriteFile(hFile, row.data(), (DWORD)size, &dw, NULL)) { 
					WriteFile(hFile, "X", 1, &dw, NULL); // place non digit character as EOF marker.

					dwFileSize += size; 
					sprintf(szBuf, g_zeromarkerformat, dwFileSize); 
					SetFilePointer(hFile, 0, 0, FILE_BEGIN); 
					if(WriteFile(hFile, szBuf, g_zeromarkerlen, &dw, NULL)) { 
						fOk = true; 
					} 
				} 
			} 
			if(!fOk) { 
				SetFilePointer(hFile, 0, 0, FILE_BEGIN); 
				SetEndOfFile(hFile); 
			} 

			CloseHandle(hFile); 
		} 
	} 

	_agate.unlock(); 

	return fOk; 
} 


