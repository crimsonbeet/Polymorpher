/*****************************************************************************
Module:  WSIClassFactory.h
Created by ANB on April/16/07
******************************************************************************/

// -- Localizing behavior implies more effective maintenance --

//----------------------------------------------------------------------------

#ifndef wsiclassfactoryH
#define wsiclassfactoryH


#include ".\..\Include\WSIClassFactoryInterfaces.h"
#include ".\WSIObject.h"


#define WSI_MAX_VECTOR_SIZE (65536)


struct non_array_member_tag {};
struct vector_member_tag {};
struct array_member_tag {};



//	type-category detection (array or value-type?).

template <typename T> inline
non_array_member_tag Type_cat(T&) {
	return non_array_member_tag();
}

template <typename T, int S> inline
array_member_tag Type_cat(T (&)[S]) {
	return array_member_tag();
}

template <typename T> inline
vector_member_tag Type_cat(std::vector<T>&) {
	return vector_member_tag();
}

namespace std {
typedef basic_string<unsigned char> u8string;
typedef basic_string<signed char> payload_string;
};


//	function_type is a typeless interface to templatized serializer,
//	which is a specialized converter for each particular type.
//	a single int parameter represents an action to perform.
//	0 - reset, 1 - put, 2 - get, 3 - erase.
struct function_type1: public std::unary_function<int, bool> {
	virtual bool is(__int64) {
		return false;
	}
	virtual bool operator()(int) {
		return true;
	}
	inline bool action(int arg) {
		return this->operator()(arg);
	}
	virtual ~function_type1() {
	}
};



class CWsiSerializerBase; // forward declaration.

//	converter_tuple establishes an association between a member of a class and serializer.
struct EXP converter_tuple {
	substring_locator _xwsi_id;
	CWsiSerializerBase *_serializer;

	std::string _wsi; 
	const substring_locator *_pstr; 
	function_type1 *_functor; // ownership is not transferable.

	converter_tuple(): _functor(0), _pstr(0) {
	}
	~converter_tuple() {
		if(_functor) {
			delete _functor;
			_functor = 0;
		}
	}
};




//EXPORT_STL_MAP(std::string, converter_tuple)
//EXPORT_STL_VECTOR(converter_tuple*)



class EXP CWsiSerializerBase: public IWsiSerializerBase {
public:
	bool _is_composite;
	bool _is_late_binding;

	typedef std::map<substring_locator, converter_tuple> converter_map;
	typedef std::vector<converter_tuple*> converter_vector;

//	converter_map binds member-id and converter_tuple.
	converter_map *_pmap;

//	converter_vector stores pointers to converter_tuples sorted that the flat members coming first.
	converter_vector *_pvector;

	converter_tuple& add_converter_tuple(const char* const wsi_id, bool flat_member = false);


	template<typename M, typename T>
		static const char * find_member_id(M T::*p_mem) {
			return ((CWsiSerializerBase&)(GetRootSerializer((T*)0))).find_member_id_byoffset((__int64)&(((T*)0)->*p_mem));
		}
	const char * find_member_id_byoffset(__int64 member);


	ISTREAMWriter *_writer_ptr;

	char _encoding;
	char _human_readable_output;

protected:
	bool accept_a_member(const paired_captures& capture); /*accepts string image of a single field*/

	void accept_flat_object(const wsi_object& aInner);

	void accept_composite_object(const wsi_object& aComposite, std::vector<wsi_object>& ostack, size_t& pos);

	void remove_composite_from_stack(const wsi_object& aComposite, std::vector<wsi_object>& ostack, size_t& pos);


//	Main call_functor.
//	It works for non array member types.
public:
	//virtual bool call_functor(converter_tuple& t, int action);
	virtual bool call_functor(int action);


public:
	void LateBinding();

//	resets iterators of current binding.
	void reset_all_fundamental_types(__w64 int outer_offset);

//	erases all memebrs of current binding.
	void current_binding_erase_all(__w64 int outer_offset);


	void get_string_image(size_t idx = 0, bool can_be_empty = true);


//	Main public functions.
public:
	// This function converts top wsi_object to plain members of associated class.
	// It also converts to complex members the wsi_objects that follow.
	void operator()(const IWSIObject&, LONG_PTR rpc_handle = 0, IWSIMarshaller *current_host = NULL, BOOL lock = FALSE, BOOL use_clones = FALSE);

