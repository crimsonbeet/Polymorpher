
#include "stdafx_classfactory.h"
#include "DartParser.h"
#include "DartWriter.h"
#include "WSIObject.h"



char *_dart_envelopestructure_name = "SDartEnvelope"; 
short _dart_envelopestructure_namesize = strlen(_dart_envelopestructure_name); 

char *_dart_connectstructure_name = "SDartConnect"; 
short _dart_connectstructure_namesize = strlen(_dart_connectstructure_name); 

char *_dart_packetstructure_name = "SDartPacket"; 
short _dart_packetstructure_namesize = strlen(_dart_packetstructure_name); 

char *_dart_apacketstructure_name = "SDartAPacket"; 
short _dart_apacketstructure_namesize = strlen(_dart_apacketstructure_name); 

char *_dart_datastructure_name = "SDartData"; 
short _dart_datastructure_namesize = strlen(_dart_datastructure_name); 

char *_dartrtls_unknownstructure_name = "SRtlsUnknown"; 
short _dartrtls_unknownstructure_namesize = strlen(_dartrtls_unknownstructure_name); 

char *_dartrtls_presencestructure_name = "SRtlsPresence"; 
short _dartrtls_presencestructure_namesize = strlen(_dartrtls_presencestructure_name); 

char *_dartrtls_2d_calculationstructure_name = "SRtls2DLocation"; 
short _dartrtls_2d_calculationstructure_namesize = strlen(_dartrtls_2d_calculationstructure_name); 

char *_dartrtls_2d_estimatestructure_name = "SRtls2DEstimate"; 
short _dartrtls_2d_estimatestructure_namesize = strlen(_dartrtls_2d_estimatestructure_name); 

char *_dartrtls_3d_calculationstructure_name = "SRtls3DLocation"; 
short _dartrtls_3d_calculationstructure_namesize = strlen(_dartrtls_3d_calculationstructure_name); 

char *_dartrtls_diagnosticstructure_name = "SRtlsDiagnostic"; 
short _dartrtls_diagnosticstructure_namesize = strlen(_dartrtls_diagnosticstructure_name); 

char *_dartrtls_subscribestructure_name = "SRtlsSubscribe"; 
short _dartrtls_subscribestructure_namesize = strlen(_dartrtls_subscribestructure_name); 

char *_dartrtls_unsubscribestructure_name = "SRtlsUnsubscribe"; 
short _dartrtls_unsubscribestructure_namesize = strlen(_dartrtls_unsubscribestructure_name); 


char *_dart_field_names[10] = {"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA"}; 
short _dart_names_sizes[10] = {2,    2,    2,    2,    2,    2,    2,    2,    2,    2}; 



CDartParser::delegate_validator_type *CDartParser::_s_validator = 0; 


cached_work_objects<wsi_object > _s_temp_wsi_objects; 


void CDartParser::SetValidator(delegate_validator_type *validator) { 
	_s_validator = validator; 
} 

void CDartParser::parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer& mat) const {
	const std::string wsi(from, to);
	parse(wsi, mat);
}
void CDartParser::parse(const std::string& wsi, IWSIMaterializer& mat) const {
	wsi_object& obj = *_s_temp_wsi_objects.get_object(); 

	size_t obj_count = 0; 
	size_t ncount = wsi.size(); 

	size_t x = 0; 
	while(x < ncount) { 
		obj._vecCaptures.resize(0); 
		obj._vecCaptures.reserve(10); 

		while(x < ncount && wsi[x] < 0x20) { 
			++x; 
		} 

		size_t y = 0; 
		size_t z = x; 

		while(y < ARRAY_NUM_ELEMENTS(_dart_field_names) && x < ncount) { 
			size_t s = obj._vecCaptures.size(); 
			obj._vecCaptures.resize(s + 1); 
			obj._vecCaptures[s].first._begin = _dart_field_names[y]; 
			obj._vecCaptures[s].first._end = obj._vecCaptures[s].first._begin + _dart_names_sizes[y]; 
			++y; 

			obj._vecCaptures[s].second._begin = &wsi[x]; 
			while(x < ncount && wsi[x] >= 0x20 && wsi[x] != ',') { 
				++x; 
			} 
			obj._vecCaptures[s].second._end = &wsi[x]; 

			if(x < ncount && wsi[x++] < 0x20) { 
				break; 
			} 
		} 

		if(y < 3) { 
			break; // insufficient data. 
		} 

		char *packet_name = _dartrtls_unknownstructure_name; 
		short packet_namesize = _dartrtls_unknownstructure_namesize; 
		const substring_locator *tag_id2validate = 0; 

		switch(wsi[z]) { 
		case (int)'S': 
			packet_name = _dartrtls_subscribestructure_name; 
			packet_namesize = _dartrtls_subscribestructure_namesize; 
			break; 
		case (int)'U': 
			packet_name = _dartrtls_unsubscribestructure_name; 
			packet_namesize = _dartrtls_unsubscribestructure_namesize; 
			break; 
		case (int)'R': 
			packet_name = _dartrtls_3d_calculationstructure_name; 
			packet_namesize = _dartrtls_3d_calculationstructure_namesize; 
			tag_id2validate = &obj._vecCaptures[1].second; 
			break; 
		case (int)'T': 
			packet_name = _dartrtls_2d_calculationstructure_name; 
			packet_namesize = _dartrtls_2d_calculationstructure_namesize; 
			tag_id2validate = &obj._vecCaptures[1].second; 
			break; 
		case (int)'O': 
			packet_name = _dartrtls_2d_estimatestructure_name; 
			packet_namesize = _dartrtls_2d_estimatestructure_namesize; 
			tag_id2validate = &obj._vecCaptures[1].second; 
			break; 
		case (int)'P': 
			packet_name = _dartrtls_presencestructure_name; 
			packet_namesize = _dartrtls_presencestructure_namesize; 
			tag_id2validate = &obj._vecCaptures[1].second; 
			break; 
		case (int)'D': 
			packet_name = _dartrtls_diagnosticstructure_name; 
			packet_namesize = _dartrtls_diagnosticstructure_namesize; 
			break; 
		case (int)'a': 
			packet_name = _dart_apacketstructure_name; 
			packet_namesize = _dart_apacketstructure_namesize; 
			if(y > 3) { 
				tag_id2validate = &obj._vecCaptures[4].second; 
			} 
			break; 
		case (int)'p': 
			packet_name = _dart_packetstructure_name; 
			packet_namesize = _dart_packetstructure_namesize; 
			tag_id2validate = &obj._vecCaptures[1].second; 
			break; 
		case (int)'d': 
			packet_name = _dart_datastructure_name; 
			packet_namesize = _dart_datastructure_namesize; 
			break; 
		} 

		obj._header.first._begin = packet_name;
		obj._header.first._end = obj._header.first._begin + packet_namesize;

		if(tag_id2validate) { 
			if(_s_validator) { 
				if(!_s_validator(*tag_id2validate, obj, x >= ncount)) { 
					continue; 
				} 
			} 
		} 

		if(x < ncount && !obj_count) { // more objects to follow, so create a composite if it has not been created yet. 
			wsi_object obj; 
			obj._header.first._begin = _dart_envelopestructure_name; 
			obj._header.first._end = obj._header.first._begin  + _dart_envelopestructure_namesize; 

			mat.place_composite_object_on_stack(IWSIObject(&obj)); 
			++obj_count; 
		} 

		mat.process_flat_object(IWSIObject(&obj));
		++obj_count; 
	} 

	mat.reduce_stack(); 

	_s_temp_wsi_objects.return_object(&obj); 
}

