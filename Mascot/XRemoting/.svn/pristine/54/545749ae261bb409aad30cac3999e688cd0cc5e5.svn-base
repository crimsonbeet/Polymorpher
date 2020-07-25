// Created by Alex Bourakov (ANB) around April/11


#include "stdafx_classfactory.h"
#include "FepWriter.h"

CFepWriter::CFepWriter(): _name("FepWriter") {
}

CFepWriter::CFepWriter(const CFepWriter& x) {
	_name = x._name;
}

CFepWriter& CFepWriter::operator = (const CFepWriter&) {
	return *this;
}

CFepWriter::~CFepWriter() {
}

void CFepWriter::Initialize(const std::string& s) {
	_content = s;
}

void CFepWriter::ToString(std::string& s) {
	if(_content.size()) { 
		_content.resize(_content.size() + 1); 
	} 
	_content.swap(s);
	//s = _content; 
	//s.resize(_content.size() + 1); 
	Reset();
}

void CFepWriter::Reset() {
	_content.resize(0);
	_count = 0; 
}

const char * CFepWriter::GetName() {
	return _name;
}

long CFepWriter::GetPos() {
	return (long)_content.size();
}

void CFepWriter::SetPos(long pos) {
	_content.resize(pos);
}

