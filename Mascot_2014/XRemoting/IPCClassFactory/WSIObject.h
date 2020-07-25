// Created by Alex Nikol Bourakov (ANB) around April/07


#ifndef wsipobjectH
#define wsipobjectH

#include ".\..\Include\WSIClassFactoryInterfaces.h"
#include ".\..\IPCInterface\IPCInterface.h"


struct EXP substring_locator {
	const char *_begin;
	const char *_end;

	substring_locator();
	substring_locator(const char* const s);
	std::string str() const;
	bool operator == (const std::string& s) const;
	bool operator < (const substring_locator& s) const;
	bool operator == (const substring_locator& s) const;
	substring_locator& operator = (const char* const s);
	operator std::string() const;
	inline void clear() {
		_begin = 0;
		_end = 0;
	}
	inline size_t size() const {
		return (size_t)(_end - _begin);
	}
	inline bool empty() const {
		return size() == 0;
	}
	const char *fix_quote() const; 

	int Atoi() const; 
	long Atol() const; 
	double Atof() const; 
	__int64 Atoi64() const; 
};




struct paired_captures {
	substring_locator first;
	substring_locator second;
	substring_locator anamespace; 
	int index;

	paired_captures();
	void clear();
	bool empty() const;
};





class wsi_object {
public:
	paired_captures _header;
	std::vector<paired_captures> _vecCaptures;

	int _nInnerObjects;
	wsi_object();

	void clear();

	bool empty() const;

	void log(const std::string& sError) const;
};


#endif //wsipobjectH