ISTREAMWriter * CDartParser::create_writer() const {
	return new CDartWriter();
}






BYTE _dart_packet_field_sizes[3] = {1, 8, 2}; 
BYTE _dart_packet_field_padding[3] = {'0', '0', '0'}; 

BYTE _dart_apacket_field_sizes[6] = {1, 4, 8, 2, 8, 2}; 
BYTE _dart_apacket_field_padding[6] = {'0', '0', '0', '0', '0', '0'}; 

BYTE _dart_data_field_sizes[3] = {1, 3, 2}; 
BYTE _dart_data_field_padding[3] = {'0', '0', '0'}; 

BYTE _dartrtls_packet_field_sizes[10] = {1, 8, 8, 8, 8, 2, 16, 2, 4, 2}; 
BYTE _dartrtls_packet_field_padding[10] = {'0', '0', 0, 0, '0', '0', 0, '0', '0', '0'}; 



void CDartWriter::BeginObject(const std::string& id, int index, bool is_composite) {
	static std::string s_dart_packetstructure_name(_dart_packetstructure_name, _dart_packetstructure_namesize); 
	static std::string s_dart_apacketstructure_name(_dart_apacketstructure_name, _dart_apacketstructure_namesize); 
	static std::string s_dart_datastructure_name(_dart_datastructure_name, _dart_datastructure_namesize); 
	_max_count = 0; 
	_count = 0; 
	if(id == s_dart_packetstructure_name) { 
		_field_sizes = _dart_packet_field_sizes; 
		_field_padding = _dart_packet_field_padding; 
		_max_count = 3; 
	} 
	else 
	if(id == s_dart_apacketstructure_name) { 
		_field_sizes = _dart_apacket_field_sizes; 
		_field_padding = _dart_apacket_field_padding; 
		_max_count = 6; 
	} 
	else 
	if(id == s_dart_datastructure_name) { 
		_field_sizes = _dart_data_field_sizes; 
		_field_padding = _dart_data_field_padding; 
		_max_count = 3; 
	} 
	else { 
		_field_sizes = _dartrtls_packet_field_sizes; 
		_field_padding = _dartrtls_packet_field_padding; 
		_max_count = 10; 
	} 
	_content.reserve(64); 
}

void CDartWriter::AddAttribute(const std::string& id, const std::string& value, int index, bool out_of_order) {
	if(_count >= _max_count) { 
		return; 
	} 
	if(_count > 0) { 
		_content += ','; 
	} 
	size_t size = _field_sizes[_count]; 
	while(size > value.size() && _field_padding[_count]) { 
		_content += _field_padding[_count]; 
		--size; 
	} 
	if(value.size()) { 
		_content += value; 
	} 
	_count++; 
} 

void CDartWriter::EndIterator(const std::string&, int) {
}

void CDartWriter::EndObject(const std::string& id, int, bool is_composite, int) {
	if(_content.size()) { 
		if(!is_composite) { 
			_content += 0xa; 
		} 
	} 
}

