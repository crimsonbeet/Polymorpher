
#include "stdafx_classfactory.h"
#include "FepParser.h"
#include "FepWriter.h"
#include "WSIObject.h"

#include "WSIMaterializer.h"


char *g_fep_header_field_names[6] = {"F1", "F2", "F3", "F4", "F5", "F6"};
BYTE g_fep_header_field_ids[6] = {0, 0, 0, 0, 0, 0};
WORD g_fep_header_field_sizes[6] = {4, 4, 2, 3, 1, 6};
BYTE g_fep_header_field_padding[6] = {'0', '0', '0', '0', ' ', ' '};


char *g_fep_mid002_field_names[7] = {"F7", "F8", "F9", "F10", "F11", "F12", "F13"};
BYTE g_fep_mid002_field_ids[7] = {1, 2, 3, 4, 5, 6, 7};
WORD g_fep_mid002_field_sizes[7] = {4, 2, 25, 3, 19, 19, 19};
BYTE g_fep_mid002_field_padding[7] = {'0', '0', ' ', ' ', ' ', ' ', ' '};

char *g_fep_mid004_field_names[2] = {"F7", "F8"};
BYTE g_fep_mid004_field_ids[2] = {0, 0};
WORD g_fep_mid004_field_sizes[2] = {4, 2};
BYTE g_fep_mid004_field_padding[2] = {'0', '0'};

char *g_fep_mid005_field_names[1] = {"F7"};
BYTE g_fep_mid005_field_ids[1] = {0};
WORD g_fep_mid005_field_sizes[1] = {4};
BYTE g_fep_mid005_field_padding[1] = {'0'};

char *g_fep_mid041_field_names[11] = {"F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17"};
BYTE g_fep_mid041_field_ids[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
WORD g_fep_mid041_field_sizes[11] = {14, 10, 19, 10, 6, 19, 10, 2, 2, 3, 19};
BYTE g_fep_mid041_field_padding[11] = {' ', '0', ' ', ' ', '0', ' ', '0', '0', '0', '0', ' '};


char *g_fep_mid061_field_names[23] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"};
BYTE g_fep_mid061_field_ids[23] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
WORD g_fep_mid061_field_sizes[23] = {4, 2, 25, 25, 2, 3, 4, 4, 1, 1, 1, 6, 6, 6, 6, 5, 5, 5, 5, 19, 19, 1, 10};
BYTE g_fep_mid061_field_padding[23] = {'0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ', '0', ' '};

char *g_fep_mid061998_field_names[58] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58"};
BYTE g_fep_mid061998_field_ids[58] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58};
WORD g_fep_mid061998_field_sizes[58] = {4, 2, 25, 25, 4, 3, 2, 5, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 10, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 3, 3, 3, 6, 6, 6, 6, 6, 6, 10, 5, 5, 14, 19, 19, 25, 1, 2, 25, 25, 25, 4, 6, 10, 2, 2, 0};
BYTE g_fep_mid061998_field_padding[58] = {'0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ', ' ', ' ', '0', ' ', ' ', '0', ' ', ' ', '0', '0', '0', '0', ' '};

char *g_fep_mid064_field_names[1] = {"F7"};
BYTE g_fep_mid064_field_ids[1] = {0};
WORD g_fep_mid064_field_sizes[1] = {10};
BYTE g_fep_mid064_field_padding[1] = {'0'};

