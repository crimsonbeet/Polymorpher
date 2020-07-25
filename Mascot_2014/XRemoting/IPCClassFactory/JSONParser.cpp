// Created by Alex Nikol Bourakov (ANB) around April/07


#include "stdafx_classfactory.h"
#include "JSONParser.h"
#include "WSIObject.h"


void yyjsonerror(
				std::string::const_iterator& _begin,
				std::string::const_iterator& _end,
				IWSIMaterializer&,
				char const *err);
int yyjsonparse(
				std::string::const_iterator& _begin,
				std::string::const_iterator& _end,
				IWSIMaterializer& _context);

void CJSONParser::parse(const std::string& wsi, IWSIMaterializer& mat) const {
	std::string::const_iterator from = wsi.begin();
	std::string::const_iterator to = wsi.end();
	yyjsonparse(from, to, mat);
}
void CJSONParser::parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer& mat) const {
	yyjsonparse(from, to, mat);
}

ISTREAMWriter * CJSONParser::create_writer() const {
	return new CJSONWriter();
}
int CJSONParser::Encoding() const {
	return 4;
}


void yylogerror(std::string::const_iterator& _begin, std::string::const_iterator& _end, IWSIMaterializer&, char const *err, const std::string& prefix); 


#if defined(_WIN32_WINDOWS)
#pragma warning(disable: 4065)
#endif

#include "JSONParser.y.tab.h"


void yyjsonerror(std::string::const_iterator& _begin, std::string::const_iterator& _end, IWSIMaterializer& mat, char const *err) { 
	yylogerror(_begin, _end, mat, err, "json-parser"); 
}


int yyjson_value_tokens[10] = { Number, SignedNumber, FloatNumber, NumberExp, FloatNumberExp, true_token, false_token, null_token, Word, GeneralString };


int yyjsonlex(
			yyjson_token_type *yylval,
			std::string::const_iterator& _begin,
			std::string::const_iterator& _end) {

	while(_begin != _end && *(_begin) < 0x21) {
		++_begin;
	}

	if(_begin == _end) {
		return 0;
	}

	yylval->clear();

//	A current position in the array of value tokens.
//	Position in the array specifies type of value token.
//	A type of token can be promoted forward only.
	int t = -1;

//	Check non value tokens first.
	switch(*(_begin)) {
		case ':':
		case '[':
		case ']':
		case ',':
		case '{':
		case '}':
			return *(_begin++);
	}

	int done = 0;

//	Check value tokens.
	yylval->_substr._begin = &_begin[0];

	switch(*(_begin)) {
		case '\"': // (Marker ") is a begin of GeneralString token.
			t = 9;
//			Assume it is a GeneralString.
//			Content between markers will be returned. 
			if(++_begin != _end) { 
				yylval->_substr._begin = &_begin[0];
				while(_begin != _end) {
					if(*(_begin) == '\"') {
						break;
					}
					if(*(_begin) == '\\') {
						if((_begin + 1) != _end) {
							++_begin; // Skip marker.
						}
					}
					++_begin; 
				}
				if(_begin != _end) { 
					yylval->_substr._end = &_begin[0];
					++_begin;
				} 
				else { 
					yylval->_substr._end = (&(_begin-1)[0]) + 1;
				} 
			} 
			done = 2;
			return yyjson_value_tokens[t]; 
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
		}
		else
		switch(xchar) {
			case ':':
			case '[':
			case ']':
			case ',':
			case '{':
			case '}':
				done = 1;
			break;
			default:
				if(t == 9) {
				}
				else
				switch(xchar) {
					case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
						if(t < 0) {
							t = 0; // Promote to Number
						}
					break;
					case '-':
					case '+':
						t = 9;
					break;
					case '.':
						if(t < 2) {
							t = 2; // Promote from Number to FloatNumber
						}
						else {
							t = 9;
						}
					break;
					case 'e':
					case 'E':
						if(t < 2) {
							t = 3; // Promote from Number to NumberExp
							done = 1;
						}
						else 
						if(t < 3) {
							t = 4; // Promote from FloatNumber to FloatNumberExp
							done = 1;
						}
						else 
						if(t < 8) { 
							t = 8; 
						} 
					break;
					default:
						if(xchar == '_' || isalpha(xchar)) {
							if(t < 8) { 
								t = 8; 
							} 
						}
						else {
							t = 9;
						}
					break;
				}
				++_begin;
			break;
		}
	}

	yylval->_substr._end = _begin != _end? &_begin[0]: (&(_begin-1)[0]) + 1;
	
	return yyjson_value_tokens[t];
}

