

#ifndef fepwriterH
#define fepwriterH


#include ".\..\Include\WSIClassFactoryInterfaces.h"



struct SFepEntity {
	std::vector<char*> _field_names;
	std::vector<WORD> _field_sizes;
	std::vector<BYTE> _field_ids;
	std::vector<BYTE> _field_padding;

	void Resize(size_t x) {
		_field_names.resize(x);
		_field_sizes.resize(x);
		_field_ids.resize(x);
		_field_padding.resize(x);
	}
	void Copy(size_t cnt, char *names[], WORD sizes[], BYTE ids[], BYTE padding[]) {
		Resize(cnt);

		memcpy(&_field_names[0], names, sizeof(names[0]) * cnt);
		memcpy(&_field_sizes[0], sizes, sizeof(sizes[0]) * cnt);
		memcpy(&_field_ids[0], ids, sizeof(ids[0]) * cnt);
		memcpy(&_field_padding[0], padding, sizeof(padding[0]) * cnt);
	}
	void Append(SFepEntity& other) {
		size_t cnt = other._field_sizes.size();
		size_t s = _field_names.size();
		Resize(s + cnt);

		PCHAR *names = &other._field_names[0];
		WORD *sizes = &other._field_sizes[0];
		BYTE *ids = &other._field_ids[0];
		BYTE *padding = &other._field_padding[0];

		memcpy(&_field_names[0] + s, names, sizeof(names[0]) * cnt);
		memcpy(&_field_sizes[0] + s, sizes, sizeof(sizes[0]) * cnt);
		memcpy(&_field_ids[0] + s, ids, sizeof(ids[0]) * cnt);
		memcpy(&_field_padding[0] + s, padding, sizeof(padding[0]) * cnt);
	}
};

typedef SFepEntity *PSFepEntity;


typedef std::map<std::string/*ID*/, SFepEntity> map_fep_entities_type;
typedef std::map<int/*MID*/, std::string/*ID*/> map_fep_mid2id_type;




class EXP CFepWriter: public ISTREAMWriter {
	const char *_name;
	std::string _content;
	size_t _count;
	SFepEntity _entity;

	char _temp_buffer[16];

	inline char *itoa(char *fmt, int x);

public:
	CFepWriter();
	CFepWriter(const CFepWriter&);
	CFepWriter& operator = (const CFepWriter&);
	~CFepWriter();

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


#endif //fepwriterH
