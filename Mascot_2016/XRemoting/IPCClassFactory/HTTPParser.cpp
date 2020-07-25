
#include "stdafx_classfactory.h"
#include "HttpParser.h"
#include "HttpWriter.h"

#include "WSIObject.h"






void yyhttperror(
				std::string::const_iterator& _begin,
				std::string::const_iterator& _end,
				IWSIMaterializer&,
				char const *err);
int yyhttpparse(
				std::string::const_iterator& _begin,
				std::string::const_iterator& _end,
				IWSIMaterializer& _context);


void CHttpParser::parse(const std::string& wsi, IWSIMaterializer& mat) const {
	std::string::const_iterator from = wsi.begin();
	std::string::const_iterator to = wsi.end();
	yyhttpparse(from, to, mat);
}

void CHttpParser::parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer& mat) const {
	yyhttpparse(from, to, mat);
}

ISTREAMWriter * CHttpParser::create_writer() const {
	return new CHttpWriter();
}

int CHttpParser::Encoding() const {
	return 1;
}

void yylogerror(std::string::const_iterator& _begin, std::string::const_iterator& _end, IWSIMaterializer&, char const *err, const std::string& prefix); 


#if defined(_WIN32_WINDOWS)
#pragma warning(disable: 4065)
#endif

#include "HttpParser.y.tab.h"


void yyhttperror(std::string::const_iterator& _begin, std::string::const_iterator& _end, IWSIMaterializer& mat, char const *err) {
	yylogerror(_begin, _end, mat, err, "http-parser"); 
}

BOOL yyhttp_check_newline(std::string::const_iterator& _begin, std::string::const_iterator& _end) {
	while(_begin != _end && *(_begin) == 0xD) {
		++_begin;
	}
	if(_begin != _end) {
		if(*(_begin) == 0xA) {
			++_begin;
			return TRUE;
		}
	}
	return FALSE;
}

int yyhttp_check_text_token_after_newline(yyhttp_token_type *yylval, std::string::const_iterator& _begin, std::string::const_iterator& _end) {
	size_t newline_count = 0;
	for(std::string::const_iterator it = _begin - 1; (_begin - it) < 5 && (*(it) == 0xD || *(it) == 0xA); --it) {
		if(*(it) == 0xA) {
			++newline_count;
		}
	}
	if(newline_count > 1) {
		if(_begin != _end && *(_begin) != 0) {
			yylval->clear();
			yylval->_substr._begin = &_begin[0];
			yylval->_substr._end = &_begin[0] + (_end - _begin);
			_begin = _end;
			return Text; // Text is terminating token
		}
	}
	return NewLine;
}

int yyhttplex(yyhttp_token_type *yylval, std::string::const_iterator& _begin, std::string::const_iterator& _end) {
	if(yyhttp_check_newline(_begin, _end)) {
		return yyhttp_check_text_token_after_newline(yylval, _begin, _end);
	}

	if(_begin == _end || *_begin == 0) {
		return 0;
	}

	if(*(_begin) == 0x20) {
		while(_begin != _end && *(_begin) == 0x20) {
			++_begin;
		}
		if(yyhttp_check_newline(_begin, _end)) {
			return yyhttp_check_text_token_after_newline(yylval, _begin, _end);
		}
		return ' ';
	}

	yylval->clear();
	yylval->_substr._begin = &_begin[0];

	switch(*(_begin)) {
	case ':': case ',': case ';': case '=': case '\t':
		yylval->_substr._end = &_begin[0] + 1;
		return *(_begin++);
	}

	yylval->clear();
	yylval->_substr._begin = &_begin[0];

	if(*(_begin) == '(') { // Marker ( is a begin of GeneralString.
		for(++_begin; _begin != _end && *(_begin) != ')' && *(_begin) >= 0x20; ++_begin);
		if(_begin != _end) {
			++_begin;
		}
		yylval->_substr._end = _begin != _end? &_begin[0]: (&(_begin-1)[0]) + 1;
		return Word;
	} 

	int urlfollows = 0; 

	if((_begin + 5) < _end) {
		if(_begin[0] == 'H' && memcmp(&_begin[0], "HTTP/", 5) == 0) {
			_begin += 5;
			yylval->_substr._end = &_begin[0] + 5;
			return Http;
		} 
		if(_begin[0] == '/' && _begin[1] == '/') { 
			urlfollows = 1; 
		} 
	}

	int done = 0;

	while(!done && _begin != _end) {
		char xchar = *(_begin);
		if(xchar < 0x21) {
			done = 1;
		}
		else 
		if(urlfollows) { 
			++_begin;
		} 
		else 
		switch(xchar) {
			case ':':
			case ',':
			case ';':
			case '=':
				done = 1;
			break;
			default:
				++_begin;
			break;
		}
	}

	yylval->_substr._end = _begin != _end? &_begin[0]: (&(_begin-1)[0]) + 1;
	return Word;
}