	void operator()(const IWSIStackObject&, LONG_PTR rpc_handle = 0, IWSIMarshaller *current_host = NULL, BOOL lock = FALSE, BOOL use_clones = FALSE);

	void operator()(ISTREAMWriter& writer, const void *obj, std::string& wsi); /*returns string image*/

//	Construction/Destruction.
public:
	CWsiSerializerBase(void * inner, const std::string& wsi_id = "", bool late_binding = false);
	~CWsiSerializerBase();

	void set_outer_ptr(void *obj);

//	Other public functions.
public:
	std::string operator()(ISTREAMWriter& writer); /*returns string image*/


	void operator()(ISTREAMWriter& writer, std::string& wsi); /*returns string image*/


//	ARRAY ISSUE:
//	The following members will provide support for array members of the base class.
public:
	__w64 int _outer_offset;

	virtual bool MoveNext(size_t index = 0/*if index > 0, then jump to the index*/);
	virtual void Init(int offset = 0, CWsiSerializerBase *This = 0) = 0;

	virtual bool is(__int64 member);

//	VECTOR ISSUE:
//	_inner_ptr of internal objects are initially initialized with zero,
//	because vector has size zero initially.
//	_outer_offset has to be reevaluated in order to position the pointers to newly added item.
//	see CWsiVectorSerializer implementation for details.

};









class EXP ABFundamentalTypesSerializer {
//	ARRAY TYPE support:
//	Current index of an array element.
//	It supports the case when this class is used to address elements of an array, i.e.
//	the fundamental type is an array.
	size_t _iter;

//	A reference to the hosting serializer.
//	It contains a pointer to currrent instance of a base class, a member of which this class represents.
	converter_tuple	*_base;
	char *_mem_base; 

	char *GetBase() {
		char *base =
			(char*)_base->_serializer->get_inner_ptr();
		base += _base->_serializer->_outer_offset;

		return base;
	}

	void SetMemBase(long int p_mem) {
		_mem_base = GetBase() + (long int)p_mem; 
	}


// Specialization of converter functions

	template <typename T>
	void wsi_getMember(const T& x) {
		std::string& wsi = _base->_wsi; 
		if(x != T()) {
			std::ostringstream ostr;
			ostr << x;
			wsi = ostr.str();
		}
		else {
			wsi.resize(0);
		}
	}

	void wsi_getMember(__int32 x);

	void wsi_getMember(unsigned __int32 x);

	void wsi_getMember(const __int16& x);

	void wsi_getMember(const unsigned __int16& x);

	void wsi_getMember(const long& x);

	void wsi_getMember(const unsigned long& x);

	void wsi_getMember(const __int64& x);

	void wsi_getMember(const float& x);

	void wsi_getMember(const double& x);

	void wsi_getMember(const char& x);

	void wsi_getMember(const signed char& x);

	void wsi_getMember(const unsigned char& x);

	void wsi_getMember(const std::string& x);

	void wsi_getMember(const std::u8string& x);

	void wsi_getMember(const std::payload_string& x);

	void wsi_getMember(const substring_locator& x);

//	character array versions.
	void wsi_getMember(const char* x); // unsafe

	void wsi_getString(const char *x, size_t S); 

	void wsi_getHexString(const unsigned char *x, size_t S); 

	void wsi_getHexString(const signed char *x, size_t S); 

	void wsi_getHexString(const wchar_t *x, size_t S);



//	put
	//template <typename T>
	//void wsi_putMember(T& what) {
	//	std::istringstream istr(_base->_wsi);
	//	istr >> what;
	//}

	void wsi_putMember(char& what);

	void wsi_putMember(signed char& what);

	void wsi_putMember(unsigned char& what);

	void wsi_putMember(std::string& what);

	void wsi_putMember(std::u8string& what);

	void wsi_putMember(std::payload_string& what);

	void wsi_putMember(substring_locator& what);

	void wsi_putMember(float& what);

	void wsi_putMember(double& what);

	void wsi_putMember(__int16& what);

	void wsi_putMember(__int32& what);

