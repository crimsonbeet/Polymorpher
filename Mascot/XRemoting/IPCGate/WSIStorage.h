// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef WSISTORAGE_H
#define WSISTORAGE_H

#include ".\IPCGate.h"

class CWsiStorage: public IWsiStorage { 
public: 
	std::vector<std::string> _handles; 
	int _hcurrent; 
	int _pcurrent; 
	int _maxsize; 

	std::string _handle; 
	std::string _handlepath; 

	bool _set_dir; 

protected: 
	void SortByLastWriteDate(); 
	DWORD MyGetStorageFileSize(HANDLE hFile); 
	DWORD MyInitializeStorageFile(HANDLE hFile); 

	wsi_gate _agate; 

public: 
	std::string _folder; 
	std::string _pattern; 

	CWsiStorage(); 
	virtual ~CWsiStorage(); 

	void Synch() { 
	} 

	void Init(const std::string& folder, const std::string& pattern, int maxsize = 4194304); 

	const std::string& GetHandle() { 
		return _handle; 
	} 

	int GetCurrentPos() { 
		return _pcurrent; 
	} 

	void Rewind(); 
	bool Read(std::string& row); 
	bool Append(const std::string& row); 

	void SetPosition(const std::string& filename, int position); 
	int FindHandlePosition(const std::string& filename); 
}; 

class CWsiStorageClient: public CWsiStorage { 
public: 
	void Synch() { 
		SortByLastWriteDate(); 
	} 
}; 

#endif //WSISTORAGE_H
