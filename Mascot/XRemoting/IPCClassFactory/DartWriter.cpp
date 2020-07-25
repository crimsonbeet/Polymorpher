// Created by Alex Nikol Bourakov (ANB) around April/07


#include "stdafx_classfactory.h"
#include "DartWriter.h"

CDartWriter::CDartWriter(): _name("DartWriter") {
}

CDartWriter::CDartWriter(const CDartWriter& x) {
	_name = x._name;
}

CDartWriter& CDartWriter::operator = (const CDartWriter&) {
	return *this;
}

CDartWriter::~CDartWriter() {
}

void CDartWriter::Initialize(const std::string& s) {
	_content = s;
}

void CDartWriter::ToString(std::string& s) {
	_content.swap(s);
	Reset();
}

void CDartWriter::Reset() {
	_content.resize(0);
}

const char * CDartWriter::GetName() {
	return _name;
}

long CDartWriter::GetPos() {
	return (long)_content.size();
}

void CDartWriter::SetPos(long pos) {
	_content.resize(pos);
}
