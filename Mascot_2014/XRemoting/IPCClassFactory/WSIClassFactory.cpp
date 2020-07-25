// Created by Alex Nikol Bourakov (ANB) around April/07

#ifdef __BORLANDC__
//#pragma hdrstop
#endif

#include "stdafx_classfactory.h"
#include "WSIClassFactory.h"
#include "WSIParser.h"
#include "XMLParser.h"



PCHAR DEC2HEX = "0123456789ABCDEF"; 


//http://en.wikipedia.org/wiki/List_of_XML_and_HTML_character_entity_references

const char *XML_Character_Eentities[5] = {"quot;", "amp;", "apos;", "lt;", "gt;"}; 




inline char HEX2DEC(char hex, bool& err) {
	err = false;
	switch(hex) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			return hex - '0';
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
			return (hex - 'a') + 10;
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
			return (hex - 'A') + 10;
	}
	err = true;
	return 0;
}


char XMLCE2CHAR(const char *str, const char *last, size_t& ncount) { 
	ncount = 0; 
	const char *ent_buf = 0; 
	int ent = -1; 
	switch(*str++) { 
	case 'q': 
		ent_buf = XML_Character_Eentities[ent = 0] + 1; 
		break; 
	case 'a': 
		if(str < last) { 
			switch(*str++) { 
			case 'm': 
				ent_buf = XML_Character_Eentities[ent = 1] + 2; 
				break; 
			case 'p': 
				ent_buf = XML_Character_Eentities[ent = 2] + 2; 
				break; 
			} 
		} 
		break; 
	case 'l': 
		ent_buf = XML_Character_Eentities[ent = 3] + 1; 
		break; 
	case 'g': 
		ent_buf = XML_Character_Eentities[ent = 4] + 1; 
		break; 
	} 
	if(ent > -1) { 
		while(*ent_buf != 0) { 
			if(str >= last) { 
				return '&'; 
			} 
			else 
			if(*str++ != *ent_buf++) { 
				return '&'; 
			} 
		} 
		switch(ent) { 
		case 0: ncount = 5; return '\"'; 
		case 1: ncount = 4; return '&'; 
		case 2: ncount = 5; return '\''; 
		case 3: ncount = 3; return '<'; 
		case 4: ncount = 3; return '>'; 
		} 
	} 
	return '&'; 
} 


char JSONCE2CHAR(const char *str, const char *last, size_t& ncount) { 
	ncount = 0; 

	bool err = false;

	char ent_hex[4]; 
	char ent;
	switch((ent = *str++)) {
	case 'U': 
	case 'u': 
		for(int x = 0; !err && x < 4; ++x) { 
			ent_hex[x] = HEX2DEC(*str++, err); 
		} 
		if(!err) { 
			ent = (ent_hex[2] << 4) | ent_hex[3]; 
			ncount = 5; 
		} 
		break; 
	case '\"':
		ncount = 1;
		break;
	case '\\': 
		ncount = 1; 
		break; 
	case '/':
		ncount = 1;
		break;
	case 'b':
		ent = 0x20;
		ncount = 1;
		break;
	case 'f':
		ent = 0x20;
		ncount = 1;
		break;
	case 'n':
		ent = 0xA;
		ncount = 1;
		break;
	case 'r':
		ent = 0xD;
		ncount = 1;
		break;
	case 't':
		ent = 0x9;
		ncount = 1;
		break;
	}
	return ent; 
} 


template<typename T>
size_t WSIPutString(const char* wsi_inp_string, size_t isize, T *out_string, size_t osize) { 
	char marker = 0; 
	const char *last = wsi_inp_string + isize; 
	if(*wsi_inp_string == '\'' || *wsi_inp_string == '\"') { 
		marker = *wsi_inp_string; 
		++wsi_inp_string; 
	} 
	size_t x = 0; 
	while(wsi_inp_string < last && x < osize) {
		if(*wsi_inp_string == marker) {
			if(++wsi_inp_string < last) { 
				if(*wsi_inp_string == marker) {
					out_string[x] = *wsi_inp_string;
					if(++wsi_inp_string < last) {
						++x;
					}
				}
				else {
					break;
				}
			} 
		}
		else {
			out_string[x++] = *(wsi_inp_string++);
		}
	}
	if(x < osize) {
		out_string[x] = 0;
	}
	return x;
}


size_t EXP
WSIEncode(const char* wsi_inp_string/*zero termninated*/, size_t isize, PCHAR wsi_out_string/*must be at least three times larger than input string*/, BOOL *pAlert) {
	if(pAlert) {
		*pAlert = false;
	}

	size_t x = 0;
	size_t j = 0;
	for(j = 0; wsi_inp_string[j] != 0 && j < isize; ++j) {
		switch(wsi_inp_string[j]) {
			case '.': case '-': case '+': case '_':
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9': break;
			default:
			if(wsi_inp_string[j] >= 'A' && wsi_inp_string[j] <= 'Z') {
			}
			else
			if(wsi_inp_string[j] >= 'a' && wsi_inp_string[j] <= 'z') {
			}
			else {
				wsi_out_string[x++] = '%';
				wsi_out_string[x++] = DEC2HEX[(wsi_inp_string[j] >> 4) & 0x0f];
				wsi_out_string[x++] = DEC2HEX[wsi_inp_string[j] & 0x0f];

				if(pAlert) {
					if(wsi_inp_string[j] < 0x20) {
						if(wsi_inp_string[j]) { 
							*pAlert = true;
						} 
					}
				}
				continue; 
			}
			break; 
		}

		wsi_out_string[x++] = wsi_inp_string[j];
	}

	wsi_out_string[x] = 0;
	return x;
}


size_t EXP
JSONEncode(const char* wsi_inp_string/*zero termninated*/, size_t isize, PCHAR wsi_out_string/*must be at least five times larger than input string*/, BOOL *pAlert) {
	if(pAlert) {
		*pAlert = false;
	}

	const char *ent_buf = 0; 
	char esc_buf[16]; 

	size_t x = 0;
	size_t j = 0;
	for(j = 0; wsi_inp_string[j] != 0 && j < isize; ++j) { 
		switch(wsi_inp_string[j]) { 
			case '\"': ent_buf = "\\u0022"; break; 
			case '\\': ent_buf = "\\u005C"; break; 
			default: 
				if(wsi_inp_string[j] < 0x20) { 
					sprintf(esc_buf, "\\u%04X", (int)wsi_inp_string[j]); 
					ent_buf = esc_buf; 
					if(pAlert) { 
						if(wsi_inp_string[j]) { 
							*pAlert = true; 
						} 
					} 
				} 
				else { 
					wsi_out_string[x++] = wsi_inp_string[j]; 
					continue; 
				} 
				break;
		} 

		while(*ent_buf) { 
			wsi_out_string[x++] = *ent_buf++; 
		} 
	} 

	wsi_out_string[x] = 0;
	return x;
}