	void wsi_putMember(long& what);

	void wsi_putMember(unsigned __int16& what);

	void wsi_putMember(unsigned __int32& what);

	void wsi_putMember(unsigned long& what);

	void wsi_putMember(__int64& what);

//	character array versions.
	void wsi_putString(char *what, size_t S);

	void wsi_putHexString(unsigned char *what, size_t S);

	void wsi_putHexString(signed char *what, size_t S);

	void wsi_putHexString(wchar_t *what, size_t S);


//	erase
	template <typename T>
	void wsi_eraseMember(T& m) {
		m = T(0);
	}

	void wsi_eraseMember(std::string& m);
	void wsi_eraseMember(std::u8string& m);
	void wsi_eraseMember(std::payload_string& m);
	void wsi_eraseMember(substring_locator& m);






//	get
	template <typename T> inline bool wsi_get(T& m, non_array_member_tag) {
		wsi_getMember(m);
		return false;
	}


	template <typename T> inline bool wsi_getMember(T *m, int S) {
		if(_iter < (size_t)S) {
			wsi_getMember(m[_iter++]);
			return true; 
		}
		return false;
	}

	inline bool wsi_getMember(char *m, int S) {
		wsi_getString(m, S);
		return false;
	}

	inline bool wsi_getMember(unsigned char *m, int S) {
		wsi_getHexString(m, S);
		return false;
	}

	inline bool wsi_getMember(signed char *m, int S) {
		wsi_getHexString(m, S);
		return false;
	}

	template <typename T, int S> inline bool wsi_get(T (&m)[S], array_member_tag) {
		return wsi_getMember(m, S);
	}


	template <typename T> inline bool wsi_get(T& m, vector_member_tag) {
		if(_iter < m.size()) {
			wsi_getMember(m[_iter++]);
			return true; 
		}

		return false;
	}


//	put
	template <typename T> inline void wsi_put(T& m, non_array_member_tag) {
		wsi_putMember(m);
	}

	template <typename T> inline void wsi_putMember(T *m, int S) {
		if(_iter < (size_t)S) {
			wsi_putMember(m[_iter++]);
		}
	}

	inline void wsi_putMember(char *m, int S) {
		wsi_putString(m, S);
	}

	inline void wsi_putMember(unsigned char *m, int S) {
		wsi_putHexString(m, S);
	}

	inline void wsi_putMember(signed char *m, int S) {
		wsi_putHexString(m, S);
	}

	template <typename T, int S> inline void wsi_put(T (&m)[S], array_member_tag) {
		wsi_putMember(m, S);
	}

	template <typename T> inline void wsi_put(T& m, vector_member_tag) {
		if(_iter >= m.size() && _iter < WSI_MAX_VECTOR_SIZE) {
			m.resize(_iter + 1);
		}
		if(_iter < WSI_MAX_VECTOR_SIZE) {
			wsi_putMember(m[_iter++]);
		}
	}




//	erase a single value
	template <typename T> inline void wsi_erase(T& m, non_array_member_tag) {
		m = T();
	}

	void inline wsi_erase(std::string& m, non_array_member_tag) {
		wsi_eraseMember(m);
	}


	template <typename T> inline void wsi_eraseMember(T *m, int S) {
		_iter = S;
		while(_iter) {
			wsi_eraseMember(m[--_iter]);
		}
	}
	inline void wsi_eraseMember(char *m, int S) {
		memset(m, 0, S);
	}
	inline void wsi_eraseMember(unsigned char *m, int S) {
		memset(m, 0, S);
	}
	inline void wsi_eraseMember(signed char *m, int S) {
		memset(m, 0, S);
	}

//	erase an array.
	template <typename T, int S> inline void wsi_erase(T (&m)[S], array_member_tag) {
		wsi_eraseMember(m, S);
	}

	template <typename T> inline void wsi_erase(T& m, vector_member_tag) {
		m.resize(_iter = 0);
	}




	void wsi_resetArray(size_t it, size_t S) {
		if(it > 0 && it <= S) {
			_iter = it - 1;
		}
		else 
		if(it == (size_t)-1) { // increment
			if(++_iter >= S) {
				_iter = S - 1;
			}
		} 
		else {
			_iter = 0;
		}
	}

