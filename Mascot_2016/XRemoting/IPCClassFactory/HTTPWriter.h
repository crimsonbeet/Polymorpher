

#ifndef httpwriterH
#define httpwriterH


#include ".\..\Include\WSIClassFactoryInterfaces.h"


class EXP CHttpWriter: public ISTREAMWriter {
	const char *_name; 
	std::string _content; 

	int _current_object_type; // -1 - Undefined, 0 - HttpMessage, 1 - HttpHeader, 2 - HttpHeaderValue, 3 - HttpPayload
	int _attributes_count; 

public:
	CHttpWriter();
	CHttpWriter(const CHttpWriter&);
	CHttpWriter& operator = (const CHttpWriter&);
	~CHttpWriter();

	const char * GetName();
	bool PutAll(bool); 

	void BeginObject(const std::string& id, int index, bool is_composite);
	void AddAttribute(const std::string& id, const std::string& value, int index, bool out_of_order);

	void EndIterator(const std::string& id, int elements_count);
	void EndObject(const std::string& id, int index, bool is_composite, int nElements);

public:
	void Initialize(const std::string& s);

	void ToString(std::string& s);

	void Reset();

	long GetPos();

	void SetPos(long pos);

};


#endif //httpwriterH
