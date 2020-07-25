// Created by Alex Nikol Bourakov (ANB) around April/07


#include "stdafx_classfactory.h"
#include "WSIWriter.h"

CWSIWriter::CWSIWriter(): _name("WSIWriter") {
	_active_brackets = false;
	_format_humanreadable = false;
	_version = XXVERSION;
}

CWSIWriter::CWSIWriter(const CWSIWriter& x) {
	this->_name = x._name;
	this->_active_brackets = false;
	this->_format_humanreadable = false;
	this->_version = XXVERSION;
}

CWSIWriter& CWSIWriter::operator = (const CWSIWriter&) {
	return *this;
}

CWSIWriter::~CWSIWriter() {
}

void CWSIWriter::AddContent() {
	_strcontent += _content.str();
	_content.str("");
}


bool CWSIWriter::IsHumanReadableFormat() {
	return _format_humanreadable;
}

int CWSIWriter::Encoding() { 
	return 1;
} 

void CWSIWriter::SetHumanReadableFormat(bool value) {
	_format_humanreadable = value;
}

void CWSIWriter::SetVersion(int version) {
	_version = version;
	if(_version == 0) {
		_format_humanreadable = true;
	}
}

int CWSIWriter::Version() {
	return _version;
}

bool CWSIWriter::PutAll(bool) { 
	return false; 
} 

void CWSIWriter::Initialize(const std::string& s) {
	_strcontent = s;
}

void CWSIWriter::ToString(std::string& s) {
//	s = _strcontent;
	_strcontent.swap(s);
	Reset();
}

void CWSIWriter::Reset() {
	_strcontent.resize(0);
}

const char * CWSIWriter::GetName() {
	return _name;
}

long CWSIWriter::GetPos() {
	return (long)_strcontent.size();
}

void CWSIWriter::SetPos(long pos) {
	_strcontent.resize(pos);
}

void CWSIWriter::BeginObject(const std::string& id, int index, bool is_composite) {
	_content << "><" << (is_composite? '$': ' ') << id;
	if(index) {
		_content << '@' << index;
	}
	_content  << ' ';

	AddContent();
}

void CWSIWriter::AddAttribute(const std::string& id, const std::string& value, int index, bool out_of_order) {
//	conditions:
//	(index == 0 && out_of_order == false) --> a single value; no brackets.
//	(index == 0 && out_of_order == true ) --> error.
//	(index >  0 && out_of_order == false) --> next element of multiple values; do put Id then do open brackets, if brackets not opened yet.
//	(index >  0 && out_of_order == true ) --> jump to an element of multiple values; do close brackets if opened, put Id@idx and open brackets.
	if(value.size()) {
		if(index) {
			switch(_version) {
				case 0:
					_content << id;
					if(out_of_order) _content << '@' << index;
					_content << '=';
					break;
				default:
					if(out_of_order) {
						EndIterator(std::string(), 0);
					}
					if(!_active_brackets) {
						_content << std::endl << id;
						if(out_of_order) _content << '@' << index;
						_content << '=' << '{';

						_active_brackets = true;
					}
					break;
			}
		}
		else {
			_content << id << '=';
		}
		_content << value << ' ';
	}

	AddContent();
}

void CWSIWriter::EndIterator(const std::string&, int) {
	if(_active_brackets) {
		_strcontent += '}';
		_strcontent += ' ';
	}
	_active_brackets = false;
}

void CWSIWriter::EndObject(const std::string& id, int, bool is_composite, int nElements) {
	if(is_composite) {
		_content << std::endl << '$' << nElements << ' ' << std::endl;
	}
	AddContent();
}

void CWSIWriter::Append(const std::string& str) {
	_strcontent += str;
}

void CWSIWriter::Append(char c) {
	_strcontent += c;
}





CRawWriter::CRawWriter(): _name("RawWriter") {
}

CRawWriter::CRawWriter(const CRawWriter& x) {
	_name = x._name;
}

CRawWriter& CRawWriter::operator = (const CRawWriter&) {
	return *this;
}

CRawWriter::~CRawWriter() {
}

void CRawWriter::Initialize(const std::string& s) {
	_content = s;
}

void CRawWriter::ToString(std::string& s) {
	_content.swap(s);
	Reset();
}

void CRawWriter::Reset() {
	_content.resize(0);
}

const char * CRawWriter::GetName() {
	return _name;
}

long CRawWriter::GetPos() {
	return (long)_content.size();
}

void CRawWriter::SetPos(long pos) {
	_content.resize(pos);
}

void CRawWriter::BeginObject(const std::string& id, int index, bool is_composite) {
}

void CRawWriter::AddAttribute(const std::string& id, const std::string& value, int index, bool out_of_order) {
	if(_content.size()) { 
		_content += ' '; 
	} 
	_content += value; 
} 

void CRawWriter::EndIterator(const std::string&, int) {
}

void CRawWriter::EndObject(const std::string&, int, bool, int) {
}

