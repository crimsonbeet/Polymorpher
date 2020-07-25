
#ifndef httpmessageH
#define httpmessageH

#include "WSIClassFactory.h"
#include "XMLWriter.h"


typedef enum _HttpMethod {
	HttpUnknown,
	HttpResponse,
	HttpGet,
	HttpHead,
	HttpPut,
	HttpPost,
	HttpTrace,
	HttpOptions,
	HttpDelete

} HttpMethod;


struct IHttpHeaderValue {
	virtual BOOL GetFirstPair(std::string& value, std::string& name) = 0;
	virtual BOOL GetNextPair(std::string& value, std::string& name) = 0;

	virtual BOOL OutputPair(const std::string& value, const std::string& name = "") = 0;
	virtual BOOL AddPair(const std::string& value, const std::string& name = "") = 0;
	virtual VOID Clear() = 0;

	virtual ~IHttpHeaderValue() {}
};


struct IHttpHeader {
	virtual BOOL FindValue(const std::string& value, const std::string& name = "", int *pIndex = 0, int *pPairIndex = 0) = 0;
	virtual BOOL GetValue(std::string& value, int index = 0, int pair_index = 0) = 0;

	virtual BOOL GetFirstValue(IHttpHeaderValue **ppfirst = NULL) = 0;
	virtual BOOL GetNextValue(IHttpHeaderValue **ppnext = NULL) = 0;

	virtual ~IHttpHeader() {}
};


struct IHttpFormdata {
	virtual BOOL GetHeader(const std::string& name, IHttpHeader **ppheader = NULL) = 0;
	virtual BOOL GetFirstHeader(IHttpHeader **ppfirst = NULL) = 0;
	virtual BOOL GetNextHeader(IHttpHeader **ppnext = NULL) = 0;

	virtual BOOL GetText(std::string& text) = 0;

	virtual VOID Clear() = 0;

	virtual ~IHttpFormdata() {}
};


struct IHttpMessage {
	virtual int GetMethod() = 0; // see HttpMethod enumeration.

	virtual BOOL GetVersion(int& first, int& second) = 0;
	virtual BOOL GetText(std::string& text) = 0;
	virtual BOOL GetUrl(std::string& url) = 0;
	virtual BOOL GetHeader(const std::string& name, IHttpHeader **ppheader = NULL) = 0;

	virtual BOOL GetFirstHeader(IHttpHeader **ppfirst = NULL) = 0;
	virtual BOOL GetNextHeader(IHttpHeader **ppnext = NULL) = 0;

	virtual BOOL GetFirstFormdata(IHttpFormdata& first)  = 0;
	virtual BOOL GetNextFormdata(IHttpFormdata& next)  = 0;

	virtual BOOL SetVersion(int first, int second) = 0;
	virtual BOOL SetText(const std::string& prefix, const std::string& suffix = "") = 0;
	virtual BOOL SetUrl(const std::string& url) = 0;
	virtual BOOL SetStatus(const std::string& status) = 0;
	virtual BOOL SetDescription(const std::string& description) = 0;
	virtual BOOL SetContentType(const std::string& url, const std::string& encoding = "") = 0;
	virtual BOOL SetHeader(const std::string& name, const IHttpHeaderValue& value, BOOL replace = FALSE) = 0; // If header exists, it adds the value, unless replace is TRUE.
	virtual BOOL RemoveHeader(const std::string& name) = 0; 

	virtual IHttpMessage& GetHttpResponse(BOOL doclear = TRUE) = 0;
	virtual IHttpHeaderValue& GetWorkerHeaderValue(BOOL doclear = TRUE) = 0;
	virtual IHttpFormdata& GetWorkerFormdata(BOOL doclear = TRUE)  = 0;
	virtual VOID Clear() = 0;

