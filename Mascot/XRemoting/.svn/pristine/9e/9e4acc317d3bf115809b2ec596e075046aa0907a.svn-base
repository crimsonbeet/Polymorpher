
#include "stdafx_classfactory.h"
#include "XMLParser.h"
#include "WSIObject.h"


#if defined(_MSC_VERXXX)

#include <shlobj.h>
#include <comutil.h>


inline void MSXMLThrowParseError(IXMLDOMDocument *pXMLDoc) {
	IXMLDOMParseError *parse_error = 0;
	if(pXMLDoc) {
		pXMLDoc->get_parseError(&parse_error);
		throw parse_error;
	}
}

inline void MSXMLThrowParseError(IXMLDOMNode *pNode) {
	IXMLDOMDocument *pXMLDoc = 0;
	if(pNode) {
		pNode->get_ownerDocument(&pXMLDoc);
	}
	if(pXMLDoc) {
		MSXMLThrowParseError(pXMLDoc);
		pXMLDoc->Release();
	}
}

void MSXMLPrintParseError(IXMLDOMParseError *err) {
	if(err) {
		BSTR bstr_err;
		err->get_reason(&bstr_err);
		std::cout << bstr_err << std::endl;
	}
}

IXMLDOMNode *NextSibling(IXMLDOMNode *pNode) {
	IXMLDOMNode *pSibling;
	pNode->get_nextSibling(&pSibling);
	pNode->Release();
	return pSibling;
}

IXMLDOMNode *FirstChild(IXMLDOMNode *pNode) {
	IXMLDOMNode *pChild;
	pNode->get_firstChild(&pChild);
	return pChild;
}

DOMNodeType FirstChildNodeType(IXMLDOMNode *pNode) {
	DOMNodeType node_type = (DOMNodeType)0;
	IXMLDOMNode *pChild = FirstChild(pNode);
	if(pChild) {
		pChild->get_nodeType(&node_type);
		pChild->Release();
	}
	return node_type;
}

long ChildNodesLength(IXMLDOMNode *pNode) {
	VARIANT_BOOL bool_haschildnodes;
	pNode->hasChildNodes(&bool_haschildnodes);
	long iListLength = 0;
	if(bool_haschildnodes == VARIANT_TRUE) {
		IXMLDOMNodeList *pList = 0;
		pNode->get_childNodes(&pList);
		pList->get_length(&iListLength);
		pList->Release();
	}
	return iListLength;
}

long AttributesLength(IXMLDOMNamedNodeMap *attributes) {
	long iLength = 0;
	attributes->get_length(&iLength);
	return iLength;
}

void MoveChildNodes(IXMLDOMNode *pNodeSrc, IXMLDOMNode *pNodeDst) {
	IXMLDOMNodeList *pList;
	pNodeSrc->get_childNodes(&pList);
	if(pList) {
		IXMLDOMNode *pNext = 0;
		for(pList->nextNode(&pNext); pNext; pList->nextNode(&pNext)) {
			pNodeDst->appendChild(pNext, NULL);
			pNodeSrc->removeChild(pNext, NULL);
			pNext->Release();
		}
		pList->Release();
	}
}

IXMLDOMNode *FindReference(IXMLDOMNode *pAttribute) {
	IXMLDOMNode *pXMLRef = 0;
	BSTR bstr_text;
	pAttribute->get_text(&bstr_text);
	if(SysStringLen(bstr_text) > 1) {
		wchar_t query[512];
		swprintf(query, ARRAY_NUM_ELEMENTS(query), L"//*[@id='%s']", bstr_text + 1);
		IXMLDOMDocument *pXMLDoc = 0;
		pAttribute->get_ownerDocument(&pXMLDoc);
		pXMLDoc->selectSingleNode(query, &pXMLRef);
		pXMLDoc->Release();
	}
	SysFreeString(bstr_text);
	return pXMLRef;
}