size_t EXP
XMLEncode(const char* wsi_inp_string/*zero termninated*/, size_t isize, PCHAR wsi_out_string/*must be at least five times larger than input string*/, BOOL *pAlert) {
	if(pAlert) {
		*pAlert = false;
	}

	const char *ent_buf = 0; 
	size_t x = 0;
	size_t j = 0;
	for(j = 0; wsi_inp_string[j] != 0 && j < isize; ++j) { 
		switch(wsi_inp_string[j]) { 
			case '\"': ent_buf = XML_Character_Eentities[0]; break; 
			case '&': ent_buf = XML_Character_Eentities[1]; break; 
			case '\'': ent_buf = XML_Character_Eentities[2]; break; 
			case '<': ent_buf = XML_Character_Eentities[3]; break; 
			case '>': ent_buf = XML_Character_Eentities[4]; break; 
			default: 
				wsi_out_string[x++] = wsi_inp_string[j]; 
				if(pAlert) { 
					if(wsi_inp_string[j] < 0x20) { 
						if(wsi_inp_string[j]) { 
							*pAlert = true; 
						} 
					} 
				} 
				continue; 
		} 

		wsi_out_string[x++] = '&'; 
		while(*ent_buf) { 
			wsi_out_string[x++] = *ent_buf++; 
		} 
	} 

	wsi_out_string[x] = 0;
	return x;
}

size_t EXP
WSIDecodeEx(const char* wsi_inp_string, size_t isize, PCHAR wsi_out_string, size_t osize, int encoding/* 0 - no encoding, 1 - url(%) encoding, 2 - xml(&) encoding, 4 - json(\uXXXX) encoding, 3 - url & xml encoding*/) {
	switch(*wsi_inp_string) { 
	case '\'': case '\"': 
		if(isize > 1) { 
			if(*wsi_inp_string == *(wsi_inp_string + isize - 1)) { 
				size_t nsize = std::min(isize - 2, osize); 
				memcpy(wsi_out_string, wsi_inp_string + 1, nsize); 
				if(nsize < osize) { 
					wsi_out_string[nsize] = 0; 
				} 
				return nsize; 
			} 
		} 
		break;
	}

	if(!encoding) {
		memcpy(wsi_out_string, wsi_inp_string, std::min(isize, osize));
		if(isize < osize) {
			wsi_out_string[isize] = 0;
		}
		return isize;
	} 

	size_t x = 0;
	size_t j = 0;
	for(j = 0; j < isize && wsi_inp_string[j] != 0 && x < osize; ++j) {
		switch(wsi_inp_string[j]) { 
		case '%':
			if((encoding & 1) != 0) {
				int bit4[2];
				bool err = false; 
				size_t h = 0; 
				size_t i = j; 
				while(h < 2 && i < isize && wsi_inp_string[i] && !err) {
					bit4[h++] = HEX2DEC(wsi_inp_string[++i], err);
				} 
				if(h == 2 && !err) { 
					j = i; 
					CHAR c = (bit4[0] << 4) | (bit4[1]);
					wsi_out_string[x++] = c; 
				} 
				else { 
					wsi_out_string[x++] = wsi_inp_string[j]; 
				} 
				continue;
			} 
			break; 
		case '&':
			if((encoding & 2) != 0) {
				size_t ncount = 0; 
				wsi_out_string[x++] = XMLCE2CHAR(wsi_inp_string + j + 1, wsi_inp_string + isize, ncount); 
				j += ncount; 
				continue;
			} 
			break; 
		case '\\':
			if((encoding & 4) != 0) {
				size_t ncount = 0; 
				wsi_out_string[x++] = JSONCE2CHAR(wsi_inp_string + j + 1, wsi_inp_string + isize, ncount); 
				j += ncount; 
				continue;
			} 
			break; 
		}

		wsi_out_string[x++] = wsi_inp_string[j];
	} 

	if(x < osize) {
		wsi_out_string[x] = 0;
	}
	return x;
}


size_t EXP
WSIDecode(const char* wsi_inp_string, size_t isize, PCHAR wsi_out_string, size_t osize) {
	return WSIDecodeEx(wsi_inp_string, isize, wsi_out_string, osize, 3);
}


size_t EXP
IEEE754DoubleEncode(int nSign, int nExp, __int64 nMantissa, PCHAR str, int ver) {
	size_t j;
	size_t x;
	size_t k;

	if(ver) {
		if(nExp >= 0x400) {
			str[0] = nSign? '*': '|';
			nExp -= 0x400;
		}
		else {
			str[0] = nSign? '#': '!';
		}
		j = 1;
	}
	else {
		str[0] = 'S';
		str[1] = nSign? '1': '0';
		str[2] = 'E';
		j = 3;
	}

	x = j;
	for(int e = 0; e < 3; ++e) {
		str[j] = DEC2HEX[((nExp & 0xf00) >> 8)];
		if(x != j || str[j] != '0') {
			++j;
		}
		nExp <<= 4;
	}
	if(j == x) {
		++j;
	}

	str[j++] = 'M';
	x = j;
	k = 0;
	for(int m = 0; m < 13; ++m) {
		int iszero = (str[j] = DEC2HEX[(int)((nMantissa & 0x000f000000000000) >> 48)]) == '0';
		if(x != j || !iszero) {
			if(iszero && ver) {
				++k;
			}
			else {
				k = 0;
			}
			++j;
		}
//		Borland can not handle __int64 arithmetics correctly.
//		nMantissa <<= 4;
		nMantissa = nMantissa << 4;
	}
	if(j == x) {
		++j;
	}
	if(k > 1) {
		str[j -= k] = 'F' + k;
		++j;

	}

	str[j] = 0;
	return j;
}