	template <typename T> inline void wsi_reset(T& m, size_t, non_array_member_tag) {
	}

	template <typename T, int S> inline void wsi_reset(T (&m)[S], size_t iter, array_member_tag) {
		wsi_resetArray(iter, S);
	}

	template <typename T> inline void wsi_reset(T& m, size_t iter, vector_member_tag) {
		wsi_resetArray(iter, WSI_MAX_VECTOR_SIZE);
	}



public:
	template <typename M>
	bool operator()(M* const p_mem, int action) { 
		M& what = *(M*)_mem_base; 
		switch(action) { 
			//case 0: 
			//	wsi_reset(what, (size_t)0, Type_cat(what));
			//	break; 
			case 1: 
				wsi_put(what, Type_cat(what)); 
				break; 
			case 2: 
				return wsi_get(what, Type_cat(what)); 
			case 3: 
				wsi_erase(what, Type_cat(what)); 
				break; 
			case 4: 
				SetMemBase((long int)p_mem); 
				break;
			case 5: 
				wsi_reset(what, (size_t)-1, Type_cat(what)); 
				break;
			default:
				wsi_reset(what, (size_t)(-action), Type_cat(what)); 
				break;
		}

		return false;
	}

	template <typename T, typename M> 
	bool operator()(M T::*p_mem, int action) { 
		M& what = *(M*)_mem_base; 
		switch(action) { 
			//case 0: 
			//	wsi_reset(what, (size_t)0, Type_cat(what)); 
			//	break; 
			case 1: 
				wsi_put(what, Type_cat(what)); 
				break; 
			case 2: 
				return wsi_get(what, Type_cat(what)); 
			case 3: 
				wsi_erase(what, Type_cat(what)); 
				break; 
			case 4: 
				SetMemBase((long int)&(((T*)0)->*p_mem)); 
				break; 
			case 5: 
				wsi_reset(what, (size_t)-1, Type_cat(what)); 
				break;
			default: 
				wsi_reset(what, (size_t)(-action), Type_cat(what)); 
				break; 
		} 

		return false; 
	} 

	ABFundamentalTypesSerializer(converter_tuple& base): _iter(0), _mem_base(0) {
		_base = &base;
	}
	ABFundamentalTypesSerializer(): _iter(0), _mem_base(0) {
		_base = 0;
	}
	void SetBase(converter_tuple *base) {
		_base = base;
	}
};






template <class T>
class CWsiSerializer: public CWsiSerializerBase {
protected:
	void Init(int offset = 0, CWsiSerializerBase *This = 0);

	T *_my_ptr;

public:
	CWsiSerializer(T *inner, const std::string& wsi_id = "", bool late_binding = false):
		CWsiSerializerBase(inner, wsi_id, late_binding),
		_my_ptr(inner) {
		Init();
	}

	CWsiSerializer(T& inner, const std::string& wsi_id = "", bool late_binding = false):
		CWsiSerializerBase(&inner, wsi_id, late_binding),
		_my_ptr(&inner) {
		Init();
	}

	// the following constructor is mainly for inheritance.
	// the contructor will merely add converter_tuples to an external serializer presented by pointer This. 
	// an offset specifies the offset of an inerited base class. 
	CWsiSerializer(CWsiSerializerBase *This, T *inner = 0, int offset = 0): CWsiSerializerBase(inner), _my_ptr(inner) {
		Init(offset, This);
	}

	~CWsiSerializer() {
		remove_all();
	}
};



class EXP IWsiArraySerializer {
protected:
	size_t _next;
	size_t _last;

public:
	virtual void * GetElement(size_t idx);

	bool MyCallFunctor(int action, CWsiSerializerBase *This);
	bool MyMoveNext(CWsiSerializerBase *This, size_t index = 0);
	virtual ~IWsiArraySerializer() {}
};


template<typename T, int S>
class CWsiArraySerializer: public CWsiSerializer<T>, public IWsiArraySerializer {
public:
	CWsiArraySerializer(T (&member)[S], const std::string& wsi_id = ""):
		CWsiSerializer<T>(&member[0], wsi_id) {
		_next = 1;
		_last = S;
	}