BOOL ResolveReference(IXMLDOMNode *pNode, IXMLDOMNamedNodeMap *attributes) {
	BOOL rc = FALSE;
	IXMLDOMNode *pAttribute = 0;
	attributes->getNamedItem(_bstr_t("href"), &pAttribute);
	if(pAttribute) {
		IXMLDOMNode *pXMLRefNode = FindReference(pAttribute);
		if(pXMLRefNode) {
			attributes->removeNamedItem(_bstr_t("href"), NULL);
			MoveChildNodes(pXMLRefNode, pNode);
			IXMLDOMNode *pXMLRefParent = 0;
			pXMLRefNode->get_parentNode(&pXMLRefParent);
			if(pXMLRefParent) {
				pXMLRefParent->removeChild(pXMLRefNode, NULL);
				pXMLRefParent->Release();
				rc = TRUE;
			}
			pXMLRefNode->Release();
		}
		pAttribute->Release();
	}
	return rc;
}

void MSXMLAddProperty(IXMLDOMNode *pNode, std::string& base, IWSIMaterializer& mat) {
	size_t pos;
	size_t len;
	wsi_object& obj = *(reinterpret_cast<wsi_object*>(mat.get_context_current()._obj));

	BSTR bstr_name;
	pNode->get_baseName(&bstr_name);
	len = SysStringLen(bstr_name);
	pos = base.size();
	base.resize(pos + len);
	WideCharToMultiByte(CP_ACP, NULL, bstr_name, len, &base[pos], base.capacity() - pos, NULL, NULL);
	SysFreeString(bstr_name);

	size_t s = obj._vecCaptures.size();
	obj._vecCaptures.resize(s + 1);
	obj._vecCaptures[s].first._begin = &base[pos];
	obj._vecCaptures[s].first._end = &base[pos] + len;

	BSTR bstr_text;
	pNode->get_text(&bstr_text);
	len = SysStringLen(bstr_text);
	pos = base.size();
	if(len > 0) { 
		base.resize(pos + len);
		WideCharToMultiByte(CP_ACP, NULL, bstr_text, len, &base[pos], base.capacity() - pos, NULL, NULL);
	} 
	SysFreeString(bstr_text);

	if(len > 0) { 
		obj._vecCaptures[s].second._begin = &base[pos];
		obj._vecCaptures[s].second._end = &base[pos] + len;
	} 
	else { 
		obj._vecCaptures.resize(s);
	} 
}

void MSXMLAddObject(IXMLDOMNode *pNode, IXMLDOMNamedNodeMap *attributes, std::string& base, IWSIMaterializer& mat) {
	size_t pos;
	size_t len;

	BSTR bstr_name;
	pNode->get_baseName(&bstr_name);
	len = SysStringLen(bstr_name);
	pos = base.size();
	base.resize(pos + len);
	WideCharToMultiByte(CP_ACP, NULL, bstr_name, len, &base[pos], base.capacity() - pos, NULL, NULL);
	SysFreeString(bstr_name);

	wsi_object obj;

	obj._header.first._begin = &base[pos];
	obj._header.first._end = &base[pos] + len;

	wsi_object& curent_obj = *(reinterpret_cast<wsi_object*>(mat.get_current()._obj));
	curent_obj.clear();
	curent_obj._header = obj._header;

	mat.place_composite_object_on_stack(IWSIObject(&obj));

	if(attributes == 0) {
		return;
	}

	IXMLDOMNode *pAttribute = 0;
	for(long j = 0, len = AttributesLength(attributes); j < len; ++j) {
		attributes->get_item(j, &pAttribute);
		if(pAttribute) {
			BSTR bstr_prefix;
			pAttribute->get_prefix(&bstr_prefix);
			if(SysStringLen(bstr_prefix) == 0) {
				MSXMLAddProperty(pAttribute, base, mat);
			}
			SysFreeString(bstr_prefix);
			pAttribute->Release();
		}
	}
}

