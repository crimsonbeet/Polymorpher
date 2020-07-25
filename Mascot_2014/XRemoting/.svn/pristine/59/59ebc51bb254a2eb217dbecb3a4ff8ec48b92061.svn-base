
#include "stdafx_classfactory.h"
#include "FepParser.h"
#include "FepWriter.h"
#include "WSIObject.h"


char *g_fep_header_field_names[6] = {"F1", "F2", "F3", "F4", "F5", "F6"};
WORD g_fep_header_field_sizes[6] = {4, 4, 2, 3, 1, 6};
BYTE g_fep_header_field_ids[6] = {0, 0, 0, 0, 0, 0};
BYTE g_fep_header_field_padding[6] = {'0', '0', '0', '0', ' ', ' '};


char *g_fep_mid002_field_names[7] = {"F7", "F8", "F9", "F10", "F11", "F12", "F13"};
WORD g_fep_mid002_field_sizes[7] = {4, 2, 25, 3, 19, 19, 19};
BYTE g_fep_mid002_field_ids[7] = {1, 2, 3, 4, 5, 6, 7};
BYTE g_fep_mid002_field_padding[7] = {'0', '0', ' ', ' ', ' ', ' ', ' '};

char *g_fep_mid004_field_names[2] = {"F7", "F8"};
WORD g_fep_mid004_field_sizes[2] = {4, 2};
BYTE g_fep_mid004_field_ids[2] = {0, 0};
BYTE g_fep_mid004_field_padding[2] = {'0', '0'};

char *g_fep_mid005_field_names[1] = {"F7"};
WORD g_fep_mid005_field_sizes[1] = {4};
BYTE g_fep_mid005_field_ids[1] = {0};
BYTE g_fep_mid005_field_padding[1] = {'0'};

char *g_fep_mid041_field_names[11] = {"F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17"};
WORD g_fep_mid041_field_sizes[11] = {14, 10, 19, 10, 6, 19, 10, 2, 2, 3, 19};
BYTE g_fep_mid041_field_ids[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
BYTE g_fep_mid041_field_padding[11] = {' ', '0', ' ', ' ', '0', ' ', '0', '0', '0', '0', ' '};



map_fep_entities_type g_map_fep_entities;
map_fep_mid2id_type g_map_fep_mid2id;

bool g_has_been_initialized = CFepParser::Initialize();


bool CFepParser::Initialize() {
	if(g_has_been_initialized) {
		return true;
	}
	g_has_been_initialized = true;

	SFepEntity aMid;

	// Mid001
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(1, "M001"));

	// Mid002
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid002_field_sizes), g_fep_mid002_field_names, g_fep_mid002_field_sizes, g_fep_mid002_field_ids, g_fep_mid002_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M002", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(2, "M002"));

	// Mid004
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid004_field_sizes), g_fep_mid004_field_names, g_fep_mid004_field_sizes, g_fep_mid004_field_ids, g_fep_mid004_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M004", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(4, "M004"));

	// Mid005
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid005_field_sizes), g_fep_mid005_field_names, g_fep_mid005_field_sizes, g_fep_mid005_field_ids, g_fep_mid005_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M005", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(5, "M005"));

	// Mid040
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(40, "M040"));

	// Mid041
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid041_field_sizes), g_fep_mid041_field_names, g_fep_mid041_field_sizes, g_fep_mid041_field_ids, g_fep_mid041_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M041", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(41, "M041"));

	// Mid042
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(42, "M042"));

	// Mid043
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(43, "M043"));

	return true;
}