size_t EXP
IEEE754DoubleDecode(const char* str, size_t str_size, int& nSign, int& nExp, __int64& nMantissa) {

	size_t j = 1;

	nExp = 0;
	nMantissa = 0;

	switch(str[0]) {
		case '#': case '*':
			nSign = 1;
			break;
		case '!': case '|':
			nSign = 0;
			break;
		case 'S':
			switch(str[1]) {
				case '1':
					nSign = 1;
					break;
				case '0':
					nSign = 0;
					break;
				default:
					return 1;
			}
			if(str[2] != 'E') {
				return 2;
			}
			j = 3;
			break;
		default:
			return 1;
	}

	bool done = false;
	while(j < 6 && j < str_size && !done) {
		char h = HEX2DEC(str[j], done);
		if(!done) {
			nExp = (nExp << 4) | h;
			++j;
		}
	}

	switch(str[0]) {
		case '*': case '|':
			nExp += 0x400;
			break;
		default:
			break;
	}

	if(str[j++] != 'M') {
		return 3;
	}

	done = false;
	while(j < 20 && j < str_size && !done) {
		if(str[j] > 'F') {
			for(int z = 0, k = str[j] - 'F'; z < k; ++z) {
				nMantissa = (nMantissa << 4);
				++j;
			}
			break;
		}
		char h = HEX2DEC(str[j], done);
		if(!done) {
			nMantissa = (nMantissa << 4) | h;
			++j;
		}
	}

	return j;
}

void wsi_encodeDouble(std::string &wsi, const double& x, int ver) {
	if(x != 0.0) {
		__int64& ddwHexImage = *(__int64*)((PVOID)(&x));

		int nSign = (ddwHexImage & 0x8000000000000000)? 1: 0;
		int nExp = (int)((ddwHexImage & 0x7ff0000000000000) >> 52);
		__int64 nMantissa = (ddwHexImage & 0x000fffffffffffff);

		wsi.resize(32);
		wsi.resize(IEEE754DoubleEncode(nSign, nExp, nMantissa, (PCHAR)wsi.data(), ver > 2));
	}
	else { 
		wsi.resize(0);
	} 
}

void wsi_encodeFloat(std::string &wsi, const float& x, int ver) {
	if(x != 0.0) {
		__int32& dwHexImage = *(__int32*)((PVOID)(&x));

		int nSign = (dwHexImage & 0x80000000)? 1: 0;
		int nExp = (int)((dwHexImage & 0x7f800000) >> 23);
		__int64 nMantissa = ((__int64)(dwHexImage & 0x007fffff)) << 29;

		switch(nExp) {
			case 255:
				nExp = 2047;
				break;
			case 0: // can not be zero number, so Denormalized number: nMantissa * exp(-126).
			{
				double y = x;
				wsi_encodeDouble(wsi, y, ver > 2);
				return;
			}
			default:
				nExp = nExp - 127 + 1023;
				break;
		}

		wsi.resize(32);
		wsi.resize(IEEE754DoubleEncode(nSign, nExp, nMantissa, (PCHAR)wsi.data(), ver > 2));
	}
	else { 
		wsi.resize(0);
	} 
}





void ABFundamentalTypesSerializer::wsi_getMember(__int32 x) {
	std::string& wsi = _base->_wsi; 
	if(x != 0) {
		char psz[64];
		sprintf(psz, "%d", x);
		wsi = psz;
	}
	else { 
		wsi.resize(0);
	} 
}

void ABFundamentalTypesSerializer::wsi_getMember(unsigned __int32 x) {
	std::string& wsi = _base->_wsi; 
	if(x != 0) {
		char psz[64];
		sprintf(psz, "%u", x);
		wsi = psz;
	}
	else { 
		wsi.resize(0);
	} 
}

void ABFundamentalTypesSerializer::wsi_getMember(const __int16& x) {
	wsi_getMember((__int32)x);
}

void ABFundamentalTypesSerializer::wsi_getMember(const unsigned __int16& x) {
	wsi_getMember((unsigned __int32)x);
}

void ABFundamentalTypesSerializer::wsi_getMember(const long& x) {
	wsi_getMember((__int32)x);
}

void ABFundamentalTypesSerializer::wsi_getMember(const unsigned long& x) {
	wsi_getMember((unsigned __int32)x);
}

void EXP wsi_getMember(std::string &wsi, const __int64& x); 

void ABFundamentalTypesSerializer::wsi_getMember(const __int64& x) {
	::wsi_getMember(_base->_wsi, x);
}

void ABFundamentalTypesSerializer::wsi_getMember(const char& x) {
	std::string& wsi = _base->_wsi; 
	if(x != 0) {
		char psz[8];
		sprintf(psz, "%X", (((int)x)&0xff));
		wsi = psz;
	}
	else { 
		wsi.resize(0);
	} 
}

void ABFundamentalTypesSerializer::wsi_getMember(const signed char& x) {
	if(x != 0) {
		wsi_getMember((const char&) x);
	}
	else { 
		_base->_wsi.resize(0);
	} 
}

void ABFundamentalTypesSerializer::wsi_getMember(const unsigned char& x) {
	if(x != 0) {
		wsi_getMember((const char&) x);
	}
	else { 
		_base->_wsi.resize(0);
	} 
}



// put
void ABFundamentalTypesSerializer::wsi_putMember(char& what) {
	unsigned char x = what = 0;
	wsi_putMember(x);
	what |= x;
}

void ABFundamentalTypesSerializer::wsi_putMember(signed char& what) {
	unsigned char x = what = 0;
	wsi_putMember(x);
	what |= x;
}

void ABFundamentalTypesSerializer::wsi_putMember(unsigned char& what) {
	int x = 0;
	const substring_locator& str = *(_base->_pstr); 
	const char *begin = str.fix_quote(); 
	sscanf(std::string(begin, str._end - begin).c_str(), "%X", &x);
	what |= (unsigned char)(x & 0xff);
}

void ABFundamentalTypesSerializer::wsi_putMember(float& what) {
	what = (float)_base->_pstr->Atof();
}

void ABFundamentalTypesSerializer::wsi_putMember(double& what) {
	what = _base->_pstr->Atof(); 
}

void ABFundamentalTypesSerializer::wsi_putMember(__int16& what) {
	what = (__int16)_base->_pstr->Atoi(); 
}

void ABFundamentalTypesSerializer::wsi_putMember(__int32& what) {
	what = _base->_pstr->Atoi(); 
}