	bool call_functor(int action) {
		return MyCallFunctor(action, this);
	}

	void * GetElement(size_t idx) {
		return &(this->_my_ptr[idx]);
	}

//	ARRAY ISSUE: array of objects.
//	MoveNext() checks current index.
//	MoveNext() rebinds all _serializer->_map entries to current index.
//	MoveNext() increments current index.
	bool MoveNext(size_t index = 0) {
		return MyMoveNext(this, index);
	}

};






class EXP IWsiVectorSerializer: public CWsiSerializerBase {
//class EXP IWsiVectorSerializer {
protected:
	size_t _next;
	size_t _last;

public:
//	2008-09-16: Borland has hard time compiling multiple inheritance with abstract classes.
//	virtual void * GetElement(size_t idx) = 0;
//	virtual void Remap() = 0;
//	virtual void Resize(size_t new_size) = 0;
//	virtual size_t Size() = 0;
	virtual void * GetElement(size_t idx);
	virtual void Remap();
	virtual void Resize(size_t new_size);
	virtual size_t Size();

	IWsiVectorSerializer(const std::string& wsi_id, size_t last);

	bool MyCallFunctor(int action, CWsiSerializerBase *This);
	bool MyMoveNext(CWsiSerializerBase *This, size_t index = 0);
};


template<typename T, int S>
//class CWsiVectorSerializer: public IWsiVectorSerializer, public CWsiSerializer<T> {
//class CWsiVectorSerializer: public CWsiSerializer<T>, public IWsiVectorSerializer {
class CWsiVectorSerializer: public IWsiVectorSerializer {
protected:
	std::vector<T> *_my_ptr; // works to get_string_image() with offset.
	std::vector<T> *_member;

	void Init(int offset = 0, CWsiSerializerBase *This = 0) {
		CWsiSerializer<T>(this);
	}

public:
	CWsiVectorSerializer(std::vector<T>& member, const std::string& wsi_id = ""):
		//CWsiSerializer<T>((T*)0, wsi_id, true) {
		IWsiVectorSerializer(wsi_id, S), // vector serializer is always created with late_binding set on. 
		_member(&member),
		_my_ptr(&member) {
		//_next = 0;
		//_last = S;
		//_member = &member;
		//_my_ptr = &member;
	}

	bool call_functor(int action) {
		return MyCallFunctor(action, this);
	}

	bool is(__int64 member) {
		return member == (__int64)(this->_member);
	}

	void * GetElement(size_t idx) {
		return &((*(this->_my_ptr))[idx]);
	}

	void Remap(){
		this->_my_ptr = (std::vector<T>*)(((char*)_member) + this->_outer_offset);
	}

	void Resize(size_t new_size) {
		this->_my_ptr->resize(new_size);
	}

	size_t Size() {
		return this->_my_ptr->size();
	}

	bool MoveNext(size_t index = 0) {
		return MyMoveNext(this, index);
	}

};







template <class T>
class CWsiSafeSerializer: public CWsiSerializer<T>, public wsi_reenterantgate {
	bool _destroy_inner;

public:
	CWsiSafeSerializer(T& inner, const std::string& wsi_id = ""):
		CWsiSerializer<T>(inner, wsi_id) {
		_destroy_inner = false;
	}

	CWsiSafeSerializer(T *inner, const std::string& wsi_id = "", bool destroy_inner = false):
		CWsiSerializer<T>(inner, wsi_id) {
		_destroy_inner = destroy_inner;
	}

	IWsiSerializerBase* Create() const {
		T *x = new T();
		return new CWsiSafeSerializer<T>(x, this->_wsi_id, true);
	}

	IWsiSerializerBase* CreateEmpty() const {
		return new CWsiSafeSerializer<T>((T*)0, this->_wsi_id);
	}

	void lock() {
		wsi_reenterantgate::lock();
	}

	void unlock() {
		wsi_reenterantgate::unlock();
	}

	BOOL onNewObjectClone(const std::string& name, void *obj, LONG_PTR rpc_handle = 0, IWSIMarshaller *current_host = NULL, BOOL locked = FALSE) {
		T clone = *(T*)obj;
		if(locked) {
			unlock();
		}
		onNewObject(name, &clone, rpc_handle, current_host);
		return FALSE;
	}