void CFepParser::parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer& mat) const {
	const std::string wsi(from, to);
	parse(wsi, mat);
}
void CFepParser::parse(const std::string& data, IWSIMaterializer& mat) const {
	int mid = 0;
	sscanf(&data[g_fep_header_field_sizes[0]], "%04d", &mid);

	wsi_object obj;

	map_fep_mid2id_type::iterator it = g_map_fep_mid2id.find(mid);
	if(it != g_map_fep_mid2id.end()) {
		obj._header.first._begin = (*it).second.c_str();
		obj._header.first._end = obj._header.first._begin + (*it).second.size();
	}
	else {
		obj._header.first._begin = &data[g_fep_header_field_sizes[0]];
		obj._header.first._end = obj._header.first._begin + g_fep_header_field_sizes[1];
	}

	size_t y = 0;

	for(size_t x = 0; x < ARRAY_NUM_ELEMENTS(g_fep_header_field_sizes) && y < data.size(); ++x) {
		size_t s = obj._vecCaptures.size();
		obj._vecCaptures.resize(s + 1);
		obj._vecCaptures[s].first._begin = g_fep_header_field_names[x];
		obj._vecCaptures[s].first._end = g_fep_header_field_names[x] + strlen(g_fep_header_field_names[x]);
		obj._vecCaptures[s].second._begin = &data[y];
		obj._vecCaptures[s].second._end = &data[y] + g_fep_header_field_sizes[x];

		y += g_fep_header_field_sizes[x];
	}

	map_fep_entities_type::iterator iter = g_map_fep_entities.end();
	if(it != g_map_fep_mid2id.end()) {
		iter = g_map_fep_entities.find((*it).second.c_str());
	}

	if(iter != g_map_fep_entities.end()) {
		SFepEntity& entity = (*iter).second;

		for(size_t x = 0; x < entity._field_sizes.size() && y <= (data.size() - entity._field_sizes[x]); ++x) {
			size_t s = obj._vecCaptures.size();
			obj._vecCaptures.resize(s + 1);
			obj._vecCaptures[s].first._begin = entity._field_names[x];
			obj._vecCaptures[s].first._end = entity._field_names[x] + strlen(entity._field_names[x]);
			obj._vecCaptures[s].second._begin = &data[y];
			obj._vecCaptures[s].second._end = &data[y] + entity._field_sizes[x];
			if(entity._field_ids[x] != 0 && y <= (data.size() - entity._field_sizes[x] - 2)) {
				int id = 0;
				if(!sscanf(&data[y], "%02d", &id) || id != entity._field_ids[x]) { // bad id
					std::cout << "FepParser: id does not match; expected=" << entity._field_ids[x] << ' ' << "received=" << id << std::endl;
				}
				obj._vecCaptures[s].second._begin += 2;
				obj._vecCaptures[s].second._end += 2;
				y += 2;
			}

			y += entity._field_sizes[x];
		}
	}


	mat.process_flat_object(IWSIObject(&obj));
}

ISTREAMWriter * CFepParser::create_writer() const {
	return new CFepWriter();
}


char * CFepWriter::itoa(char *fmt, int x) {
	sprintf(_temp_buffer, fmt, x);
	return &_temp_buffer[0];
}


void CFepWriter::BeginObject(const std::string& id, int index, bool is_composite) {
	_entity.Copy(ARRAY_NUM_ELEMENTS(g_fep_header_field_sizes), g_fep_header_field_names, g_fep_header_field_sizes, g_fep_header_field_ids, g_fep_header_field_padding);

	map_fep_entities_type::iterator iter = g_map_fep_entities.find(id);
	if(iter != g_map_fep_entities.end()) {
		_entity.Append((*iter).second);
	}
	_count = 0;
}

void CFepWriter::AddAttribute(const std::string& id, const std::string& value, int index, bool out_of_order) {
	if(_count >= _entity._field_ids.size()) {
		return;
	}
	if(id != _entity._field_names[_count]) {
		return;
	}
	int field_id = _entity._field_ids[_count];
	if(field_id) {
		_content += itoa("%02d", field_id);
	}
	int is_signed = 0;
	size_t size = _entity._field_sizes[_count];
	if(size > value.size()) {
		if(_entity._field_padding[_count] == '0') {
			if(value.size() && (value[0] == '+' || value[0] == '-')) {
				is_signed = 1;
			}
			if(is_signed) {
				_content += value[0];
			}
			while(size > value.size()) {
				_content += _entity._field_padding[_count];
				--size;
			}
		}
	}
	if(size < value.size() || is_signed) {
		_content += value.substr(is_signed, size);
		size = 0;
	}
	else
	if(value.size()) {
		_content += value;
		size -= value.size();
	}

	while(size > 0) {
		_content += _entity._field_padding[_count];
		--size;
	}
	++_count;
}

void CFepWriter::EndIterator(const std::string&, int) {
}

void CFepWriter::EndObject(const std::string& id, int, bool is_composite, int) {
	if(_content.size() >= 20 && _count >= 6) {
		memcpy((PVOID)_content.data(), itoa("%04d", (int)_content.size()), 4);
	}
	else { 
		_content.resize(0); 
	} 
}