void CHttpWriter::BeginObject(const std::string& id, int index, bool is_composite) {
	if(id == "HttpMessage") {
		_current_object_type = 0;
	}
	else
	if(id == "Header") {
		_content += (char)0xD;
		_content += (char)0xA;
		_current_object_type = 1;
	}
	else
	if(id == "HeaderValue") {
		if(_current_object_type == 1) {
			_content += ':';
		}
		if(_current_object_type == 2) {
			_content += ';';
		}
		_current_object_type = 2;
	}
	else
	if(id == "Payload") {
		_content += (char)0xD;
		_content += (char)0xA;
		_content += (char)0xD;
		_content += (char)0xA;
		_current_object_type = 3;
	}
	else {
		_current_object_type = -1;
	}

	_attributes_count = 0;
}


void CHttpWriter::AddAttribute(const std::string& id, const std::string& value, int index, bool out_of_order) {
	if(_current_object_type == 0) {
		if(id[0] == 'V' && id == "Version") {
			return;
		}
	}
	if(_current_object_type == 2) {
		if((id[0] == 'N' && id == "Name") || (id[0] == 'V' && id == "Value")) {
			return;
		}
	}
	if(_attributes_count > 0) {
		switch(_current_object_type) {
			case 2: _content += ','; break;
		}
		_content += ' ';
	}

	_content += value;

	++_attributes_count;
}

void CHttpWriter::EndIterator(const std::string&, int) {
}

void CHttpWriter::EndObject(const std::string& id, int, bool, int) {
	if(id == "HttpMessage") {
		if(_current_object_type != 3) {
			_content += (char)0xD;
			_content += (char)0xA;
			_content += (char)0xD;
			_content += (char)0xA;
		}
	}
}





#ifdef WSI_NAMESPACE
#undef WSI_NAMESPACE
#endif
#define WSI_NAMESPACE XClassFactory

#ifdef autocreateserialization_not_necessaryH
#undef autocreateserialization_not_necessaryH
#endif



#define WSICLASSFACTORY_IMPL
#define XCLASSFACTORY_IMPL
//#define DELAY_IMPLEMENTATION_ROOTCOLLECTION_MAP

#define httpmessage_internal_use_only

#include "HttpMessage.h"







EXP IHttpMessage * CreateHttpRequest(int method) {
	return new HttpMessage(method);
}



HttpMessage::HttpMessage(int method) {
	switch(method) {
		case HttpUnknown: _method = "???"; break;
		case HttpResponse: break;
		case HttpGet: _method = "GET"; break;
		case HttpHead: _method = "HEAD"; break;
		case HttpPut: _method = "PUT"; break;
		case HttpPost: _method = "POST"; break;
		case HttpTrace: _method = "TRACE"; break;
		case HttpOptions: _method = "OPTIONS"; break;
		case HttpDelete: _method = "DELETE"; break;
	}
	_headers_pos = -1;
	_response = 0;

	_headers.reserve(64);
}

HttpMessage::~HttpMessage() {
	if(_response) {
		delete _response;
	}
}

HttpMessage& HttpMessage::operator = (const HttpMessage& other) {
	_method = other._method;
	_url = other._url;
	_version = other._version;
	_outputversion = other._outputversion;
	_status_code = other._status_code;
	_status_description = other._status_description;
	_headers = other._headers;
	_payload = other._payload;

	_headers_pos = -1;

	return *this;
}

IHttpMessage& HttpMessage::GetHttpResponse(BOOL doclear) {
	if(_response == 0) {
		_response = new HttpMessage(HttpResponse);
		doclear = TRUE; 
	}
	if(_response && doclear) { 
		_response->Clear(); 

		IHttpHeader *pheader = 0; 
		if(GetHeader("Connection", &pheader)) { 
			if(pheader->FindValue("Keep-Alive")) { 
				int ver_first = 0; 
				int ver_second = 0; 
				if(GetVersion(ver_first, ver_second) && (ver_first == 0 || (ver_first == 1 && ver_second == 0))) { 
					IHttpHeaderValue& keepalive = GetWorkerHeaderValue(); 
					keepalive.Clear(); 
					keepalive.OutputPair("Keep-Alive"); 

					_response->SetHeader("Connection", keepalive); 
				} 
			} 
		} 

		IHttpHeaderValue& cache_control = GetWorkerHeaderValue();
		cache_control.OutputPair("no-store");

		_response->SetHeader("Cache-Control", cache_control);
	} 
	return *_response;
}