BOOL MSXMLPlaceElement(IXMLDOMNode *pNode, std::string& base, IWSIMaterializer& mat) {
	BOOL is_object = FALSE;

	IXMLDOMNamedNodeMap *attributes;
	//BSTR bstr_name;
	//BSTR bstr_namespace;
	//BSTR bstr_prefix;
	//pNode->get_baseName(&bstr_name);
	//pNode->get_namespaceURI(&bstr_namespace);
	//pNode->get_prefix(&bstr_prefix);

	long iChildListLength = 0;
	long iAttributeListLength = 0;

	DOMNodeType node_type;
	pNode->get_nodeType(&node_type);
	switch(node_type) {
		case NODE_ELEMENT:
			pNode->get_attributes(&attributes);
			if(attributes != 0) {
				ResolveReference(pNode, attributes);
			}

			iChildListLength = ChildNodesLength(pNode);
			iAttributeListLength = AttributesLength(attributes);

			if(iChildListLength == 1 && FirstChildNodeType(pNode) == NODE_TEXT) {
				MSXMLAddProperty(pNode, base, mat);
			}
			else
			if(iChildListLength > 0 || iAttributeListLength > 0) {
				MSXMLAddObject(pNode, attributes, base, mat);
				is_object = TRUE;
			}
			if(attributes != 0) {
				attributes->Release();
			}
			break;
		case NODE_ATTRIBUTE:
			break;
		case NODE_TEXT:
			break;
		case NODE_CDATA_SECTION:
			break;
		case NODE_ENTITY_REFERENCE:
			break;
		case NODE_ENTITY:
			break;
		case NODE_PROCESSING_INSTRUCTION:
			break;
		case NODE_COMMENT:
			break;
		case NODE_DOCUMENT:
			break;
		case NODE_DOCUMENT_TYPE:
			break;
		case NODE_DOCUMENT_FRAGMENT:
			break;
		case NODE_NOTATION:
			break;
	}

	return is_object;
}



void MSXMLNodeParser(IXMLDOMNode *pNode, std::string& parsed_content, IWSIMaterializer& mat) {
	BOOL is_object = MSXMLPlaceElement(pNode, parsed_content, mat);

	for(IXMLDOMNode *pChild = FirstChild(pNode); pChild; pChild = NextSibling(pChild)) {
		MSXMLNodeParser(pChild, parsed_content, mat);
	}

	if(is_object) {
		mat.reduce_stack();
	}
}

void MSXMLParser(IXMLDOMDocument *pXMLDoc, const std::string& xml, IWSIMaterializer& mat) {
	const _bstr_t bstrxml(xml.c_str());

	pXMLDoc->put_async(false);

	VARIANT_BOOL isSuccessful;
	HRESULT hr = pXMLDoc->loadXML(bstrxml, &isSuccessful);
	if(hr != S_OK) {
		MSXMLThrowParseError(pXMLDoc);
	}

	IXMLDOMNode *pRootNode;
	pXMLDoc->get_firstChild(&pRootNode);

	std::string parsed_content;

	for(parsed_content.reserve(xml.size()); pRootNode; pRootNode = NextSibling(pRootNode)) {
		try {
			MSXMLNodeParser(pRootNode, parsed_content, mat);
		}
		catch(IXMLDOMParseError *err) {
			MSXMLPrintParseError(err);
		}
	}
}

void CXMLParser::parse(const std::string& xml, IWSIMaterializer& mat) const {
	IXMLDOMDocument *pXMLDoc;

	CoInitializeEx(0, COINIT_MULTITHREADED);
	HRESULT hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&pXMLDoc);
	if(hr != S_OK) {
		return;
	}

	try {
		MSXMLParser(pXMLDoc, xml, mat);
	}
	catch(IXMLDOMParseError *err) {
		MSXMLPrintParseError(err);
	}

	pXMLDoc->Release();
}

#else

void yyxmlerror(
				std::string::const_iterator& _begin,
				std::string::const_iterator& _end,
				IWSIMaterializer&,
				char const *err);
int yyxmlparse(
				std::string::const_iterator& _begin,
				std::string::const_iterator& _end,
				IWSIMaterializer& _context);

void CXMLParser::parse(const std::string& wsi, IWSIMaterializer& mat) const {
	std::string::const_iterator from = wsi.begin();
	std::string::const_iterator to = wsi.end();
	yyxmlparse(from, to, mat);
}
void CXMLParser::parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer& mat) const {
	yyxmlparse(from, to, mat);
}


void CXMLMProConfigParser::parse(const std::string& wsi, IWSIMaterializer& mat) const {
	mat.reserve(750000); 
	CXMLParser::parse(wsi, mat); 
	mat.flush(); 
	mat.release(); 
} 
void CXMLMProConfigParser::parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer& mat) const {
	mat.reserve(750000);
	CXMLParser::parse(from, to, mat);
	mat.flush();
	mat.release();
}

