// Created by Alex Nikol Bourakov (ANB) around April/07


#ifndef wsiwriterH
#define wsiwriterH


#include ".\..\Include\WSIClassFactoryInterfaces.h"


class EXP CWSIWriter: public ISTREAMWriter {
	const char *_name;
	bool _active_brackets;
	std::stringstream _content;
	std::string _strcontent;
	void AddContent();

	bool _format_humanreadable;
	int _version;

public:
	CWSIWriter();
	CWSIWriter(const CWSIWriter&);
	CWSIWriter& operator = (const CWSIWriter&);
	~CWSIWriter();

	const char * GetName();
	bool IsHumanReadableFormat();
	int Encoding();
	void SetHumanReadableFormat(bool value);
	void SetVersion(int version);
	int Version();
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



class EXP CRawWriter: public ISTREAMWriter {
	const char *_name; 
	std::string _content; 

public:
	CRawWriter();
	CRawWriter(const CRawWriter&);
	CRawWriter& operator = (const CRawWriter&);
	~CRawWriter();

	const char * GetName();

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


#endif //wsiwriterH