	virtual BOOL InvokeMarshallerOnPayload(IHttpMessage& response, ISTREAMReader& reader, ISTREAMWriter *writer = NULL, const std::string& outstr_prefix = "") = 0;
	virtual BOOL InvokeMarshaller(IHttpMessage& response, ISTREAMReader& reader, const std::string& inpstr, ISTREAMWriter *writer = NULL, const std::string& outstr_prefix = "") = 0;

	virtual ISTREAMWriter* GetXmlWriter() = 0; 

	virtual ~IHttpMessage() {}
};


EXP IHttpMessage* CreateHttpRequest(int method = HttpUnknown);





#ifdef httpmessage_internal_use_only




struct HttpHeaderValue: public IHttpHeaderValue {
	// Contains name=value pairs. Name can be empty.
	// The end result that the HttpWriter will produce looks like: name1=value1, value2, name3=value3
	// Multiple HttpHeaderValue objects produce result like: name1=value1, value2; name4=value4, name6=value6
	std::vector<std::string> _values;
	std::vector<std::string> _names;

	// For each pair from (_values, _names) it contains name=value, or just value if name is empty.
	// This entry is used for output.
	std::vector<std::string> _mergedvalues;

	int _values_pos;

	HttpHeaderValue(const std::string& value = "", const std::string& name = "");
	~HttpHeaderValue();

	// Adds only to _mergedvalues.
	// This function gives better performance.
	BOOL OutputPair(const std::string& value, const std::string& name = "");

	// Adds to _mergedvalues and (_values, _names).
	// Returns TRUE if _values.size() equals _names.size().
	// It will be TRUE if only this function is used.
	BOOL AddPair(const std::string& value, const std::string& name = "");

	VOID Clear();

	BOOL GetFirstPair(std::string& value, std::string& name);
	BOOL GetNextPair(std::string& value, std::string& name);
};


BEGIN_WSI_SERIALIZATION_OBJECT(HttpHeaderValue)
CONTAINS_FLAT_MEMBER(_values, Value)
CONTAINS_FLAT_MEMBER(_names, Name)
CONTAINS_FLAT_MEMBER(_mergedvalues, Output)
END_WSI_SERIALIZATION_OBJECT()

//AUTOCREATE_WSI_SERIALIZATION_OBJECT(HttpHeaderValue)



struct HttpHeader: public IHttpHeader {
	std::string _header_name;
	std::vector<HttpHeaderValue> _header_values;

	HttpHeader(const std::string& name = "");
	~HttpHeader();

	int _header_values_pos;

	BOOL FindValue(const std::string& value, const std::string& name = "", int *pIndex = 0, int *pPairIndex = 0);
	BOOL GetValue(std::string& value, int index = 0, int pair_index = 0);

	BOOL GetFirstValue(IHttpHeaderValue **ppfirst = NULL);
	BOOL GetNextValue(IHttpHeaderValue **ppnext = NULL);
};


BEGIN_WSI_SERIALIZATION_OBJECT(HttpHeader)

CONTAINS_FLAT_MEMBER(_header_name, HeaderName)
CONTAINS_OBJECT_MEMBER(_header_values, HeaderValue)
END_WSI_SERIALIZATION_OBJECT()

//AUTOCREATE_WSI_SERIALIZATION_OBJECT(HttpHeader)



struct HttpPayload {
	std::payload_string _text;
};

BEGIN_WSI_SERIALIZATION_OBJECT(HttpPayload)

CONTAINS_FLAT_MEMBER(_text, Text)
END_WSI_SERIALIZATION_OBJECT()




struct HttpFormdata: public IHttpFormdata {
	std::vector<HttpHeader> _headers;
	HttpPayload _payload;

	int _headers_pos;

	BOOL GetHeader(const std::string& name, IHttpHeader **ppheader = NULL);
	BOOL GetFirstHeader(IHttpHeader **ppfirst = NULL);
	BOOL GetNextHeader(IHttpHeader **ppnext = NULL);

	BOOL GetText(std::string& text);

	VOID Clear();

	HttpFormdata();
	~HttpFormdata() {}
};


BEGIN_WSI_SERIALIZATION_OBJECT(HttpFormdata)

