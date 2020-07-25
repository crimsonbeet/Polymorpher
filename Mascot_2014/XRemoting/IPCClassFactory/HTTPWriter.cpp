// Created by Alex Nikol Bourakov (ANB) around April/07


#include "stdafx_classfactory.h"
#include "HttpWriter.h"

CHttpWriter::CHttpWriter(): _name("HttpWriter") {
}

CHttpWriter::CHttpWriter(const CHttpWriter& x) {
	_name = x._name;
}

CHttpWriter& CHttpWriter::operator = (const CHttpWriter&) {
	return *this;
}

CHttpWriter::~CHttpWriter() {
}

bool CHttpWriter::PutAll(bool) { 
	return false; 
} 

void CHttpWriter::Initialize(const std::string& s) {
	_content = s;
}

void CHttpWriter::ToString(std::string& s) {
	_content.swap(s);
	Reset();
}

void CHttpWriter::Reset() {
	_content.resize(0);
}

const char * CHttpWriter::GetName() {
	return _name;
}

long CHttpWriter::GetPos() {
	return (long)_content.size();
}

void CHttpWriter::SetPos(long pos) {
	_content.resize(pos);
	_current_object_type = -1;
}