void ABFundamentalTypesSerializer::wsi_putMember(long& what) {
	what = _base->_pstr->Atol(); 
}

void ABFundamentalTypesSerializer::wsi_putMember(unsigned __int16& what) {
	what = (unsigned __int16)_base->_pstr->Atoi(); 
}

void ABFundamentalTypesSerializer::wsi_putMember(unsigned __int32& what) {
	what = (unsigned __int32)_base->_pstr->Atoi(); 
}

void ABFundamentalTypesSerializer::wsi_putMember(unsigned long& what) {
	what = (unsigned long)_base->_pstr->Atol(); 
}

void ABFundamentalTypesSerializer::wsi_putMember(__int64& what) {
	what = _base->_pstr->Atoi64(); 
}




void ABFundamentalTypesSerializer::wsi_eraseMember(std::string& m) {
	m.resize(0);
}
void ABFundamentalTypesSerializer::wsi_eraseMember(std::u8string& m) {
	m.resize(0);
}
void ABFundamentalTypesSerializer::wsi_eraseMember(std::payload_string& m) {
	m.resize(0);
}
void ABFundamentalTypesSerializer::wsi_eraseMember(substring_locator& m) {
	m.clear();
}



template<typename T>
inline void wsi_printValue(std::string& wsi, const char *frmt, T x) {
	//std::ostringstream ostr;
	//ostr << x;
	//wsi = ostr.str();
	char psz[128];
	int n = ::snprintf(psz, (size_t)sizeof(psz), frmt, x);
	if(n < 0) {
		n = 0;
	}
	wsi.resize(n);
	memcpy((PVOID)wsi.data(), psz, n);
} 

void xml_printDouble(std::string& wsi, double x) {
	int head = (int)x;
	int tail = (int)(std::abs(x - head) * 1000000.0 + 0.45);

	char psz[64];
	int n;
	if(tail) {
		n = ::sprintf(psz, "%d.%06d", head, tail);
		while(psz[n - 1] == '0') {
			--n;
		}
	}
	else {
		n = ::sprintf(psz, "%d", head);
	}
	if(n < 0) {
		n = 0;
	}

	wsi.resize(n);
	memcpy((PVOID)wsi.data(), psz, n);
}



void ABFundamentalTypesSerializer::wsi_getMember(const float& x) {
	std::string& wsi = _base->_wsi; 
	if(x == 0.0) {
		wsi.resize(0);
		return;
	}

	if(_base->_serializer->_human_readable_output) {
		xml_printDouble(wsi, x);
	}
	else {
		wsi_encodeFloat(wsi, x, _base->_serializer->_writer_ptr->Version());
	}
}

void ABFundamentalTypesSerializer::wsi_getMember(const double& x) {
	std::string& wsi = _base->_wsi; 
	if(x == 0.0) {
		wsi.resize(0);
		return;
	}

	if(_base->_serializer->_human_readable_output) {
		xml_printDouble(wsi, x);
	}
	else {
		wsi_encodeDouble(wsi, x, _base->_serializer->_writer_ptr->Version());
	}
}





// strings

void wsi_encodeHexString(std::string& wsi, char *what, size_t S, bool truncate = true) {
	char *last = what + S;
	if(truncate) {
		while(last > what && *(last - 1) == 0) {
			--last;
		}
	}
	wsi.resize((size_t)(last - what) * 2);
	PCHAR buf = (PCHAR)wsi.data();
	while(what < last) {
		int x = (int)(*what++) & 0xff;
		*buf++ = DEC2HEX[(x >> 4) & 0x0f];
		*buf++ = DEC2HEX[x & 0x0f];
	}
}

void wsi_decodeHexString(char *what, const substring_locator& str, size_t S) {
	size_t nsize = str.size();
	if(nsize) {
		switch(*str._begin) {
		case '\'': case '\"': WSIPutString(str._begin, nsize, what, S); return;
		}
	}

	const size_t width_inp = 2;

	const char *p_last_inp = str._begin + nsize;
	char *p_last_out = what + S;

	const char *p_next_inp = str._begin;
	char *p_next_out = what;

	bool err = false;
	while(!err && p_next_inp < p_last_inp && p_next_out < p_last_out) {

		CHAR a = HEX2DEC(*p_next_inp++, err);
		CHAR b = HEX2DEC(*p_next_inp++, err);

		*p_next_out++ = (a << 4) | b;
	}

	if(err) {
		WSIPutString(str._begin, nsize, what, S);
	}
}

template <typename T>
void wsi_encodeHexString(std::string& wsi, T* what, size_t S, bool truncate = true) {
	wsi_encodeHexString(wsi, (char*)what, S, truncate);
}

template <typename T>
void wsi_decodeHexString(T *what, const substring_locator& str, size_t S) {
	size_t nsize = str.size();
	if(nsize) {
		switch(*str._begin) {
		case '\'': case '\"': WSIPutString(str._begin, nsize, what, S); return;
		}
	}

	wsi_decodeHexString((char*)what, str, S * sizeof(T));
}



void ABFundamentalTypesSerializer::wsi_getMember(const std::string& x) {
	std::string& wsi = _base->_wsi; 
	if(x.size()) {
		wsi_getString(x.data(), x.size()); 
	}
	else { 
		wsi.resize(0);
	} 
}


void ABFundamentalTypesSerializer::wsi_getMember(const std::payload_string& x) {
	std::string& wsi = _base->_wsi; 
	if(x.size()) {
		wsi.resize(x.size()); 
		memcpy((void*)wsi.data(), x.data(), x.size()); 
	}
	else { 
		wsi.resize(0);
	} 
}


void ABFundamentalTypesSerializer::wsi_getMember(const std::u8string& x) {
	std::string& wsi = _base->_wsi;
	if(x.size()) {
		wsi_encodeHexString(wsi, x.data(), x.size(), false);
	}
	else {
		wsi.resize(0);
	}
}


void ABFundamentalTypesSerializer::wsi_getMember(const char* x) {
	wsi_getMember(std::string(x));
} 

void ABFundamentalTypesSerializer::wsi_getMember(const substring_locator& x) {
	wsi_getString(x._begin, x.size());
}