IHttpHeaderValue& HttpMessage::GetWorkerHeaderValue(BOOL doclear) {
	if(doclear) { 
		_workerheadervalue.Clear(); 
	} 
	return _workerheadervalue;
}

IHttpFormdata& HttpMessage::GetWorkerFormdata(BOOL doclear) {
	if(doclear) {
		_formdata.Clear();
	}
	return _formdata;
}

VOID HttpMessage::Clear() {
	*this = HttpMessage(GetMethod());

	SetVersion(1, 1);
}


int HttpMessage::GetMethod() {
	int method = HttpUnknown;
	if(_method.size() == 0) method = HttpResponse; else
	if(strcmp(_method.c_str(), "GET") == 0) method = HttpGet; else
	if(strcmp(_method.c_str(), "HEAD") == 0) method = HttpHead; else
	if(strcmp(_method.c_str(), "PUT") == 0) method = HttpPut; else
	if(strcmp(_method.c_str(), "POST") == 0) method = HttpPost; else
	if(strcmp(_method.c_str(), "TRACE") == 0) method = HttpTrace; else
	if(strcmp(_method.c_str(), "OPTIONS") == 0) method = HttpOptions; else
	if(strcmp(_method.c_str(), "DELETE") == 0) method = HttpDelete;
	return method;
}

BOOL HttpMessage::GetVersion(int& first, int& second) {
	BOOL rc = _version.size() && _version[0] == '1';
	first = 0;
	second = 0;
	if(_version.size()) {
		sscanf(_version.c_str(), "%d.%d", &first, &second);
	}
	return rc;
}

BOOL HttpMessage::GetText(std::string& text) {
	text.resize(_payload._text.size()); 
	memcpy((void*)text.data(), _payload._text.data(), _payload._text.size()); 
	//text = _payload._text;
	return text.size() > 0;
}

BOOL HttpMessage::GetUrl(std::string& url) {
	BOOL rc = _url.size() > 0;
	url = _url;
	return rc;
}

BOOL HttpMessage::GetHeader(const std::string& name, IHttpHeader **ppheader) {
	BOOL rc = FALSE;
	std::vector<HttpHeader>::iterator iter = _headers.begin();
	while(iter != _headers.end() && (*iter)._header_name != name) {
		++iter;
	}
	if(iter != _headers.end()) {
		rc = TRUE;
		if(ppheader) {
			*ppheader = &(*iter);
		}
	}
	return rc;
}

BOOL HttpMessage::GetFirstHeader(IHttpHeader **ppfirst) {
	BOOL rc = FALSE;
	if(ppfirst) {
		*ppfirst = 0;
	}
	if((_headers_pos = 0) < (int)_headers.size()) {
		rc = TRUE;
		if(ppfirst) {
			*ppfirst = &_headers[0];
		}
	}
	return rc;
}

BOOL HttpMessage::GetNextHeader(IHttpHeader **ppnext) {
	BOOL rc = FALSE;
	if(ppnext) {
		*ppnext = 0;
	}
	if((++_headers_pos) < (int)_headers.size()) {
		rc = TRUE;
		if(ppnext) {
			*ppnext = &_headers[_headers_pos];
		}
	}
	return rc;
}

BOOL HttpMessage::SetVersion(int first, int second) {
	char buf[32];
	sprintf(buf, "%d.%d", first, second);
	_version = buf;
	_outputversion = "HTTP/" + _version;
	return first > 0;
}

BOOL HttpMessage::SetText(const std::string& prefix, const std::string& suffix) {
	int payload_size = (int)(prefix.size() + suffix.size());
	BOOL rc = payload_size > 0;

	if(rc) { 
		_payload._text.reserve(payload_size);
		_payload._text = (const std::payload_string&)prefix;
		if(suffix.size()) {
			_payload._text += (const std::payload_string&)suffix;
		}

		char buf[16]; 
		sprintf(buf, "%d", (int)payload_size);

		HttpHeaderValue avalue; 
		avalue.Clear();
		avalue.OutputPair(buf);

		SetHeader("Content-Length", avalue, TRUE/*replace old value*/);
	}
	else {
		_payload._text.resize(0);
	} 

	return rc;
}