char *g_fep_mid065_field_names[23] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11"};
BYTE g_fep_mid065_field_ids[23] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
WORD g_fep_mid065_field_sizes[23]   = { 10,  25,   3,   4,   1,   1,   1,   6,   5,  19,   1};
BYTE g_fep_mid065_field_padding[23] = {'0', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0'};

char *g_fep_mid065006_field_names[36] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "48", "49", "50", "51", "52", "53", "54", "55"};
BYTE g_fep_mid065006_field_ids[36] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 48, 49, 50, 51, 52, 53, 54, 55};
WORD g_fep_mid065006_field_sizes[36]   = { 10,  25,   4,   3,   2,  10,   4,   4,   1,   1,   1,   1,   1,   1,   1,   1,   1,  10,   6,   5,   5,   3,   6,   6,   5,   5,  14,  19,   1,   2,  25,  25,  25,   4,   6,  10};
BYTE g_fep_mid065006_field_padding[36] = {'0', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', ' ', ' ', ' ', ' ', '0', ' '};



char *g_fep_mid105_field_names[1] = {"F7"};
BYTE g_fep_mid105_field_ids[1] = {0};
WORD g_fep_mid105_field_sizes[1] = {10};
BYTE g_fep_mid105_field_padding[1] = {'0'};

char *g_fep_mid106_field_names[23] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"};
BYTE g_fep_mid106_field_ids[23] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
WORD g_fep_mid106_field_sizes[23] = {2, 2, 10, 2, 20, 19, 2, 20, 1, 1, 40, 2, 2, 1, 1, 1, 7, 7, 7, 7, 7, 7, 0};
BYTE g_fep_mid106_field_padding[23] = {'0', '0', '0', '0', ' ', ' ', '0', ' ', '0', '0', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' '};

char *g_fep_mid107_field_names[15] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15"};
BYTE g_fep_mid107_field_ids[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
WORD g_fep_mid107_field_sizes[15] = {2, 2, 10, 2, 19, 4, 20, 20, 50, 4, 2, 3, 1, 0};
BYTE g_fep_mid107_field_padding[15] = {'0', '0', '0', '0', ' ', '0', ' ', ' ', ' ', ' ', '0', '0', '0', ' '};

char *g_fep_mid108_field_names[1] = {"F7"};
BYTE g_fep_mid108_field_ids[1] = {0};
WORD g_fep_mid108_field_sizes[1] = {1};
BYTE g_fep_mid108_field_padding[1] = {'0'};



map_fep_entities_type g_map_fep_entities;
map_fep_mid2id_type g_map_fep_mid2id;

bool g_has_been_initialized = CFepParser::Initialize();
int EXP CFepParser::_use_open_protocol = 0;


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



	// Mid060
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(60, "M060"));

	// Mid061
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid061_field_sizes), g_fep_mid061_field_names, g_fep_mid061_field_sizes, g_fep_mid061_field_ids, g_fep_mid061_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M061", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(61, "M061"));

	// Mid061-Rev.998
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid061998_field_sizes), g_fep_mid061998_field_names, g_fep_mid061998_field_sizes, g_fep_mid061998_field_ids, g_fep_mid061998_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M061R998", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(61998, "M061R998"));

	// Mid062
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(62, "M062"));

	// Mid064
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid064_field_sizes), g_fep_mid064_field_names, g_fep_mid064_field_sizes, g_fep_mid064_field_ids, g_fep_mid064_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M064", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(64, "M064"));

	// Mid065
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid065_field_sizes), g_fep_mid065_field_names, g_fep_mid065_field_sizes, g_fep_mid065_field_ids, g_fep_mid065_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M065", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(65, "M065"));

	// Mid065-Rev.6
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid065006_field_sizes), g_fep_mid065006_field_names, g_fep_mid065006_field_sizes, g_fep_mid065006_field_ids, g_fep_mid065006_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M065R6", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(65006, "M065R6"));


	// Mid105
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid105_field_sizes), g_fep_mid105_field_names, g_fep_mid105_field_sizes, g_fep_mid105_field_ids, g_fep_mid105_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M105", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(105, "M105"));

	// Mid106
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid106_field_sizes), g_fep_mid106_field_names, g_fep_mid106_field_sizes, g_fep_mid106_field_ids, g_fep_mid106_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M106", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(106, "M106"));

	// Mid107
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid107_field_sizes), g_fep_mid107_field_names, g_fep_mid107_field_sizes, g_fep_mid107_field_ids, g_fep_mid107_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M107", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(107, "M107"));

	// Mid108
	aMid.Copy(ARRAY_NUM_ELEMENTS(g_fep_mid108_field_sizes), g_fep_mid108_field_names, g_fep_mid108_field_sizes, g_fep_mid108_field_ids, g_fep_mid108_field_padding);
	g_map_fep_entities.insert(map_fep_entities_type::value_type("M108", aMid));
	g_map_fep_mid2id.insert(map_fep_mid2id_type::value_type(108, "M108"));

	return true;
}

void CFepParser::test(const std::string& telegram) {
	IWSIMarshaller& marshaller = *CreateIWSIMarshaller();
	marshaller(CFepParser(), telegram);
	DestroyIWSIMarshaller(&marshaller);
}

int CFepParser::set_useOpenProtocol(int val) {
	return _use_open_protocol = val; 
}