void yylogerror(std::string::const_iterator& _begin, std::string::const_iterator& _end, IWSIMaterializer&, char const *err, const std::string& prefix); 

#if defined(_WIN32_WINDOWS)
#pragma warning(disable: 4065)
#endif

#include "XMLParser.y.tab.h"


void yyxmlerror(std::string::const_iterator& _begin, std::string::const_iterator& _end, IWSIMaterializer& mat, char const *err) {
	yylogerror(_begin, _end, mat, err, "xml-parser"); 
}


int yyxml_value_tokens[3] = { Word, String, Comment };


int yyxml_scanstring(yyxml_token_type *yylval, char marker, std::string::const_iterator& _begin, std::string::const_iterator& _end) { 
	while(_begin != _end) {
		if(*(_begin) == marker) { 
			if((_begin + 1) == _end || _begin[1] != marker) { 
				break; 
			} 
			++_begin; // Skip marker.
		} 
		else 
		if(*(_begin) == '<') { 
			if((_begin + 1) != _end && _begin[1] == '/') { 
				break; 
			} 
		} 
		++_begin; 
	}
	if(_begin != _end) { 
		yylval->_substr._end = &(*_begin++);
	} 
	else { 
		yylval->_substr._end = &(*(_begin - 1)) + 1;
	} 
	return 1; 
} 

int yyxml_onopentag(yyxml_token_type *yylval, std::string::const_iterator& _begin, std::string::const_iterator& _end) { 
	int t = '<'; 
	if(_begin != _end) { 
		// Check for Comment: comment is a separate token. 
		if(*(_begin) == '!') { 
			if((_begin + 1) != _end && _begin[1] == '-') { 
				if((_begin + 2) != _end && _begin[2] == '-') { 
					_begin += 2; 
					yylval->_substr._begin = &_begin[0] + 1; 
					yylval->_substr._end = (&(_end-1)[0]) + 1; 
					int done = 0; 
					while(!done && ++_begin != _end) { 
						if(_begin[0] == '-') { 
							if((_begin + 1) != _end && _begin[1] == '-') { 
								if((_begin + 2) != _end && _begin[2] == '>') { 
									yylval->_substr._end = &_begin[0]; 
									_begin += 3; 
									done = 1; 
								} 
							} 
						} 
					} 
					t = 2; 
					return yyxml_value_tokens[t];
				} 
			} 
		} 
	} 
	return t; 
} 

int yyxmllex(yyxml_token_type *yylval, std::string::const_iterator& _begin, std::string::const_iterator& _end) {

	while(_begin != _end && *(_begin) < 0x21) {
		++_begin;
	}

	if(_begin == _end) {
		return 0;
	}

	const char& c = *(_begin++); 

	yylval->_substr._end = (yylval->_substr._begin = &c) + 1;

//	Check non value tokens first.
	switch(c) {
		case '<': 
			return yyxml_onopentag(yylval, _begin, _end); 
		case '>':
		case '/':
		case '=':
		case '?':
		case ':':
		case '!':
		case ',':
		case ';':
		case '{':
		case '}': 
			return c; 
		case '\'': // (Marker ') is a begin of String token.
		case '\"': // (Marker ") is a begin of String token.
			return yyxml_value_tokens[yyxml_scanstring(yylval, c, _begin, _end)]; 
		break;
	}

	int t = 0; 

	int done = 0; 
	while(!done && _begin != _end) {
		char xchar = *(_begin);
		if(xchar < 0x21) {
			done = 1; 
		} 
		else 
		switch(xchar) {
			case '<': 
			case '>':
			case '/':
			case '=':
			case '?':
			case ':':
			case '!':
			case ',':
			case ';':
			case '{':
			case '}':
				done = 1;
			break; 
			default: 
				++_begin;
			break;
		}
	}

	yylval->_substr._end = _begin != _end? &(*_begin): &(*(_begin - 1)) + 1;

	return yyxml_value_tokens[t];
}


ISTREAMWriter * CXMLMProConfigParser::create_writer() const {
	return new CXMLWriter();
}


#endif

ISTREAMWriter * CXMLParser::create_writer() const {
	return new CXMLWriter();
}

int CXMLParser::Encoding() const {
	return 2;
}

