// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef wsimaterializerH
#define wsimaterializerH

#include ".\..\Include\WSIClassFactoryInterfaces.h"
#include ".\WSIObject.h"

struct vectorpos_locator {
	size_t _pos;
	vectorpos_locator(const size_t pos = 0);
	operator size_t() const;
};


class wsi_materializer: public IWSIMaterializer {
	std::stack<vectorpos_locator> _stack;
	std::vector<wsi_object> _context;
	wsi_object _current;

	PVOID _cached_context; 

public:
	IWSIMarshaller *_callback;
	LONG_PTR _rpc_handle; 
	BOOL _use_clones; 

	wsi_materializer();
	wsi_materializer(const wsi_materializer& other);
	virtual ~wsi_materializer();

protected:
	inline int increment_obj_counter(size_t pos);

	void place_object_on_context_stack(const IWSIObject& obj);

public:
	void reserve(size_t n);
	void release();

	IWSIObject get_current();
	IWSIObject get_context_current();

	void reduce_stack();

	void place_composite_object_on_stack(const IWSIObject& obj);

	void process_flat_object(const IWSIObject& obj);
	void process_raw_string(const SIPCRawString& obj);

	void flush();
};





#endif //wsimaterializerH