void CFepParser::parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer& mat) const {
	const std::string wsi(from, to);
	parse(wsi, mat);
}
void CFepParser::parse(const std::string& data, IWSIMaterializer& mat) const {
	int mid = 0;
	int spindle = 0;
	int revision = 0;
	int noack = 0; 
	int j = 0; 
	sscanf(&data[j += g_fep_header_field_sizes[0]], "%04d", &mid);
	sscanf(&data[j += g_fep_header_field_sizes[1]], "%02d", &spindle);
	sscanf(&data[j += g_fep_header_field_sizes[2]], "%03d", &revision);
	sscanf(&data[j += g_fep_header_field_sizes[3]], "%01d", &noack);
	if(_use_open_protocol) {
		noack = (revision - (revision / 100) * 100) / 10; 
		revision = revision / 100 + spindle * 10;
		spindle = 0;
	}

	switch(mid) {
	case 61: 
	if(revision > 1) {
		mid = 61998; 
	}
	break; 
	case 65:
	if(revision > 1) {
		mid = 65006;
	}
	break;
	}


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

	size_t obj_count = 0;
	int number_of_variables = 0;

	if(iter != g_map_fep_entities.end()) {
		SFepEntity& entity = (*iter).second;

		for(size_t x = 0; y < data.size() && x <= entity._field_sizes.size(); ++x) {
			int id = -1;
			int next_id = -1; 
			if(x < entity._field_sizes.size()) {
				next_id = entity._field_ids[x]; 
			}
			if(next_id > 0 || x == entity._field_sizes.size()) {
				if(!sscanf(&data[y], "%02d", &id)) {
					std::cout << "FepParser: bad id; expected=" << next_id << ' ' << "received=" << data[y] << data[y + 1] << std::endl;
				}
			}
			bool id_ordermismatch_found = false; 
			if(id > 0 && id != next_id) {
				x = std::find(entity._field_ids.begin(), entity._field_ids.end(), id) - entity._field_ids.begin(); 
				id_ordermismatch_found = true; 
			} 
			if(x < entity._field_sizes.size()) {
				id = entity._field_ids[x];

				size_t field_size = entity._field_sizes[x];
				if(y > (data.size() - field_size)) {
					break; 
				}

				bool create_subobject = false; 
				if(id_ordermismatch_found) {
					switch(mid) {
					case 106: 
						case 13:
						create_subobject = true; 
						break; 
					break; 
					}
				}

				if(create_subobject) {
					if(obj_count++) {
						mat.process_flat_object(IWSIObject(&obj));
					}
					else {
						mat.place_composite_object_on_stack(IWSIObject(&obj));
					}
					obj._header.first._begin = &data[y];
					obj._header.first._end = obj._header.first._begin + 2;
					obj._vecCaptures.resize(0); 
				}

				size_t s = obj._vecCaptures.size();
				obj._vecCaptures.resize(s + 1);
				obj._vecCaptures[s].first._begin = entity._field_names[x];
				obj._vecCaptures[s].first._end = entity._field_names[x] + strlen(entity._field_names[x]);
				obj._vecCaptures[s].second._begin = &data[y];
				if(field_size == 0) {
					field_size = data.size() - y; 
					if(id) {
						field_size -= 2;
					}
				}
				obj._vecCaptures[s].second._end = &data[y] + field_size;
				if(id != 0 && y <= (data.size() - field_size - 2)) {
					obj._vecCaptures[s].second._begin += 2;
					obj._vecCaptures[s].second._end += 2;
					y += 2;
				}

				switch(mid) {
				case 107:
					switch(id) {
					case 12:
					if(!sscanf(&data[y], "%02d", &number_of_variables)) {
						number_of_variables = -1;
					}
					break;
					case 13:
					if(!sscanf(&data[y], "%03d", &number_of_variables)) {
						number_of_variables = -1;
					}
					break;
					}
				break;
				}



				y += field_size;
				++s;



				int variables_length = 0; 

				switch(mid) {
				case 107:
					switch(id) {
					case 12:
					if(number_of_variables > 0) {
						obj._vecCaptures.resize(s + 1);
						obj._vecCaptures[s].first._begin = "BoltResults";
						variables_length = number_of_variables * 29; 
					}
					break;
					case 14:
					if(number_of_variables > 0) {
						obj._vecCaptures.resize(s + 1);
						obj._vecCaptures[s].first._begin = "StepResults";
						variables_length = number_of_variables * 31;
					}
					break;
					}
				break;
				}

				if(variables_length > 0) {
					obj._vecCaptures[s].first._end = obj._vecCaptures[s].first._begin + strlen(obj._vecCaptures[s].first._begin);
					obj._vecCaptures[s].second._begin = obj._vecCaptures[s].second._end = &data[y];
					if((y + variables_length) < data.size()) {
						obj._vecCaptures[s].second._end += variables_length;
					}
					y += variables_length;

					number_of_variables = 0; 
				}

			}
		}
	}

	if(obj_count) {
		mat.reduce_stack();
	}
	else {
		mat.process_flat_object(IWSIObject(&obj));
	}
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
	if((size && size < value.size()) || is_signed) {
		_content += value.substr(is_signed, size);
		size = 0;
	}
	else
	if(value.size()) {
		_content += value;
		if(size) {
			size -= value.size();
		}
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