void ABFundamentalTypesSerializer::wsi_getString(const char *x, size_t S) { 
	std::string& wsi = _base->_wsi; 
	if(*x) { 
		const char *last = x + S; 
		const char *y = last; 
		while(y != x && *(y - 1) == 0x20) { 
			--y; 
		} 
		S = (size_t)(y - x); 
		switch(_base->_serializer->_encoding) {
		case 1: 
			wsi.resize(S * 3);
			if(S) {
				//BOOL bAlert;
				wsi.resize(WSIEncode(x, S, (PCHAR)wsi.c_str(), NULL/*&bAlert*/));
			}
			break; 
		case 2: 
			wsi.resize(S * 5);
			if(S) {
				//BOOL bAlert;
				wsi.resize(XMLEncode(x, S, (PCHAR)wsi.c_str(), NULL/*&bAlert)*/));
			}
			break; 
		case 4: 
			wsi.resize(S * 5);
			if(S) {
				//BOOL bAlert;
				wsi.resize(JSONEncode(x, S, (PCHAR)wsi.c_str(), NULL/*&bAlert)*/));
			}
			break; 
		default: 
			wsi = std::string(x, S);
			break; 
		} 
	} 
	else { 
		wsi.resize(0);
	} 
} 


void ABFundamentalTypesSerializer::wsi_putString(char *what, size_t S) { 
	const substring_locator& str = *(_base->_pstr); 
	WSIDecodeEx(str._begin, str.size(), what, S, _base->_serializer->_encoding);
} 

void ABFundamentalTypesSerializer::wsi_putMember(std::string& what) { 
	const substring_locator& str = *(_base->_pstr); 
	what.resize(str.size());
	what.resize(WSIDecodeEx(str._begin, str.size(), (PCHAR)(what.c_str()), what.size(), _base->_serializer->_encoding));
} 

void ABFundamentalTypesSerializer::wsi_putMember(std::payload_string& what) { 
	const substring_locator& str = *(_base->_pstr); 
	what.resize(str.size());
	what.resize(WSIDecodeEx(str._begin, str.size(), (PCHAR)(what.c_str()), what.size(), 0/*no encoding*/));
} 

void ABFundamentalTypesSerializer::wsi_putMember(std::u8string& what) {
	const substring_locator& str = *(_base->_pstr);
	what.resize(str.size() / 2);
	wsi_decodeHexString((char*)what.data(), str, what.size());
}

void ABFundamentalTypesSerializer::wsi_putMember(substring_locator& what) {
	what = *(_base->_pstr);
}




//	character array versions.

void ABFundamentalTypesSerializer::wsi_getHexString(const unsigned char *what, size_t S) { 
	wsi_encodeHexString(_base->_wsi, what, S); 
} 

void ABFundamentalTypesSerializer::wsi_getHexString(const signed char *what, size_t S) { 
	wsi_encodeHexString(_base->_wsi, what, S); 
} 

void ABFundamentalTypesSerializer::wsi_getHexString(const wchar_t *what, size_t S) { 
	wsi_encodeHexString(_base->_wsi, what, S); 
} 



void ABFundamentalTypesSerializer::wsi_putHexString(unsigned char *what, size_t S) { 
	wsi_decodeHexString((char*)what, *(_base->_pstr), S);
} 

void ABFundamentalTypesSerializer::wsi_putHexString(signed char *what, size_t S) { 
	wsi_decodeHexString((char*)what, *(_base->_pstr), S);
} 

void ABFundamentalTypesSerializer::wsi_putHexString(wchar_t *what, size_t S) { 
	wsi_decodeHexString(what, *(_base->_pstr), S); 
} 






void CWsiSerializerBase::LateBinding() {
	if(_is_late_binding) {
		_is_late_binding = false;
		Init();
	}
}

// resets iterator of an array of fundamental type.
void CWsiSerializerBase::reset_all_fundamental_types(__w64 int outer_offset) {
	LateBinding();
	converter_vector::iterator iter = _pvector->begin();
	while(iter != _pvector->end()) {
		if((*iter)->_serializer != this) {
			(*iter)->_serializer->_outer_offset = outer_offset;
			(*iter)->_serializer->_encoding = _encoding;
		}
		(*iter)->_functor->action(4);
		(*iter)->_functor->action(0);
		++iter;
	}
}

// erases all memebrs of current binding.
void CWsiSerializerBase::current_binding_erase_all(__w64 int outer_offset) {
	LateBinding();
	converter_vector::iterator iter = _pvector->begin();
	while(iter != _pvector->end()) {
		if((*iter)->_serializer != this) {
			(*iter)->_serializer->_outer_offset = outer_offset;
			(*iter)->_serializer->_encoding = _encoding;
		}
		(*iter)->_functor->action(4);
		(*iter)->_functor->action(3);
		++iter;
	}
}

void CWsiSerializerBase::set_outer_ptr(void *obj) {
	_outer_offset = (char*)obj - (char*)_inner_ptr;
}



converter_tuple& CWsiSerializerBase::add_converter_tuple(const char* const wsi_id, bool flat_member) {
	if(!_pmap) {
		_pmap = new converter_map();
		_pvector = new converter_vector();
		_pvector->reserve(16);

	}
	substring_locator s_wsi_id = wsi_id; 
	_pmap->insert(converter_map::value_type(s_wsi_id, converter_tuple()));
	converter_tuple& t = (*_pmap)[s_wsi_id];
	if(flat_member && _is_composite) {
		_pvector->insert(_pvector->begin(), 1, &t);
	}
	else {
		_pvector->push_back(&t);
	}

	return t;
}




const char * CWsiSerializerBase::find_member_id_byoffset(__int64 member) {
	const char *id = 0;
	lock();
	converter_vector::iterator iter = _pvector->begin();
	while(iter != _pvector->end()) {
		bool found = false;
		if((*iter)->_serializer == this) {
			found = (*iter)->_functor->is(member);
		}
		else {
			(*iter)->_functor->action(4);
			(*iter)->_functor->action(0);
			found = (*iter)->_functor->is(member);
		}
		if(found) {
			id = (*iter)->_xwsi_id._begin;
			break;
		}
		++iter;
	}
	unlock();

	return id;
}




