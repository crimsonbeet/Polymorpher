// Created by Alex Nikol Bourakov (ANB) around April/07


#include "stdafx_classfactory.h"
#include "WSIParser.h"
#include "WSIObject.h"


void CRawParser::parse(const std::string& wsi, IWSIMaterializer& mat) const { 
	mat.process_raw_string(SIPCRawString(wsi)); 
}
void CRawParser::parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer& mat) const {
	const std::string wsi(from, to);
	mat.process_raw_string(SIPCRawString(wsi));
}
ISTREAMWriter * CRawParser::create_writer() const {
	return new CRawWriter();
}






void yywsierror(
				std::string::const_iterator& _begin,
				std::string::const_iterator& _end,
				IWSIMaterializer&,
				char const *err);
int yywsiparse(
				std::string::const_iterator& _begin,
				std::string::const_iterator& _end,
				IWSIMaterializer& _context);

void CWSIParser::parse(const std::string& wsi, IWSIMaterializer& mat) const {
	std::string::const_iterator from = wsi.begin();
	std::string::const_iterator to = wsi.end();
	yywsiparse(from, to, mat);
}
void CWSIParser::parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer& mat) const {
	yywsiparse(from, to, mat);
}

ISTREAMWriter * CWSIParser::create_writer() const {
	return new CWSIWriter();
}
int CWSIParser::Encoding() const {
	return 1;
}


ISTREAMWriter * CCRFParser::create_writer() const {
	return new CCRFWriter();
}


void yylogerror(std::string::const_iterator& _begin, std::string::const_iterator& _end, IWSIMaterializer&, char const *err, const std::string& prefix) { 
	size_t nsize = std::min((size_t)64, (size_t)(_end - _begin)); 
	std::string msg((nsize? &_begin[0]: "EOF"), nsize? nsize: 3);

	std::ostringstream ostr;
	ostr << std::endl << prefix << ':' << ' ' << err << ' ' << "before" << std::endl << ' ' << msg;
	IPCLog(ostr.str()); 
} 


#if defined(_WIN32_WINDOWS)
#pragma warning(disable: 4065)
#endif

#include "WSIParser.y.tab.h"


void yywsierror(std::string::const_iterator& _begin, std::string::const_iterator& _end, IWSIMaterializer& mat, char const *err) { 
	yylogerror(_begin, _end, mat, err, "wsi-parser"); 
}


// Notes for ObjArrayHeader. 
// The parsing assumes that in the definition of the object 
// the Object's Id matches memeber's Id, and the member is an array, i.e.

// =H[=P[0.000 0.000 0.000] =P[0.000 0.000 0.000]] matches 
// ><$H ><P P={0.000 0.000 0.000} ><P P={0.000 0.000 0.000} $



int yywsi_value_tokens[5] = { Number, SignedNumber, FloatNumber, Name, GeneralString };


int yywsilex(
			yywsi_token_type *yylval,
			std::string::const_iterator& _begin,
			std::string::const_iterator& _end) {

	while(_begin != _end && *(_begin) < 0x21) {
		++_begin;
	}

	if(_begin == _end) {
		return 0;
	}

	yylval->clear();

	char aToken = *(_begin);

	while(aToken == '/' && ((_begin + 1) != _end) && _begin[1] == '*') { // comment
		_begin += 2;
		while(_begin != _end) {
			if(*(_begin++) == '*') {
				if(_begin != _end && *(_begin) == '/') {
					while(++_begin != _end && *(_begin) < 0x21);
					break;
				}
			}
		}
		if(_begin != _end) {
			aToken = *(_begin);
		}
		else {
			return 0;
		}
	}

//	A current position in the array of value tokens.
//	Position in the array specifies type of value token.
//	A type of token can be promoted forward only.
	int t = -1;

//	Check non value tokens first.
	switch(aToken) {
		case ':':
		case '$':
		case '@':
		case '=':
		case '{':
		case '}':
		case '[':
		case ']':
			return *(_begin++);
		case '>':
			if(((_begin + 1) < _end) && _begin[1] == '<') { // ><
				_begin += 2;
				return Preambula;
			}
//			At this point it can only be the GeneralString token.
			t = 4;
		break;
	}

	int done = 0;

//	Check value tokens.
	yylval->_substr._begin = &_begin[0];

	switch(aToken) {
		case '\'': // (Marker ') is a begin of GeneralString token.
			t = 4;
//			Assume it is a GeneralString.
			for(++_begin; _begin != _end; ++_begin) {
				if(*(_begin) == '\'') {
					if((_begin + 1) == _end || _begin[1] != '\'') {
						break;
					}
					++_begin; // Skip marker.
				}
			}
			if(_begin != _end) { 
				++_begin;
			} 
			done = 1;
		break;

		case '-':
		case '+':
			t = 1; // Assume it is a signed number.
			++_begin;
		break;

	}

	while(!done && _begin != _end) {
		char xchar = *(_begin);
		if(xchar < 0x21) {
			done = 1;
			break;
		}
		else
		switch(xchar) {
			case '@':
			case ':':
			case '=':
			case '{':
			case '}':
			case '[':
			case ']':
				done = 1;
			break;
			default:
				if(t == 4) {
				}
				else
				switch(xchar) {
					case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
						if(t < 0) {
							t = 0; // Promote to Number
						}
					break;
					case '.':
						if(t < 2) {
							t = 2; // Promote from Number to FloatNumber
						}
						else {
							t = 4;
						}
					break;
					default:
						if(xchar == '_' || isalpha(xchar)) {
							if(t != 3) { // Only if it is not Name yet.
								if(t < 1) {
									t = 3; // Promote from Number to Name
								}
								else {
									t = 4;
								}
							}
						}
						else {
							t = 4;
						}
					break;
				}
				++_begin;
			break;
		}
	}

	yylval->_substr._end = _begin != _end? &_begin[0]: (&(_begin-1)[0]) + 1;
	return
		yywsi_value_tokens[t];
}

