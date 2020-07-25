// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef wsimarshallerH
#define wsimarshallerH


//#include "./ExportSTLMacro.h"



#include ".\..\Include\WSIClassFactoryInterfaces.h"
#include ".\WSIMaterializer.h"


class CWSIMarshaller: public IWSIMarshaller, wsi_gate {
//	Serializers portion.
public:
	typedef std::map<IWsiSerializerBase*, std::string> serializer_cookies;
	typedef std::map<std::string, IWsiSerializerBase*> object_serializers;

public:
	bool add_callee(IWsiSerializerBase* t, const std::string& wsi_id);

	void remove_callee(IWsiSerializerBase* t);


//	Serializers portion.
protected:
	IWsiSerializerBase * find_serializer(const std::string& wsi_id);

public:
	void operator()(const IWSIStackObject&, LONG_PTR rpc_handle, BOOL use_clones = FALSE);
//	void operator()(std::vector<wsi_object>& ostack, LONG_PTR rpc_handle);

	void operator()(const IWSIObject&, LONG_PTR rpc_handle, BOOL use_clones = FALSE);
//	void operator()(wsi_object& obj, LONG_PTR rpc_handle);

protected:
	object_serializers _mapFact;
	serializer_cookies _mapSerializersCookies;


//	Consumers portion.
public:
	typedef std::map<IWsiConsumerBase*, std::string> consumer_cookies;
	typedef std::multimap<std::string, IWsiConsumerBase*> object_consumers;

	bool add_callee(IWsiConsumerBase* t, const std::string& wsi_id);

	void remove_callee(IWsiConsumerBase* t);

//	Consumers portion.
public:
	void operator()(const std::string& id, void* obj, LONG_PTR rpc_handle = 0);

protected:
	object_consumers _mapCons;
	consumer_cookies _mapConsumersCookies;


//	Main interface for class Connector.
public:
	void operator()(const ISTREAMReader& parser, const std::string& wsi, LONG_PTR rpc_handle = 0, BOOL use_clones = FALSE);
	void operator()(const ISTREAMReader& parser, std::string::const_iterator from, std::string::const_iterator to, LONG_PTR rpc_handle = 0, BOOL use_clones = FALSE);

	void operator()(ISTREAMWriter& writer, const void *obj, const std::string& wsi_id, std::string& wsi);

protected:
	void operator()(const ISTREAMReader& parser, const std::string& wsi, std::string::const_iterator from, std::string::const_iterator to, LONG_PTR rpc_handle = 0, BOOL use_clones = FALSE);
	wsi_materializer _materializer;
	int _encoding;


public:
//	Construction/Destruction.
	void Init();
	int Encoding() const;

public:
	CWSIMarshaller();

	virtual ~CWSIMarshaller();

};




#endif //wsimarshallerH

