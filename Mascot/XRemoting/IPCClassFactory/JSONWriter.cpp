/*
 * JSONWriter.cpp; Created on: Mar 20, 2013; Author: alex bourakov
 *
 */


#include "stdafx_classfactory.h"
#include "JSONWriter.h"

CJSONWriter::CJSONWriter(): _name("JSONWriter") {
}

CJSONWriter::CJSONWriter(const CJSONWriter& x) {
	_name = x._name;
}

CJSONWriter& CJSONWriter::operator = (const CJSONWriter&) {
	return *this;
}

CJSONWriter::~CJSONWriter() {
}

void CJSONWriter::AddContent() {
	_strcontent += _content.str();
	_content.str("");
}

int CJSONWriter::Encoding() {
	return 4;
}

bool CJSONWriter::PutAll(bool iterate) { 
	return iterate; 
} 

void CJSONWriter::Initialize(const std::string& s) {
	_strcontent = s;
}

void CJSONWriter::ToString(std::string& s) {
	_strcontent.swap(s); 
	Reset();
}

void CJSONWriter::Reset() {
	_strcontent.resize(0);
}

const char * CJSONWriter::GetName() {
	return _name;
}

long CJSONWriter::GetPos() {
	return (long)_strcontent.size();
}

void CJSONWriter::SetPos(long pos) {
	_strcontent.resize(pos);
}

void CJSONWriter::BeginFile(const std::string&) {
	_content << '{';

	AddContent();
}

void CJSONWriter::BeginProperty(const std::string& id) {
	if(_strcontent.c_str()[_strcontent.size() - 1] != '{') {
		_content << ',';
	}
	_content << id << ':';
}

void CJSONWriter::BeginObject(const std::string& id, int index, bool /*is_composite*/) {
	if(index <= 1) {
		BeginProperty(id);
	}
	else {
		_content << ',';
	}

	if(index == 1) {
		_content << '[';
	}

	_content << '{';

	AddContent();
}

void CJSONWriter::AddAttribute(const std::string& id, const std::string& value, int index, bool /*out_of_order*/) {
	bool do_output = value.size() || index;

	if(index <= 1) {
		if(do_output) {
			BeginProperty(id);
		} 
	}
	else {
		_content << ',';
	}

	if(index == 1) {
		_content << '[';
	}

	if(do_output) {
		_content << '"' << value << '"';
	}
	AddContent();
}

void CJSONWriter::EndIterator(const std::string&, int elem_count) { 
	if(elem_count >= 1) {
		_content << ']';
	} 
} 

void CJSONWriter::EndObject(const std::string&, int, bool, int) {
	_content << '}'; 
	AddContent();
}

void CJSONWriter::EndFile(const std::string&) {
	_content << '}';
	AddContent();
}

void CJSONWriter::Append(const std::string& str) {
}

void CJSONWriter::Append(char c) {
}

