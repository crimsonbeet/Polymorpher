// Created by Alex Nikol Bourakov (ANB) around April/07

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "stdafx_classfactory.h"
#include "WSISerializableStruct.h"


void IWSISerializable::SetHumanReadableFormat(bool value) {
	_writer.SetHumanReadableFormat(value);
}


std::string& IWSISerializable::Filename() {
	return _filename;
}

void IWSISerializable::Load() {
	std::string inp_str;
	ReadTextFile(_filename, inp_str);

	if(inp_str.size() == 0) {
		inp_str = " >< " + _psroot->_wsi_id;
	}

	if(inp_str.size()) {
		(*_marshaller)(_parser, inp_str);
	}
}


void IWSISerializable::Save() {
	std::string out_str;
	(*_psroot)(_writer, _psroot->get_inner_ptr(), out_str);

	WriteTextFile(_filename, out_str);
}


void IWSISerializable::Append(IWsiSerializerBase& s, void *obj) {
	std::string out_str;
	s(_writer, obj, out_str);
	AppendTextFile(_filename, out_str);
}


IWSISerializable::IWSISerializable() {
	_marshaller = 0;
	_psroot = 0;
}

void IWSISerializable::Init(IWsiSerializerBase *psroot, const std::string& filename) {
	_marshaller = CreateIWSIMarshaller();
	psroot->register_with(*_marshaller);
	_psroot = psroot;
	_filename = filename;
}

IWSISerializable::~IWSISerializable() {
	DestroyIWSIMarshaller(_marshaller);
}
