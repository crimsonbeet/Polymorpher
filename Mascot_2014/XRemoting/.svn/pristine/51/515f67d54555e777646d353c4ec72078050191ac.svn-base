// Created by Alex Bourakov (ANB) around April/11


#include "stdafx_classfactory.h"
#include "XMLWriter.h"

CXMLWriter::CXMLWriter(): _name("XMLWriter") {
}

CXMLWriter::CXMLWriter(const CXMLWriter& x) {
	_name = x._name;
}

CXMLWriter& CXMLWriter::operator = (const CXMLWriter&) {
	return *this;
}

CXMLWriter::~CXMLWriter() {
}

void CXMLWriter::AddContent() {
	_strcontent += _content.str();
	_content.str("");
}

int CXMLWriter::Encoding() { 
	return 2; 
} 

bool CXMLWriter::PutAll(bool iterate) { 
	return iterate; 
} 

void CXMLWriter::Initialize(const std::string& s) {
	_strcontent = s;
}

void CXMLWriter::ToString(std::string& s) {
	//s = _strcontent;
	_strcontent.swap(s); 
	Reset();
}

void CXMLWriter::Reset() {
	_strcontent.resize(0);
}

const char * CXMLWriter::GetName() {
	return _name;
}

long CXMLWriter::GetPos() {
	return (long)_strcontent.size();
}

void CXMLWriter::SetPos(long pos) {
	_strcontent.resize(pos);
}

void CXMLWriter::BeginObject(const std::string& id, int index, bool is_composite) {
	_content << '<' << id << '>';
	AddContent();
}

void CXMLWriter::AddAttribute(const std::string& id, const std::string& value, int index, bool out_of_order) {
	if(value.size()) { 
		_content << '<' << id << '>' << value << '<' << '/' << id << '>';
	} 
	else { 
		_content << '<' << id << '/' << '>';
	} 
	AddContent();
}

void CXMLWriter::EndIterator(const std::string&, int) {
}

void CXMLWriter::EndObject(const std::string& id, int, bool, int) {
	_content << '<' << '/' << id << '>';
	AddContent();
}

void CXMLWriter::Append(const std::string& str) {
	_strcontent += str;
}

void CXMLWriter::Append(char c) {
	_strcontent += c;
}

