// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef wsiserializablestructH
#define wsiserializablestructH

#include "./ExportSTLMacro.h"

#include "./WSIClassFactory.h"
#include "./WSIParser.h" 

/*
 This file provides a programming interface related to Persistent Serializable Structures (supported by a text file). 
 */

class EXP IWSISerializable { 
protected: 
	IWSIMarshaller* _marshaller; 
	IWsiSerializerBase* _psroot; 

	CWSIParser _parser; 
	CWSIWriter _writer; 

	std::string _filename; 

public: 
	IWSISerializable(); 
	void Init(IWsiSerializerBase *psroot, const std::string& filename = std::string()); 
	virtual ~IWSISerializable(); 

public: 
	virtual std::string& Filename(); 
	virtual void Load(); 
	virtual void Save(); 

	virtual void Append(IWsiSerializerBase& s, void *obj); 

	void SetHumanReadableFormat(bool value); 
}; 

template <class T> 
class IWSISerializableStruct: public IWSISerializable { 
public: 
	T _root; 
	CWsiSerializer<T> _sroot; 

	T& Root() { 
		return _root; 
	} 

	IWSISerializableStruct(): _root(), _sroot(_root) { 
		Init(&_sroot); 
	} 

	IWSISerializableStruct(const IWSISerializableStruct<T>& x): _root(x._root), _sroot(_root) { 
		Init(&_sroot, x._filename); 
	} 
}; 


/*
 Here follows the definition of Persistent Serializable Vector of Structures. 
 This class is designed to support a structure of type T that contains a large container of other structures of type E. 
 When AppendElement(E&) method is called, the string image of E is appended to the file. 
 The layout of this class in the file becomes then like 
 String Image of T 
 String Image of E
 String Image of E
 . 
 . 
 . 

 Load method loads String Image of T first, 
 then for each String Image of E 
 it calls OnNewElement(E*) method of class T. 
 Thus, class T must have void OnNewElement(E*) declared. 

 Save method replaces the content of the file with just one 
 String Image of T. 
 Thus, the string image of T should contain all the string images of E. 

 Thus, the purpose of IWSISerializableVector is a mere optimization of Append operation, 
 when a large number of new items is expected to be added in short time, and 
 persistence must be kept at all time. 
 */ 

template <class T, typename E> 
class IWSISerializableVector: public IWSISerializableStruct<T> { 
	void MyInit() { 
		_callback.set_base(this); 
		_callback._wsi_id = _selement._wsi_id; 
		_selement.register_with(*(this->_marshaller)); 
		_callback.register_with(*(this->_marshaller)); 
	} 

public: 
	E _element; 
	CWsiSerializer<E> _selement; 

	void NewElement(E *obj) { 
		this->Root().OnNewElement(obj); 
	} 
	CWSIMemCallback<IWSISerializableVector<T, E >, E > _callback; 

	IWSISerializableVector(): 
	IWSISerializableStruct<T>(), _selement(_element), _callback(&IWSISerializableVector::NewElement) { 
			MyInit(); 
	} 

	IWSISerializableVector(const IWSISerializableVector<T, E>& x): 
	IWSISerializableStruct<T>(x), _selement(_element), _callback(&IWSISerializableVector::NewElement) { 
			MyInit(); 
	} 

	void AppendElement(const E& elem) { 
		this->Append(_selement, (void*)&elem); 
	} 
}; 


#endif //wsiserializablestructH



