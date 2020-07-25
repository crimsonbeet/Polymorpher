// Created by Alex Nikol Bourakov (ANB) around April/07


#include "stdafx_classfactory.h"
#include "WSIObject.h"



substring_locator::substring_locator() {
	clear();
}
std::string substring_locator::str() const {
	if(_begin == 0) {
		return std::string();
	}
	return std::string(_begin, size());
}
bool substring_locator::operator == (const std::string& s) const {
	if(s.size() == size()) {
		if(_begin) {
			return memcmp(s.c_str(), _begin, size()) == 0;
		}
	}
	return false;
}
bool substring_locator::operator == (const substring_locator& s) const {
	if(s.size() == size()) {
		if(_begin) {
			return memcmp(s._begin, _begin, size()) == 0;
		}
	}
	return false;
}
substring_locator::operator std::string() const {
	return str();
}

substring_locator& substring_locator::operator = (const char* const s) { 
	_begin = _end = s; 
	if(s) { 
		_end += strlen(s); 
	} 
	return *this; 
} 
substring_locator::substring_locator(const char* const s) { 
	_begin = _end = s; 
	if(s) { 
		_end += strlen(s); 
	} 
} 

bool substring_locator::operator < (const substring_locator& s) const { 
	if(_begin == _end) { 
		return s._end > s._begin; 
	} 
	if(s._begin == s._end) { 
		return false; 
	} 
	size_t lsize = size(); 
	size_t rsize = s.size(); 
	int rc = strncmp(_begin, s._begin, std::min(lsize, rsize)); 
	if(rc < 0) { 
		return true; 
	} 
	if(rc > 0) { 
		return false; 
	} 
	return lsize < rsize; 
} 

const char *substring_locator::fix_quote() const { 
	switch(*_begin) { 
		case '\'': case '\"': 
			if(_begin < _end) { 
				return _begin + 1; 
			} 
			break;
	} 
	return _begin; 
} 


int substring_locator::Atoi() const { 
	const char *begin = fix_quote(); 
	if(begin == _end) { 
		return 0; 
	} 
	if(*_end >= '0' && *_end <= '9') { 
		return atoi(std::string(begin, _end - begin).c_str());
	} 
	return atoi(begin);
} 
long substring_locator::Atol() const { 
	const char *begin = fix_quote(); 
	if(begin == _end) { 
		return 0; 
	} 
	if(*_end >= '0' && *_end <= '9') { 
		return atol(std::string(begin, _end - begin).c_str());
	} 
	return atoi(begin);
} 
__int64 substring_locator::Atoi64() const { 
	const char *begin = fix_quote(); 
	if(begin == _end) { 
		return 0; 
	} 
	if(*_end >= '0' && *_end <= '9') { 
		return _atoi64(std::string(begin, _end - begin).c_str());
	} 
	return _atoi64(begin);
} 

size_t EXP
IEEE754DoubleDecode(const char* str, size_t str_size, int& nSign, int& nExp, __int64& nMantissa); 

double substring_locator::Atof() const { 
	const char *begin = fix_quote(); 
	size_t nsize = _end - begin; 
	if(nsize == 0) { 
		return 0; 
	} 
	switch(*begin) {
	case 'S': case '#': case '!': case '*': case '|':
		if(nsize >= 4) {
			int nSign;
			int nExp;
			__int64 nMantissa;

			double what = 0;
			if(IEEE754DoubleDecode(begin, nsize, nSign, nExp, nMantissa) >= 4) {
				__int64& ddwHexImage = *(__int64*)((PVOID)(&what));
				if(nSign) {
					ddwHexImage = 0x8000000000000000;
				}
				else {
					ddwHexImage = 0;
				}

				ddwHexImage = ddwHexImage | ((__int64)nExp) << 52;
				ddwHexImage = ddwHexImage | nMantissa;
			}
			return what;
		}
		break;
	default:
		break;
	}
	if(*_end >= '0' && *_end <= '9') { 
		return atof(std::string(begin, nsize).c_str());
	} 
	return atof(begin);
} 


paired_captures::paired_captures() {
	index = 0;
}
void paired_captures::clear() {
	first.clear();
	second.clear();
	index = 0;
}
bool paired_captures::empty() const {
	return first.empty() && second.empty();
}



wsi_object::wsi_object() {
	_nInnerObjects = 0;
}

void wsi_object::clear() {
	_nInnerObjects = 0;
	_header.clear();
	_vecCaptures.clear();
}

bool wsi_object::empty() const {
	return (_header.empty() && _vecCaptures.empty() && !_nInnerObjects);
}

void wsi_object::log(const std::string& sError) const {
	std::ostringstream msg;
	msg << std::endl;
	msg
	<< sError << ' '
	<< _header.first.str() << '@'
	<< _header.index << (_header.second.size()? '=': ' ')
	<< _header.second.str() << ' ';

	std::vector<paired_captures>::const_iterator iter = _vecCaptures.begin();
	while(iter != _vecCaptures.end()) {
		msg
		<< (*iter).first.str() << '@'
		<< (*iter).index << '='
		<< (*iter).second.str() << ' ';
		++iter;
	}
	msg << std::endl;

#if defined(__BORLANDC__)
	OutputDebugStringA(msg.str().c_str());
#else
	IPCLog(msg.str()); 
	//std::cout << msg.str();
#endif
}




