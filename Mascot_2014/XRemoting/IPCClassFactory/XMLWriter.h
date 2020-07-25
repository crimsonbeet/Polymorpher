

#ifndef simplewriterH
#define simplewriterH


#include ".\..\Include\WSIClassFactoryInterfaces.h"


class EXP CXMLWriter: public ISTREAMWriter {
	const char *_name;
	std::stringstream _content;
	std::string _strcontent;
	void AddContent();

public:
	CXMLWriter();
	CXMLWriter(const CXMLWriter&);
	CXMLWriter& operator = (const CXMLWriter&);
	~CXMLWriter();

	const char * GetName();
	bool IsHumanReadableFormat() { return true; }
	int Encoding();
	bool PutAll(bool); 

	void BeginObject(const std::string& id, int index, bool is_composite);
	void AddAttribute(const std::string& id, const std::string& value, int index, bool out_of_order);

	void EndIterator(const std::string& id, int elements_count);
	void EndObject(const std::string& id, int index, bool is_composite, int nElements);

	void Append(const std::string& str);
	void Append(char c);

public:
	void Initialize(const std::string& s);

	void ToString(std::string& s);

	void Reset();

	long GetPos();

	void SetPos(long pos);

};


#endif //simplewriterH
