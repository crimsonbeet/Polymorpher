// Created by Alex Nikol Bourakov (ANB) around April/07


#include "XRemoting.h" 





BEGIN_WSI_SERIALIZATION_OBJECT(XListener)

CONTAINS_FLAT_MEMBER(_name, Name)
CONTAINS_FLAT_MEMBER(_addr, Address)
CONTAINS_FLAT_MEMBER(_index, Index)
CONTAINS_FLAT_MEMBER(_parser_name, Parser)
CONTAINS_FLAT_MEMBER(_datatypes, Datatype)
CONTAINS_FLAT_MEMBER(_max_parallel_requests, MaxParallelRequests)
CONTAINS_FLAT_MEMBER(_is_time_critical, IsTimeCritical)
END_WSI_SERIALIZATION_OBJECT()





BEGIN_WSI_SERIALIZATION_OBJECT(XSubscriber)

CONTAINS_FLAT_MEMBER(_persistent, Persistent)
CONTAINS_FLAT_MEMBER(_is_default, IsDefault)
CONTAINS_FLAT_MEMBER(_is_indexed, IsIndexed)
CONTAINS_FLAT_MEMBER(_is_nonblocking, IsNonBlocking)
CONTAINS_FLAT_MEMBER(_no_keepalive, NoKeepalive)
CONTAINS_FLAT_MEMBER(_in_buffer_size_kbytes, InBufferSizeKBytes) 
CONTAINS_FLAT_MEMBER(_out_buffer_size_kbytes, OutBufferSizeKBytes) 
CONTAINS_FLAT_MEMBER(_version, Version)
CONTAINS_FLAT_MEMBER(_name, Name)
CONTAINS_FLAT_MEMBER(_addr, Address)
CONTAINS_FLAT_MEMBER(_index, Index)
CONTAINS_FLAT_MEMBER(_parser_name, Parser)
CONTAINS_FLAT_MEMBER(_datatypes, Datatype)
CONTAINS_FLAT_MEMBER(_returntypes, Returns)
CONTAINS_FLAT_MEMBER(_log_receiver, IsLogReceiver)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(XSubscriber)





BEGIN_WSI_INHERITED_SERIALIZATION_OBJECT(XSubscriber, XSubscriberArray)
CONTAINS_FLAT_MEMBER(_max_index, MaxIndex)
END_WSI_SERIALIZATION_OBJECT()





BEGIN_WSI_SERIALIZATION_OBJECT(XProducer)

CONTAINS_FLAT_MEMBER(_name, Name)
CONTAINS_FLAT_MEMBER(_addr, Address)
CONTAINS_FLAT_MEMBER(_index, Index)
CONTAINS_FLAT_MEMBER(_parser_name, Parser)
CONTAINS_FLAT_MEMBER(_datatypes, Returns) // Notice: the meaning is opposite to Subscriber's. 
CONTAINS_FLAT_MEMBER(_returntypes, Datatype)
CONTAINS_FLAT_MEMBER(_is_a_log_server, IsLogServer)
END_WSI_SERIALIZATION_OBJECT()

//AUTOCREATE_WSI_SERIALIZATION_OBJECT(XProducer)

BEGIN_WSI_INHERITED_SERIALIZATION_OBJECT(XProducer, XProducerArray)
CONTAINS_FLAT_MEMBER(_max_index, MaxIndex)
END_WSI_SERIALIZATION_OBJECT()





BEGIN_WSI_SERIALIZATION_OBJECT(XConnector)

CONTAINS_FLAT_MEMBER(_name, Name)
CONTAINS_FLAT_MEMBER(_responsetimeoutsecs, ResponseTimeoutSecs)
CONTAINS_FLAT_MEMBER(_use_clones, UseClones)
CONTAINS_FLAT_MEMBER(_generic_index, GenericIndex)
CONTAINS_FLAT_MEMBER(_add_subscriber_ondemand, AddSubscriberOndemand)
CONTAINS_OBJECT_MEMBER(_listener, Listener)
CONTAINS_OBJECT_MEMBER(_static_subscribers, Subscriber)
CONTAINS_OBJECT_MEMBER(_array_subscribers, SubscriberArray)
CONTAINS_OBJECT_MEMBER(_array_producers, ProducerArray)
CONTAINS_OBJECT_MEMBER(_producers, Producer)
END_WSI_SERIALIZATION_OBJECT()

//AUTOCREATE_WSI_SERIALIZATION_OBJECT(XConnector)





BEGIN_WSI_SERIALIZATION_OBJECT(XChangeableAddress)

CONTAINS_FLAT_MEMBER(_name, Name)
CONTAINS_FLAT_MEMBER(_addr, Address)
CONTAINS_FLAT_MEMBER(_description, Description)
CONTAINS_FLAT_MEMBER(_index, Index)
END_WSI_SERIALIZATION_OBJECT()





BEGIN_WSI_SERIALIZATION_OBJECT(XRemoting)

CONTAINS_FLAT_MEMBER(_base_directory, Directory)
CONTAINS_FLAT_MEMBER(_subscribers2ignore, IgnoreSubscriber)
CONTAINS_FLAT_MEMBER(_listeners2ignore, IgnoreListener)
CONTAINS_FLAT_MEMBER(_trace, Trace)
CONTAINS_OBJECT_MEMBER(_connectors, Connector)
CONTAINS_OBJECT_MEMBER(_changeable_addresses, ChangeableAddress)
END_WSI_SERIALIZATION_OBJECT()

//AUTOCREATE_WSI_SERIALIZATION_OBJECT(XRemoting)



BEGIN_WSI_SERIALIZATION_OBJECT(XRemotingSettings)

CONTAINS_FLAT_MEMBER(_subscribers2ignore, IgnoreSubscriber)
CONTAINS_FLAT_MEMBER(_listeners2ignore, IgnoreListener)
CONTAINS_OBJECT_MEMBER(_changeable_addresses, ChangeableAddress)
END_WSI_SERIALIZATION_OBJECT()