	~CWsiSafeSerializer() {
		this->remove_all();
		if(_destroy_inner) {
			delete this->_my_ptr;
			this->_my_ptr = 0;
		}
	}
};




struct function_type2: function_type1 { // neccessary evil due to optimization of contains_flat_member()
	ABFundamentalTypesSerializer s;
};



template<typename M, typename T>
class ABFundamentalTypesFunctor: public function_type2 {
	M T::* _member;

public:
	ABFundamentalTypesFunctor(M T::*p_mem): _member(p_mem) {
	}
	bool operator()(int arg) {
		return s(_member, arg);
	}
	bool is(__int64 member) {
		return member == (__int64)&(((T*)0)->*_member);
	}
};

template<typename M>
class ABFundamentalTypesFunctor2: public function_type2 {
	M *_member;

public:
	ABFundamentalTypesFunctor2(M *p_mem): _member(p_mem) {
	}
	bool operator()(int arg) {
		return s(_member, arg);
	}
	bool is(__int64 member) {
		return member == (__int64)_member;
	}
};


class ABSerializerBaseFunctor: public function_type1 {
	converter_tuple& _base;

public:
	ABSerializerBaseFunctor(converter_tuple& base): _base(base) {
	}
	bool is(__int64 member) {
		return _base._serializer->is(member);
	}
	bool operator()(int arg) {
		return _base._serializer->call_functor(arg);
	}
};






template<typename M>
inline CWsiSerializerBase *create_wsiserializer(std::vector<M>& member, const std::string& wsi_id, vector_member_tag) {
	return
		new CWsiVectorSerializer<M, WSI_MAX_VECTOR_SIZE>(member, wsi_id);
}

template<typename M, int S>
inline CWsiSerializerBase *create_wsiserializer(M (&member)[S], const std::string& wsi_id, array_member_tag) {
	return
		new CWsiArraySerializer<M, S>(member, wsi_id);
}

template<typename M>
inline CWsiSerializerBase *create_wsiserializer(M& member, const std::string& wsi_id, non_array_member_tag) {
	return
		new CWsiSerializer<M>(member, wsi_id);
}




void EXP add_flat_member(const char* const wsi_id, CWsiSerializerBase *base, function_type2 *functor2);
void EXP add_object_member(const char* const wsi_id, CWsiSerializerBase *base, CWsiSerializerBase *serializer);



//template<typename M, typename T>
//inline void contains_flat_member(M T::*p_mem, const char* const wsi_id, CWsiSerializerBase *base) {
//	add_flat_member(wsi_id, base, new ABFundamentalTypesFunctor<M, T>(p_mem));
//}



template<typename M, typename T>
inline void contains_flat_member(M T::*p_mem, int offset, const char* const wsi_id, CWsiSerializerBase *base) {
	add_flat_member(wsi_id, base, new ABFundamentalTypesFunctor2<M>(&(((T*)offset)->*p_mem)));
}
template<typename M, typename T>
inline void contains_object_member(M T::*p_mem, T *inner_ptr, const char* const wsi_id, CWsiSerializerBase *base) {
	M& member = inner_ptr->*p_mem;
	add_object_member(wsi_id, base, create_wsiserializer(member, wsi_id, Type_cat(member)));
}



template<typename M>
inline void contains_flat_member(M& mem, const char* const wsi_id, CWsiSerializerBase *base) {
	add_flat_member(wsi_id, base, new ABFundamentalTypesFunctor2<M>(&mem));
}
template<typename M>
inline void contains_object_member(M& mem, const char* const wsi_id, CWsiSerializerBase *base) {
	add_object_member(wsi_id, base, create_wsiserializer(mem, wsi_id, Type_cat(mem)));
}







