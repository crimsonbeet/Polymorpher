

#ifndef dartwriterH
#define dartwriterH


#include ".\..\Include\WSIClassFactoryInterfaces.h"


class EXP CDartWriter: public ISTREAMWriter {
	const char *_name; 
	std::string _content; 

	BYTE *_field_sizes; 
	BYTE *_field_padding; 
	int _max_count; 
	int _count; 

public:
	CDartWriter();
	CDartWriter(const CDartWriter&);
	CDartWriter& operator = (const CDartWriter&);
	~CDartWriter();

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


#endif //dartwriterH