BOOL HttpMessage::SetUrl(const std::string& url) {
	BOOL rc = url.size() > 0 && _method.size() > 0;
	if(rc) {
		_url = url;
	}
	return rc;
}

BOOL HttpMessage::SetStatus(const std::string& status) {
	BOOL rc = status.size() > 0 && _method.size() == 0;
	if(rc) {
		_status_code = status;
	}
	return rc;
}

BOOL HttpMessage::SetDescription(const std::string& description) {
	BOOL rc = description.size() > 0 && _method.size() == 0;
	if(rc) {
		_status_description = description;
	}
	return rc;
}

BOOL HttpMessage::SetHeader(const std::string& name, const IHttpHeaderValue& value, BOOL replace) {
	BOOL rc = TRUE;
	size_t pos = _headers.size();
	for(size_t x = 0; x < pos; ++x) {
		if(_headers[x]._header_name == name) {
			pos = x;
		}
	}
	if(pos == _headers.size()) {
		_headers.push_back(HttpHeader());
		_headers[pos]._header_name = name;
	}
	if(replace) { 
		_headers[pos]._header_values.clear();
	} 
	_headers[pos]._header_values.push_back((HttpHeaderValue&)value);
	return rc;
}

BOOL HttpMessage::RemoveHeader(const std::string& name) {
	BOOL rc = FALSE;
	size_t pos = _headers.size();
	for(size_t x = 0; x < pos; ++x) {
		if(_headers[x]._header_name == name) {
			pos = x;
		}
	}
	if(pos != _headers.size()) {
		_headers.erase(_headers.begin() + pos);
		rc = TRUE; 
	}
	return rc;
}

ISTREAMWriter* HttpMessage::GetXmlWriter() { 
	return &_xmlwriter; 
} 

BOOL HttpMessage::SetContentType(const std::string& url, const std::string& encoding) {
	std::string file_content;
	std::string content_type;
	std::string accept_ranges;

	content_type = GetFileNameExtension(url);
	if(content_type == "jpg") {
		content_type = "jpeg";
	}

	if(content_type == "jpeg" || content_type == "gif" || content_type == "png" || content_type == "bmp") {
		accept_ranges = "bytes";
		content_type = std::string("image/") + content_type;
	}
	else
	if(content_type == "js") {
		accept_ranges = "bytes";
		content_type = "application/javascript";
		file_content += "\r\n";
	}
	else {
		content_type = std::string("text/") + content_type;
		file_content += "\r\n";
	}

	IHttpHeaderValue& aValue = GetWorkerHeaderValue();

	aValue.Clear();
	aValue.OutputPair(content_type);
	SetHeader("Content-Type", aValue);

	if(accept_ranges.size()) {
		aValue.Clear();
		aValue.OutputPair(accept_ranges);
		SetHeader("Accept-Ranges", aValue);
	}

	if(encoding.size()) {
		aValue.Clear();
		aValue.OutputPair(encoding);
		SetHeader("Content-Encoding", aValue);
	}

	return TRUE;
}

BOOL HttpMessage::InvokeMarshaller(IHttpMessage& response, ISTREAMReader& reader, const std::string& text_in, ISTREAMWriter *writer, const std::string& prefix) {
	BOOL rc = FALSE;
	TLSObject *tlsobj = GetTLSObject2();
	if(tlsobj) {
		XRPCHandle *rpc_handle = (XRPCHandle*)tlsobj->GetRpcHandle();
		if(rpc_handle) {
			std::string text_out;
			if(rpc_handle->InvokeMarshaller(reader, text_in, writer, text_out)) {
				if(text_out.size()) {
					IHttpHeaderValue& aValue = GetWorkerHeaderValue();
					aValue.Clear();
					aValue.OutputPair("text/plain");
					response.SetHeader("Content-Type", aValue);

					response.SetText(prefix, text_out);
					rc = true;
				}
			}
		}
	}

	return rc;
}

BOOL HttpMessage::InvokeMarshallerOnPayload(IHttpMessage& response, ISTREAMReader& reader, ISTREAMWriter *writer, const std::string& prefix) {
	std::string text_in;

	BOOL rc = FALSE;
	if(GetText(text_in)) {
		rc = InvokeMarshaller(response, reader, text_in, writer, prefix); 
	}

	return rc;
}