void CWsiSerializerBase::get_string_image(size_t idx, bool can_be_empty) /*returns string image*/ {
	LateBinding();

	if(!_writer_ptr) {
		return;
	}

	long orig_pos = _writer_ptr->GetPos();

	_writer_ptr->Append('\n');
	if(idx < 2) {
		_writer_ptr->Append('\n');
	}
	_writer_ptr->BeginObject(_wsi_id, (int)idx, _is_composite);

	int count = 0;
	int obj_count = 0;
	converter_vector::iterator iter = _pvector->begin();
	while(iter != _pvector->end()) {
//		ARRAY ISSUE: array of objects.
//		this will force iteration over array's elements.
		bool iterate = true;
		int elem_count = 0;
		int elem_last_nemp = 0; // last not empty element.

//		Late binding of current Writer to member object.
//		Only composites can contain other objects.
		if(_is_composite) {
			if((*iter)->_serializer != this) {
				(*iter)->_serializer->_writer_ptr = _writer_ptr;
				(*iter)->_serializer->_human_readable_output = _human_readable_output;
				(*iter)->_serializer->_encoding = _encoding;
			}
		} 

		converter_tuple *ptr = (*iter); 
		std::string ptr_id(ptr->_xwsi_id._begin, ptr->_xwsi_id.size()); 

		while(iterate) {
			long pos = _writer_ptr->GetPos();

			ptr->_wsi.resize(0);
			iterate = ptr->_functor->action(2/*action get*/);
//			ARRAY ISSUE: array of objects.
//			Note: in case of array: _functor must be pointing to CWsiArraySerializer::call_functor().
//			see call_functor() for details.
//			2013-03-21: The functor for arrays returns true when the processed element is still in the valid range, 
//			it returns false when action is called upon arrays's index outside of the valid range. 

			if(iterate) { 
				++elem_count; // non zero only for containers, even with size 1. 
			} 
			else 
			if(elem_count) { 
				break; // end of range for containers. 
			} 

			if(ptr->_serializer == this) { // flat serializer.
				if(ptr->_wsi.size() || _writer_ptr->PutAll(iterate)) {
					_writer_ptr->AddAttribute(
						ptr_id,
						ptr->_wsi,
						elem_count,
						(++elem_last_nemp < elem_count));
					elem_last_nemp = elem_count;
				}
			}

			if(pos != _writer_ptr->GetPos()) {
				++count;
				if(ptr->_serializer != this) {
					++obj_count;
				}
			}

		}

		if(_is_composite) {
			if(ptr->_serializer != this) {
				ptr->_serializer->_writer_ptr = 0;
				ptr->_serializer->_human_readable_output = 1;
				ptr->_serializer->_encoding = 3;
			}
		}

		_writer_ptr->EndIterator(ptr_id, elem_count);

		++iter;
	}

	if(count > 0 || !can_be_empty) {
		_writer_ptr->EndObject(_wsi_id, (int)idx, _is_composite, obj_count);
	}
	else {
		_writer_ptr->SetPos(orig_pos);
	}
}




bool CWsiSerializerBase::accept_a_member(const paired_captures& capture) /*accepts string image of a single field*/ {
	bool rc = false;
	converter_map::iterator iter = _pmap->find(capture.first);
	if(iter != _pmap->end()) {
		converter_tuple &t = (*iter).second;

		t._pstr = &capture.second; 

//		ARRAY ISSUE:
//		TO DO: place here rebind to index if index is not zero. DONE. 
		if(capture.index > 0) {
			t._functor->action(-capture.index/*action: rebind to index*/);
		} 

		if(t._pstr->_begin) { 
			t._functor->action(1/*action put*/);
		} 
		else { 
			t._functor->action(5/*action increment*/);
		} 
		rc = true;
	}
	return rc;
}


void CWsiSerializerBase::accept_flat_object(const wsi_object& aInner) {
	LateBinding();

	if(!_pmap) {
		return;
	}

//	ARRAY ISSUE:
//	MoveNext() increments current index.
//	MoveNext() rebinds all _map entries to current index.
//	MoveNext() does nothing for non array entities.

	if(!MoveNext(aInner._header.index)) {
		aInner.log("Index out of range");
	}

	size_t x = 0;
	while(x < aInner._vecCaptures.size()) {
		accept_a_member(aInner._vecCaptures[x++]);
	}
}

void CWsiSerializerBase::accept_composite_object(const wsi_object& aComposite, std::vector<wsi_object>& ostack, size_t& pos) {
//	internal fields are defined same way as in a flat object.
	accept_flat_object(aComposite);
	if(!_pmap) {
		return;
	}

	size_t cnt = aComposite._nInnerObjects;
	size_t siz = ostack.size();

	for(size_t n = 0; n < cnt && pos < siz; ++n) {
		wsi_object& aInner = ostack[pos++];

		converter_map::iterator iter = _pmap->find(aInner._header.first);
		if(iter != _pmap->end()) {
			if((*iter).second._serializer == this) {
//				an internal field of a composite may also be serilalized as an object.
				if(aInner._vecCaptures.size() > 0) {
					accept_a_member(aInner._vecCaptures[0]);
				}
				else { //FEB/08/2013 - XML
					accept_a_member(aInner._header); 
				} 
				continue;
			}

			CWsiSerializerBase *serializer = (*iter).second._serializer;
			if(serializer->_is_composite) {
				serializer->accept_composite_object(aInner, ostack, pos);
			}
			else 
			if(aInner._nInnerObjects > 0) {
				serializer->accept_composite_object(aInner, ostack, pos);
			}
			else {
				serializer->accept_flat_object(aInner);
			}
		}
		else {
			aInner.log("Unknown ");
//			Object does not belong to the context of this serializer.
//			This object may have sub-objects on the stack.
//			Reduce the stack.
			if(aInner._nInnerObjects) {
				remove_composite_from_stack(aInner, ostack, pos);
			}
		}
	}
}

void CWsiSerializerBase::remove_composite_from_stack(const wsi_object& aComposite, std::vector<wsi_object>& ostack, size_t& pos) {
	for(int n = 0; n < aComposite._nInnerObjects && pos < ostack.size(); ++n) {
		const wsi_object& aInner = ostack[pos++];
		if(aInner._nInnerObjects) {
			remove_composite_from_stack(aInner, ostack, pos);
		}
	}
}


//	Main call_functor.
//	It works for non array member types.
bool CWsiSerializerBase::call_functor(int action) {
	switch(action) {
		case 0/*reset*/:
			reset_all_fundamental_types(_outer_offset);
			break;
		case 2/*get*/:
			get_string_image();
			break;
		case 3/*erase*/:
			current_binding_erase_all(_outer_offset);
			break;
	}
	return false;
}