#if defined(WSICLASSFACTORY_IMPL)
#define BEGIN_WSI_SERIALIZATION_OBJECT(ClassName) \
typedef CWsiSerializer<ClassName> CWSI##ClassName; \
typedef CWsiSafeSerializer<ClassName> CWSISafe##ClassName; \
template<> void CWsiSerializer<ClassName>::Init(int offset, CWsiSerializerBase *This) { \
	typedef ClassName Inner_Type; \
	if(_wsi_id.size() == 0) _wsi_id = #ClassName; \
	if(_is_late_binding) return; \
	if(This == 0) This = this; 

#define INHERIT_SERIALIZATION_OBJECT(BaseType) \
	CWsiSerializer<BaseType>(This, (BaseType*)_my_ptr, (int)static_cast<BaseType*>((Inner_Type*)1000) - 1000); 

#define BEGIN_WSI_INHERITED_SERIALIZATION_OBJECT(BaseType, ClassName) \
	BEGIN_WSI_SERIALIZATION_OBJECT(ClassName) \
	INHERIT_SERIALIZATION_OBJECT(BaseType)

#define SET_OBJECT_IDENTIFICATION(ObjectId) \
	_wsi_id = #ObjectId;

#if defined(_MSC_VER) || __BORLANDC__ > 0x630
#define CONTAINS_FLAT_MEMBER(Member, Identifyer)  contains_flat_member(((Inner_Type*)offset)->Member, #Identifyer, This);
#define CONTAINS_OBJECT_MEMBER(Member, Identifyer)  contains_object_member(_my_ptr->Member, #Identifyer, This);
#else
#define CONTAINS_FLAT_MEMBER(Member, Identifyer)  contains_flat_member(&Inner_Type::Member, offset, #Identifyer, This);
#define CONTAINS_OBJECT_MEMBER(Member, Identifyer)  contains_object_member(&Inner_Type::Member, _my_ptr, #Identifyer, This);
#endif 

#define CONTAINS_INHERITED_FLAT_MEMBER(BaseType, Member, Identifyer) CONTAINS_FLAT_MEMBER(Member, Identifyer)

#define END_WSI_SERIALIZATION_OBJECT() \
}



#else
#define BEGIN_WSI_SERIALIZATION_OBJECT(ClassName) \
typedef CWsiSerializer<ClassName> CWSI##ClassName; \
typedef CWsiSafeSerializer<ClassName> CWSISafe##ClassName; \
template<> void CWsiSerializer<ClassName>::Init(int offset, CWsiSerializerBase *This);

#define INHERIT_SERIALIZATION_OBJECT(BaseType)
#define BEGIN_WSI_INHERITED_SERIALIZATION_OBJECT(BaseType, ClassName) BEGIN_WSI_SERIALIZATION_OBJECT(ClassName)

#define SET_OBJECT_IDENTIFICATION(ObjectId)
#define CONTAINS_FLAT_MEMBER(Member, Identifyer)
#define CONTAINS_OBJECT_MEMBER(Member, Identifyer)
#define CONTAINS_INHERITED_FLAT_MEMBER(BaseType, Member, Identifyer)
#define END_WSI_SERIALIZATION_OBJECT()
#define AUTOCREATE_WSI_SERIALIZATION_OBJECT(ClassName)
#endif






class EXP CWsiCollectionImpl: public IWsiCollection {
	const CWsiCollectionImpl& operator=(const CWsiCollectionImpl&);
	object_serializers *_pmap;

public:
	const object_serializers& Get() const;
	void insert(IWsiSerializerBase* serializer);
	IWsiSerializerBase* find(const std::string& id);

	void merge(const IWsiCollection& coll);


	CWsiCollectionImpl();
	~CWsiCollectionImpl();
};





void EXP XMLXConfigure(IWsiSerializerBase& obj, const std::string& config);

void EXP WSIXConfigure(IWsiSerializerBase& obj, const std::string& config, IXFunction<IWSIMarshaller, void> *pExtra_callbacks = 0);
void EXP WSIXSerialize(std::string& config, IWsiSerializerBase& obj, bool human_readable = false, IWsiSerializerBase **extra = 0, const int extra_cnt = 0);



template<class T>
void XConfigure(T& obj, const std::string& config, IXFunction<IWSIMarshaller, void> *pExtra_callbacks = 0) {
	if(config.size()) {
		CWsiSerializer<T> serializer(&obj);
		WSIXConfigure(serializer, config, pExtra_callbacks);
	}
}

template<class T>
void XSerialize(std::string& config, T& obj, bool human_readable = false, IWsiSerializerBase **extra = 0, const int extra_cnt = 0) {
	CWsiSerializer<T> serializer(&obj);
	WSIXSerialize(config, serializer, human_readable, extra, extra_cnt);
}



