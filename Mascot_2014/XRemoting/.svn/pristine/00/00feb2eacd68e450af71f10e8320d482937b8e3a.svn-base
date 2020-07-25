/*
 * JSONWriter.h; Created on: Mar 20, 2013; Author: alex bourakov
 *
 */


#include ".\..\Include\WSIClassFactoryInterfaces.h"


#ifndef jsonwriterH
#define jsonwriterH


class EXP CJSONWriter: public ISTREAMWriter {
	const char *_name;
	std::stringstream _content;
	std::string _strcontent;
	void AddContent();
	void BeginProperty(const std::string& id);

public:
	CJSONWriter();
	CJSONWriter(const CJSONWriter&);
	CJSONWriter& operator = (const CJSONWriter&);
	~CJSONWriter();

	const char * GetName();
	bool IsHumanReadableFormat() { return true; }
	int Encoding();
	bool PutAll(bool); 

	void BeginFile(const std::string& id);
	void BeginObject(const std::string& id, int index, bool is_composite);
	void AddAttribute(const std::string& id, const std::string& value, int index, bool out_of_order);

	void EndIterator(const std::string& id, int elements_count);
	void EndObject(const std::string& id, int index, bool is_composite, int nElements);
	void EndFile(const std::string& id);

	void Append(const std::string& str);
	void Append(char c);

public:
	void Initialize(const std::string& s);

	void ToString(std::string& s);

	void Reset();

	long GetPos();

	void SetPos(long pos);

};


#endif //jsonwriterH