//	Main public functions.
void CWsiSerializerBase::operator()(const IWSIObject& iwsiobj, LONG_PTR rpc_handle, IWSIMarshaller *current_host, BOOL dolock, BOOL use_clones) {
	const wsi_object& cObj = *(reinterpret_cast<wsi_object*>(iwsiobj._obj));
	if(!cObj.empty()) {
		if(dolock) { 
			lock(); 
		}

		_encoding = current_host->Encoding();

		current_binding_erase_all(_outer_offset);
		accept_flat_object(cObj);

		if(use_clones) { 
			dolock = onNewObjectClone(_wsi_id, ((char*)_inner_ptr) + _outer_offset, rpc_handle, current_host, dolock);
		} 
		else { 
			onNewObject(_wsi_id, ((char*)_inner_ptr) + _outer_offset, rpc_handle, current_host); 
		} 

		_encoding = 3;

		if(dolock) { 
			unlock(); 
		} 
	}
}

void CWsiSerializerBase::operator()(const IWSIStackObject& iwsistack, LONG_PTR rpc_handle, IWSIMarshaller *current_host, BOOL dolock, BOOL use_clones) {
	std::vector<wsi_object>& ostack = *(reinterpret_cast<std::vector<wsi_object>*>(iwsistack._stobj));
	if(ostack.size()) {
		size_t pos = 1;
		if(dolock) { 
			lock(); 
		}

		_encoding = current_host->Encoding();

		current_binding_erase_all(_outer_offset);
		accept_composite_object(ostack[0], ostack, pos);

		if(use_clones) { 
			dolock = onNewObjectClone(_wsi_id, ((char*)_inner_ptr) + _outer_offset, rpc_handle, current_host, dolock);
		} 
		else { 
			onNewObject(_wsi_id, ((char*)_inner_ptr) + _outer_offset, rpc_handle, current_host);
		} 

		_encoding = 3;

		if(dolock) { 
			unlock(); 
		} 
		ostack.resize(0);
	}
}


void CWsiSerializerBase::operator()(ISTREAMWriter& writer, const void *obj, std::string& wsi) /*returns string image*/ {
	_outer_offset = (char*)obj - (char*)_inner_ptr;
	this->operator()(writer, wsi);
	_outer_offset = 0;
}


void CWsiSerializerBase::operator()(ISTREAMWriter& writer, std::string& wsi) /*returns string image*/ {
	_writer_ptr = &writer;
	_writer_ptr->lock();

	_encoding = writer.Encoding();
	_human_readable_output = writer.IsHumanReadableFormat()? 1: 0;

	_writer_ptr->BeginFile(_wsi_id);

	reset_all_fundamental_types(_outer_offset);
	get_string_image(0/*idx*/, false/*can_be_empty=false*/);

	_writer_ptr->EndFile(_wsi_id);
	_writer_ptr->ToString(wsi);

	_encoding = 3;
	_human_readable_output = 1;

	_writer_ptr->unlock();
}


std::string CWsiSerializerBase::operator()(ISTREAMWriter& writer) /*returns string image*/ {
	std::string wsi;
	this->operator()(writer, wsi);
	return wsi;
}



CWsiSerializerBase::converter_vector Shared_place_holder; // will contain no items.

//	Construction/Destruction.
CWsiSerializerBase::CWsiSerializerBase(void * inner, const std::string& wsi_id, bool late_binding) {
	_pmap = 0;
	_pvector = 0;
	_writer_ptr = 0;
	_outer_offset = 0;
	_encoding = 3;
	_human_readable_output = 1;
	_is_composite = false;
	_is_late_binding = late_binding;
	_inner_ptr = inner;
	_wsi_id = wsi_id;

//	Empty object issue:
//	Empty object contains no members, thus _pvector will not be initialized ever.
//	So, initially do assign _pvector to a static empty vector.
	_pvector = &Shared_place_holder;
//	_pmap will not be used unless _pvector contains something.
}

CWsiSerializerBase::~CWsiSerializerBase() {
	if(_pmap) {
		delete _pmap;
		_pmap = 0;
	}
	if(_pvector) {
		if(_pvector != &Shared_place_holder) {
			delete _pvector;
		}
		_pvector = 0;
	}
}



bool CWsiSerializerBase::MoveNext(size_t index/*if index > 0, then jump to the index*/) {
//	MoveNext() does nothing for non array members.
	return index == 0;
}


bool CWsiSerializerBase::is(__int64 member) {
	return member == (__int64)_inner_ptr;
}



bool IWsiArraySerializer::MyCallFunctor(int action, CWsiSerializerBase *This) {
	bool rc = false;
	switch(action) {
		case 0: /*reset*/
			_next = 1;
			This->set_inner_ptr(GetElement(0));
			This->reset_all_fundamental_types(This->_outer_offset);
			break;
		case 2: /*get*/
			if(_next <= _last) { 
				This->get_string_image(_next, !This->_writer_ptr->PutAll(true)); 
				if(!MyMoveNext(This)) { 
					++_next; 
				} 
				rc = true; 
			} 
			break;
		case 3: /*erase*/
			for(_next = 0; _next < _last; ++_next) {
				This->set_inner_ptr(GetElement(_next));
				This->current_binding_erase_all(This->_outer_offset + ((char*)This->_inner_ptr - (char*)GetElement(0)));
			}
			_next = 0;
			break;
	}

	return rc;
}

bool IWsiArraySerializer::MyMoveNext(CWsiSerializerBase *This, size_t index) { // it is used mainly for accept object. Erase is done already when called from accept object.
	bool rc = false;
	if(index > 0) {
		if(--index < _last) {
			_next = index;
		}
	}
	if(_next < _last) {
		This->set_inner_ptr(GetElement(_next++));
		This->reset_all_fundamental_types(This->_outer_offset + ((char*)This->_inner_ptr - (char*)GetElement(0)));
		rc = true;
	}

	return rc;
}

void * IWsiArraySerializer::GetElement(size_t idx) {
	return 0;
}



IWsiVectorSerializer::IWsiVectorSerializer(const std::string& wsi_id, size_t last):
	CWsiSerializerBase(NULL, wsi_id, true),
	_next(0),
	_last(last) {
}