HttpHeader::HttpHeader(const std::string& name) {
	_header_name = name;
	_header_values_pos = -1;
}
HttpHeader::~HttpHeader() {
}

BOOL HttpHeader::FindValue(const std::string& value, const std::string& name, int *pIndex, int *pPairIndex) {
	int found = 0;
	for(size_t x = 0; x < _header_values.size(); ++x) { 
		HttpHeaderValue& http_value = _header_values[x]; 
		for(size_t y = 0; !found && y < http_value._values.size(); ++y) { 
			if(http_value._names[y].size()) {
				if(name.size()) {
					if(name == http_value._names[y]) {
						if(value.size() == 0 || http_value._values[y] == value) {
							found = (int)(y + 1);
						}
					}
				} 
				else 
				if(value.size() && value == http_value._names[y]) {
					found = (int)(y + 1);
				} 
			} 
			else
			if(http_value._values[y] == value && name.size() == 0) {
				found = (int)(y + 1);
			}
		} 
		if(found) {
			if(pIndex) {
				*pIndex = (int)x;
			}
			if(pPairIndex) {
				*pPairIndex = found - 1;
			}
			break;
		}
	} 
	return found? TRUE: FALSE;
}

BOOL HttpHeader::GetValue(std::string& value, int index, int pair_index) {
	BOOL rc = FALSE;
	if(index >= 0 && index < (int)_header_values.size()) {
		HttpHeaderValue& http_value = _header_values[index];
		if(pair_index >= 0 && pair_index < (int)http_value._values.size()) {
			value = http_value._values[pair_index];
			rc = TRUE;
		}
	}
	return rc;
} 

BOOL HttpHeader::GetFirstValue(IHttpHeaderValue **ppfirst) {
	BOOL rc = FALSE;
	if(ppfirst) {
		*ppfirst = 0;
	}
	if((_header_values_pos = 0) < (int)_header_values.size()) {
		rc = TRUE;
		if(ppfirst) {
			*ppfirst = &_header_values[0];
		}
	}
	return rc;
}

BOOL HttpHeader::GetNextValue(IHttpHeaderValue **ppnext) {
	BOOL rc = FALSE;
	if(ppnext) {
		*ppnext = 0;
	}
	if((++_header_values_pos) < (int)_header_values.size()) {
		rc = TRUE;
		if(ppnext) {
			*ppnext = &_header_values[_header_values_pos];
		}
	}
	return rc;
}



HttpHeaderValue::HttpHeaderValue(const std::string& value, const std::string& name) {
	if(value.size()) {
		AddPair(value, name);
	}
	_values_pos = -1;
}
HttpHeaderValue::~HttpHeaderValue() {
}

BOOL HttpHeaderValue::AddPair(const std::string& value, const std::string& name) {
	if(value.size()) {
		_values.push_back(value);
		_names.push_back(name);

		OutputPair(value, name);
	}

	return _values.size() == _names.size();
}

BOOL HttpHeaderValue::OutputPair(const std::string& value, const std::string& name) {
	BOOL rc = FALSE;

	if(value.size()) {
		std::string mergedvalue;
		if(name.size()) {
			mergedvalue = name + '=';
		}
		mergedvalue += value;
		rc = TRUE;

		_mergedvalues.push_back(mergedvalue);
	}

	return rc;
}

VOID HttpHeaderValue::Clear() {
	_names.resize(0);
	_values.resize(0);
	_mergedvalues.resize(0);
}

BOOL HttpHeaderValue::GetFirstPair(std::string& value, std::string& name) {
	_values_pos = -1;
	return GetNextPair(value, name);
}

BOOL HttpHeaderValue::GetNextPair(std::string& value, std::string& name) {
	BOOL rc = FALSE;
	if((++_values_pos) < (int)_values.size()) {
		rc = TRUE;
		value = _values[_values_pos];
		name = _names[_values_pos];
	}
	return rc;
}




BOOL HttpFormdata::GetHeader(const std::string& name, IHttpHeader **ppheader) {
	BOOL rc = FALSE;
	std::vector<HttpHeader>::iterator iter = _headers.begin();
	while(iter != _headers.end() && (*iter)._header_name != name) {
		++iter;
	}
	if(iter != _headers.end()) {
		rc = TRUE;
		if(ppheader) {
			*ppheader = &(*iter);
		}
	}
	return rc;
}