#endif //wsiclassfactoryH
//----------------------------------------------------------------------------











#ifndef autocreateserialization_not_necessaryH
#define autocreateserialization_not_necessaryH


#ifdef WSI_NAMESPACE
namespace WSI_NAMESPACE {
#endif


class CWsiRootCollection: public IWsiCollection {
	static object_serializers _map;

public:
	const object_serializers& Get() const {
		return _map;
	}

	static void insert(IWsiSerializerBase* serializer) {
		m_insert(_map, serializer);
	}
};


class CWsiCollection: public CWsiCollectionImpl {
public:
	CWsiCollection(): CWsiCollectionImpl() {
		merge(CWsiRootCollection());
	}
};



template <class T>
class CWsiRootSerializer: public CWsiSafeSerializer<T> {
	T _inner;

public:
	CWsiRootSerializer(const std::string& wsi_id = ""): CWsiSafeSerializer<T>(_inner, wsi_id) {
		CWsiRootCollection::insert(this);
	}
	~CWsiRootSerializer() {
		this->_terminating = true;
	}
};



#ifdef WSICLASSFACTORY_IMPL
#ifndef DELAY_IMPLEMENTATION_ROOTCOLLECTION_MAP
std::map<std::string, IWsiSerializerBase* > CWsiRootCollection::_map;
#endif
#endif //WSICLASSFACTORY_IMPL



#ifdef WSI_NAMESPACE
};
#endif


#endif //autocreateserialization_not_necessaryH
//----------------------------------------------------------------------------









#ifdef WSICLASSFACTORY_IMPL



#ifdef AUTOCREATE_WSI_SERIALIZATION_OBJECT
#undef AUTOCREATE_WSI_SERIALIZATION_OBJECT
#endif

#ifndef APIEXP
#define APIEXP EXP
#endif

#ifdef WSI_NAMESPACE
#define AUTOCREATE_WSI_SERIALIZATION_OBJECT(ClassName) \
namespace WSI_NAMESPACE { \
CWsiRootSerializer<ClassName> __wsiRoot##ClassName; \
}; \
template<> APIEXP IWsiSerializerBase& GetRootSerializer<ClassName>(const ClassName*) { return WSI_NAMESPACE::__wsiRoot##ClassName; }


#else
#define AUTOCREATE_WSI_SERIALIZATION_OBJECT(ClassName) \
CWsiRootSerializer<ClassName> __wsiRoot##ClassName; \
template<> APIEXP IWsiSerializerBase& GetRootSerializer<ClassName>(const ClassName*) { return __wsiRoot##ClassName; }


#endif //WSI_NAMESPACE

#else //WSICLASSFACTORY_IMPL

#if defined(_UCC)

#ifdef AUTOCREATE_WSI_SERIALIZATION_OBJECT
#undef AUTOCREATE_WSI_SERIALIZATION_OBJECT
#endif

#ifndef APIEXP
#define APIEXP EXP
#endif

#define AUTOCREATE_WSI_SERIALIZATION_OBJECT(ClassName) \
template<> APIEXP IWsiSerializerBase& GetRootSerializer<ClassName>(const ClassName*);

#endif

#endif //WSICLASSFACTORY_IMPL
//----------------------------------------------------------------------------






EXP const IWsiCollection& GetWsiClassFactoryRootCollection();


#ifdef XCLASSFACTORY_IMPL

#ifdef WSI_NAMESPACE
namespace WSI_NAMESPACE {
#endif
CWsiRootCollection _g_classfactoryrootcollection;
#ifdef WSI_NAMESPACE
};
#endif

EXP const IWsiCollection& GetWsiClassFactoryRootCollection() {
#ifdef WSI_NAMESPACE
	return WSI_NAMESPACE::_g_classfactoryrootcollection;
#else
	return _g_classfactoryrootcollection;
#endif
}


BEGIN_WSI_SERIALIZATION_OBJECT(SIPCRawString)
CONTAINS_FLAT_MEMBER(_string_write, Str)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(SIPCRawString)


#endif //XCLASSFACTORY_IMPL
//----------------------------------------------------------------------------