CONTAINS_OBJECT_MEMBER(_headers, Header)
CONTAINS_OBJECT_MEMBER(_payload, Payload)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(HttpFormdata)

template<> APIEXP IWsiSerializerBase& GetRootSerializer<IHttpFormdata>(const IHttpFormdata *p) { return WSI_NAMESPACE::__wsiRootHttpFormdata; }



struct HttpMessage: public IHttpMessage {
	std::string _method;
	std::string _url;
	std::string _version;
	std::string _outputversion;
	std::string _status_code;
	std::string _status_description;
	std::vector<HttpHeader> _headers;
	HttpPayload _payload;

	HttpFormdata _formdata;	
	HttpMessage *_response;
	HttpHeaderValue _workerheadervalue; 

	CXMLWriter _xmlwriter; 

	int _headers_pos;
	int _formdata_pos;

	std::string _boundary;

	HttpMessage(int method = HttpUnknown);
	~HttpMessage();

	HttpMessage& operator = (const HttpMessage& other);

	int GetMethod();

	BOOL GetVersion(int& first, int& second);
	BOOL GetText(std::string& text);
	BOOL GetUrl(std::string& url);
	BOOL GetHeader(const std::string& name, IHttpHeader **ppheader = NULL);

	BOOL GetFirstHeader(IHttpHeader **ppfirst = NULL);
	BOOL GetNextHeader(IHttpHeader **ppnext = NULL);

	BOOL GetFirstFormdata(IHttpFormdata& first);
	BOOL GetNextFormdata(IHttpFormdata& next);

	BOOL SetVersion(int first, int second);
	BOOL SetText(const std::string& prefix, const std::string& suffix = "");
	BOOL SetUrl(const std::string& url);
	BOOL SetStatus(const std::string& status);
	BOOL SetDescription(const std::string& description);
	BOOL SetContentType(const std::string& url, const std::string& encoding = "");
	BOOL SetHeader(const std::string& name, const IHttpHeaderValue& value, BOOL replace = FALSE);
	BOOL RemoveHeader(const std::string& name); 

	IHttpMessage& GetHttpResponse(BOOL doclear = TRUE);
	IHttpHeaderValue& GetWorkerHeaderValue(BOOL doclear = TRUE);
	IHttpFormdata& GetWorkerFormdata(BOOL doclear = TRUE);
	VOID Clear();

	BOOL InvokeMarshallerOnPayload(IHttpMessage& response, ISTREAMReader& reader, ISTREAMWriter *writer = NULL, const std::string& prefix = "");
	BOOL InvokeMarshaller(IHttpMessage& response, ISTREAMReader& reader, const std::string& inpstr, ISTREAMWriter *writer = NULL, const std::string& prefix = "");

	ISTREAMWriter* GetXmlWriter(); 
};




BEGIN_WSI_SERIALIZATION_OBJECT(HttpMessage)

CONTAINS_FLAT_MEMBER(_method, Method)
CONTAINS_FLAT_MEMBER(_url, Url)
CONTAINS_FLAT_MEMBER(_version, Version)
CONTAINS_FLAT_MEMBER(_outputversion, OutputVersion)
CONTAINS_FLAT_MEMBER(_status_code, Status)
CONTAINS_FLAT_MEMBER(_status_description, Description)
CONTAINS_OBJECT_MEMBER(_headers, Header)
CONTAINS_OBJECT_MEMBER(_payload, Payload)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(HttpMessage)

template<> APIEXP IWsiSerializerBase& GetRootSerializer<IHttpMessage>(const IHttpMessage *p) { return WSI_NAMESPACE::__wsiRootHttpMessage; }

#else //httpmessage_internal_use_only

#if defined(_UCC)

#ifndef APIEXP
#define APIEXP EXP
#endif

template<> APIEXP IWsiSerializerBase& GetRootSerializer<IHttpMessage>(const IHttpMessage *p);

#endif


#endif //httpmessage_internal_use_only



#endif //httpmessageH