BOOL HttpFormdata::GetFirstHeader(IHttpHeader **ppfirst) {
	BOOL rc = FALSE;
	if(ppfirst) {
		*ppfirst = 0;
	}
	if((_headers_pos = 0) < (int)_headers.size()) {
		rc = TRUE;
		if(ppfirst) {
			*ppfirst = &_headers[0];
		}
	}
	return rc;
}

BOOL HttpFormdata::GetNextHeader(IHttpHeader **ppnext) {
	BOOL rc = FALSE;
	if(ppnext) {
		*ppnext = 0;
	}
	if((++_headers_pos) < (int)_headers.size()) {
		rc = TRUE;
		if(ppnext) {
			*ppnext = &_headers[_headers_pos];
		}
	}
	return rc;
}

HttpFormdata::HttpFormdata() {
	_headers_pos = -1;
	_headers.reserve(2);
}

BOOL HttpFormdata::GetText(std::string& text) {
//	text.swap(*(std::string*)(void*)&_payload._text);
	text.resize(_payload._text.size());
	memcpy((void*)text.data(), _payload._text.data(), _payload._text.size());
	return text.size() > 0;
}

VOID HttpFormdata::Clear() {
	_headers.resize(0);
	_headers_pos = -1;
	_payload._text.resize(0);
}





BOOL HttpMessage::GetFirstFormdata(IHttpFormdata& first) {
	BOOL rc = FALSE;
	std::payload_string& text = _payload._text;

	_formdata_pos = -1;

	if(text.size() > 8) {
		IHttpHeader *pheader = 0;
		if(GetHeader("Content-Type", &pheader)) {
			if(pheader->FindValue("multipart/form-data")) {
				int index;
				int pair_index;
				if(pheader->FindValue("", "boundary", &index, &pair_index)) {
					if(pheader->GetValue(_boundary, index, pair_index)) {
						_formdata_pos = 0;
						rc = GetNextFormdata(first);
					}
				}
			}
		}
	}
	return rc;
}

BOOL HttpMessage::GetNextFormdata(IHttpFormdata& next) {
	BOOL rc = FALSE;
	std::string& text = *(std::string*)(void*)&_payload._text;
	int pos = _formdata_pos;

	_formdata_pos = -1;

	if(pos >= 0 && (pos + (int)_boundary.size() + 4) < (int)text.size()) {
		std::string::const_iterator pText = text.begin() + pos;
		std::string::const_iterator end = text.end() - _boundary.size() - 4;

		while(*pText++ != '-' && pText < end);

		if(pText < end && *pText++ == '-') {
			int x;
			int y;

			// 1. Find start boundary line
			for(x = 0, y = (int)_boundary.size(); x < y && pText < end; ++x) {
				if(_boundary[x] != *pText++) {
					break;
				}
			}

			std::string::const_iterator from = pText;
			std::string::const_iterator to = pText;

			// 2. Find end boundary line
			if(x == y && *pText++ == 0xd && *pText++ == 0xa) { // start boundary ends with new line.
				while(!rc && pText < end) {
					to = pText;
					if(*pText++ == 0xd) if(pText[0] == 0xa) if(pText[1] == '-') if(pText[2] == '-') {
						pText += 3;
						for(x = 0, y = (int)_boundary.size(); x < y; ++x) {
							if(_boundary[x] != *pText++) {
								break;
							}
						}
						if(x == y) {
							TLSObject* tls = GetTLSObject2();
							if(tls) {
								XRPCHandle *rpc_handle = (XRPCHandle*)tls->GetRpcHandle();
								if(rpc_handle) {
									IRemotingObject* remoting = (IRemotingObject*)rpc_handle->_remoting_object;
									if(remoting) {
										IWsiSerializerBase *serializer = remoting->FindObj(GetRootSerializer(&next)._wsi_id);
										serializer->lock();
										serializer->set_outer_ptr(&next);
										CHttpParser parser;
										if(rpc_handle->InvokeMarshaller(parser, from, to)) {
											_formdata_pos = (int)((to + 2) - text.begin());
											rc = TRUE;
										}
										serializer->set_outer_ptr(serializer->get_inner_ptr());
										serializer->unlock();
									}
								}
							}
							if(!rc) {
								// TO DO: fall back to instantiation of serializer and marshaller.
							}
						}
					}
				}
			}
		}
	}
	return rc;
}