bool IWsiVectorSerializer::MyCallFunctor(int action, CWsiSerializerBase *This) {
	bool rc = false;
	switch(action) {
		case 0: /*reset*/
			Remap();
			This->_outer_offset = 0;
			if(Size() > 0) {
				_next = 1;
				This->reset_all_fundamental_types((__w64 int)This->set_inner_ptr(GetElement(0)));
			}
			else {
				_next = 0;
				This->set_inner_ptr(0);
			}
			break;
		case 2: /*get*/
			if(_next <= Size() && This->get_inner_ptr()) {
				This->get_string_image(_next, !This->_writer_ptr->PutAll(true));
				if(_next < Size()) {
					This->reset_all_fundamental_types((__w64 int)This->set_inner_ptr(GetElement(_next)));
				}
				++_next; 
				rc = true;
			}
			break;
		case 3: /*erase*/
			Remap();
			This->_outer_offset = 0;
			Resize(0);
			This->set_inner_ptr(0);

			_next = 0;

			This->LateBinding();
			break;
	}

	return rc;
}

bool IWsiVectorSerializer::MyMoveNext(CWsiSerializerBase *This, size_t index) { // it is used exclusively for accept object. Erase is done already.
	bool rc = false;
	if(index > 0) {
		if(--index < _last) {
			_next = index;
		}
	}
	if(_next < _last) {
		if(Size() < (++_next)) {
			size_t last_check_point = Size();
			Resize(_next);
			while(last_check_point < _next) {
				This->current_binding_erase_all((__w64 int)This->set_inner_ptr(GetElement(last_check_point++)));
			}
		}
		else {
			This->reset_all_fundamental_types((__w64 int)This->set_inner_ptr(GetElement(_next - 1)));
		}
		rc = true;
	}

	return rc;
}

void * IWsiVectorSerializer::GetElement(size_t idx) {
	return 0;
}
void IWsiVectorSerializer::Remap() {
}
void IWsiVectorSerializer::Resize(size_t new_size) {
}
size_t IWsiVectorSerializer::Size() {
	return _last;
}



void EXP add_flat_member(const char* const wsi_id, CWsiSerializerBase *base, function_type2 *functor2) {
	converter_tuple& t = base->add_converter_tuple(wsi_id, true);

	functor2->s.SetBase(&t);

	t._xwsi_id = wsi_id;
	t._serializer = base;
	t._functor = functor2;
}


void EXP add_object_member(const char* const wsi_id, CWsiSerializerBase *base, CWsiSerializerBase *serializer) {
	converter_tuple& t = base->add_converter_tuple(wsi_id);

	t._xwsi_id = wsi_id;
	t._functor = new ABSerializerBaseFunctor(t);
	t._serializer = serializer;

	base->_is_composite = true;
}




const CWsiCollectionImpl& CWsiCollectionImpl::operator=(const CWsiCollectionImpl&) {
	return *this;
}

const IWsiCollection::object_serializers& CWsiCollectionImpl::Get() const {
	return *_pmap;
}

void CWsiCollectionImpl::insert(IWsiSerializerBase* serializer) {
	bool rc = (*_pmap).insert(object_serializers::value_type(serializer->_wsi_id, serializer)).second;
	assert(rc);
}

IWsiSerializerBase* CWsiCollectionImpl::find(const std::string& id) {
	IWsiSerializerBase *t = 0;
	object_serializers::iterator iter = (*_pmap).	find(id);
	if(iter != (*_pmap).end()) {
		t = (*iter).second;
	}
	return t;
}

void CWsiCollectionImpl::merge(const IWsiCollection& coll) {
	const object_serializers& src = coll.Get();
	object_serializers::const_iterator iter = src.begin();
	while(iter != src.end()) {
		IWsiSerializerBase *t = (*iter).second->Create();
		if(t) {
			insert(t);
		}
		++iter;
	}
}

CWsiCollectionImpl::CWsiCollectionImpl() {
	_pmap = new object_serializers();
}

CWsiCollectionImpl::~CWsiCollectionImpl() {
	object_serializers::const_iterator iter = (*_pmap).begin();
	while(iter != (*_pmap).end()) {
		delete (*iter).second;
		++iter;
	}
}




void EXP WSIXConfigure(IWsiSerializerBase& obj, const std::string& config, IXFunction<IWSIMarshaller, void> *pExtra_callbacks) {
	if(config.size()) {
		IWSIMarshaller& marshaller = *CreateIWSIMarshaller();

		obj.register_with(marshaller);

		if(pExtra_callbacks) {
			(*pExtra_callbacks)(marshaller);
		}

		marshaller(CWSIParser(), config);

		DestroyIWSIMarshaller(&marshaller);
	}
}

void EXP WSIXSerialize(std::string& config, IWsiSerializerBase& obj, bool human_readable, IWsiSerializerBase **extra, const int extra_cnt) {
	ISTREAMWriter& writer = *CWSIParser().create_writer();
	writer.SetHumanReadableFormat(human_readable);

	obj(writer, obj.get_inner_ptr(), config);

	if(extra_cnt) { 
		std::string wsi;
		for(int x = 0; x < extra_cnt; ++x) { 
			(*(extra[x]))(writer, (extra[x])->get_inner_ptr(), wsi); 
			config +=  wsi; 
		} 
	} 

	delete &writer;
}


void EXP XMLXConfigure(IWsiSerializerBase& obj, const std::string& config) {
	if(config.size()) {
		IWSIMarshaller& marshaller = *CreateIWSIMarshaller();

		obj.register_with(marshaller);

		marshaller(CXMLParser(), config);

		DestroyIWSIMarshaller(&marshaller);
	}
}




/*
struct CWSIClassFactoryTest {
	char char_array[2];
	unsigned char uschar_array[2];
	signed char schar_array[2];
	wchar_t wchar_array[2];
};

BEGIN_WSI_SERIALIZATION_OBJECT(CWSIClassFactoryTest)
CONTAINS_FLAT_MEMBER(char_array, CHARARRAY)
CONTAINS_FLAT_MEMBER(uschar_array, USCHARARRAY)
CONTAINS_FLAT_MEMBER(schar_array, SCHARARRAY)
CONTAINS_FLAT_MEMBER(wchar_array, WCHARARRAY)
END_WSI_SERIALIZATION_OBJECT()
*/

