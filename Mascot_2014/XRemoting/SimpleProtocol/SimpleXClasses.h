
#ifndef simpleprotocol_wsiclassesH
#define simpleprotocol_wsiclassesH




struct SPMessage {
};

struct SIMessage {
};






#ifdef WSI_NAMESPACE
#undef WSI_NAMESPACE
#endif
#ifdef autocreateserialization_not_necessaryH
#undef autocreateserialization_not_necessaryH
#endif

#define WSI_NAMESPACE XSimpleProtocol




#include ".\..\Include\WSIClassFactory.h"



BEGIN_WSI_SERIALIZATION_OBJECT(SPMessage)
SET_OBJECT_IDENTIFICATION(p)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(SPMessage)




BEGIN_WSI_SERIALIZATION_OBJECT(SIMessage)
SET_OBJECT_IDENTIFICATION(i)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(SIMessage)











struct SimpleProcotolConfiguration {
	std::string _ipaddress;

	int _ports[32];

	int _sockets[32];

	SimpleProcotolConfiguration() {
		memset(_sockets, 0xff, sizeof(_sockets));
	}
};




BEGIN_WSI_SERIALIZATION_OBJECT(SimpleProcotolConfiguration)
SET_OBJECT_IDENTIFICATION(Config)
CONTAINS_FLAT_MEMBER(_ipaddress, Address)
CONTAINS_FLAT_MEMBER(_ports, Port)
END_WSI_SERIALIZATION_OBJECT()

//AUTOCREATE_WSI_SERIALIZATION_OBJECT(SimpleProcotolConfiguration)








struct RPCRequest_StartCommunication {
	int _spindle;
	RPCRequest_StartCommunication(int spindle = 0): _spindle(spindle) {
	}
};

BEGIN_WSI_SERIALIZATION_OBJECT(RPCRequest_StartCommunication)
CONTAINS_FLAT_MEMBER(_spindle, Spindle)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(RPCRequest_StartCommunication)



struct RPCRequest_EnableDisableTool {
	int _spindle;
	int _enable;
	RPCRequest_EnableDisableTool(int spindle = 0, int enable = 0): _spindle(spindle), _enable(enable) {
	}
};

BEGIN_WSI_SERIALIZATION_OBJECT(RPCRequest_EnableDisableTool)
CONTAINS_FLAT_MEMBER(_spindle, Spindle)
CONTAINS_FLAT_MEMBER(_enable, Enable)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(RPCRequest_EnableDisableTool)





struct RPCResponse {
	std::string _response;
	int _ok;
	double _test_double;

	RPCResponse() {
		_ok = 0;
	}
};

BEGIN_WSI_SERIALIZATION_OBJECT(RPCResponse)
CONTAINS_FLAT_MEMBER(_response, Response)
CONTAINS_FLAT_MEMBER(_ok, Ok)
CONTAINS_FLAT_MEMBER(_test_double, TestDouble)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(RPCResponse)












struct SDartPacket { 
	std::string _id; 
	std::string _tag_id; 
	int _battery_level; 

	SDartPacket() { 
		_id = 'p'; 
		_battery_level = 0; 
	} 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(SDartPacket)
CONTAINS_FLAT_MEMBER(_id, F1)
CONTAINS_FLAT_MEMBER(_tag_id, F2)
CONTAINS_FLAT_MEMBER(_battery_level, F3)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(SDartPacket)


struct SDartData { 
	std::string _id; 
	int _sequence_number; 
	int _message_header; 

	SDartData() { 
		_id = 'd'; 
		_sequence_number = 0; 
		_message_header = 1; 
	} 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(SDartData)
CONTAINS_FLAT_MEMBER(_id, F1)
CONTAINS_FLAT_MEMBER(_sequence_number, F2)
CONTAINS_FLAT_MEMBER(_message_header, F3)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(SDartData)


struct SDartConnect { 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(SDartConnect)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(SDartConnect)








struct FPMessageHeader { 
	int _length; 
	int _mid; 
	int _spindle; 
	int _revision; 
	int _noack;
	std::string _reserved; 

	FPMessageHeader() { 
		_length = 0; 
		_mid = 0; 
		_spindle = 0; 
		_revision = 0; 
		_noack = 0; 
	} 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(FPMessageHeader)
CONTAINS_FLAT_MEMBER(_length, F1)
CONTAINS_FLAT_MEMBER(_mid, F2)
CONTAINS_FLAT_MEMBER(_spindle, F3)
CONTAINS_FLAT_MEMBER(_revision, F4)
CONTAINS_FLAT_MEMBER(_noack, F5)
CONTAINS_FLAT_MEMBER(_reserved, F6)
END_WSI_SERIALIZATION_OBJECT()



struct FPMessageStartCommunication: FPMessageHeader { 
	FPMessageStartCommunication(int spindle = 0) { 
		//_spindle = spindle; 
		_mid = 1; 
	} 
}; 

BEGIN_WSI_INHERITED_SERIALIZATION_OBJECT(FPMessageHeader, FPMessageStartCommunication)
SET_OBJECT_IDENTIFICATION(M001)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(FPMessageStartCommunication)



struct FPMessageStartAcknowledge: FPMessageHeader { 
	int _cell_id; 
	int _channel_id; 
	std::string _controller_name; 
	std::string _supplier_code; 
	std::string _protocol_version; 
	std::string _software_version_1; 
	std::string _software_version_2; 

	FPMessageStartAcknowledge() { 
		_cell_id = 0; 
		_channel_id = 0; 
		_revision = 1; 
		_mid = 2; 
	} 
}; 

BEGIN_WSI_INHERITED_SERIALIZATION_OBJECT(FPMessageHeader, FPMessageStartAcknowledge)
SET_OBJECT_IDENTIFICATION(M002)
CONTAINS_FLAT_MEMBER(_cell_id, F7)
CONTAINS_FLAT_MEMBER(_channel_id, F8)
CONTAINS_FLAT_MEMBER(_controller_name, F9)
CONTAINS_FLAT_MEMBER(_supplier_code, F10)
CONTAINS_FLAT_MEMBER(_protocol_version, F11)
CONTAINS_FLAT_MEMBER(_software_version_1, F12)
CONTAINS_FLAT_MEMBER(_software_version_2, F13)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(FPMessageStartAcknowledge)



struct FPMessageCommandAccepted: FPMessageHeader { 
	int _cmd_mid; 
	FPMessageCommandAccepted() { 
		_cmd_mid = 0; 
		_mid = 5; 
	} 
}; 

BEGIN_WSI_INHERITED_SERIALIZATION_OBJECT(FPMessageHeader, FPMessageCommandAccepted)
SET_OBJECT_IDENTIFICATION(M005)
CONTAINS_FLAT_MEMBER(_cmd_mid, F7)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(FPMessageCommandAccepted)




struct FPMessageToolDataRequest: FPMessageHeader { 
	FPMessageToolDataRequest(int spindle = 0) { 
		//_spindle = spindle; 
		_mid = 40; 
	} 
}; 

BEGIN_WSI_INHERITED_SERIALIZATION_OBJECT(FPMessageHeader, FPMessageToolDataRequest)
SET_OBJECT_IDENTIFICATION(M040)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(FPMessageToolDataRequest)




struct FPMessageToolDataResponse: FPMessageHeader { 
	std::string _tool_serialnumber; 
	int _total_cycles; 
	std::string _calibration_date; 
	std::string _controller_serialnumber; 
	int _calvalue100; 
	std::string _lastservice_date; 
	int _cycles_sinceservice; 
	int _tool_type; 
	int _motor_size; 
	int _openend_data; 
	std::string _software_version; 

	FPMessageToolDataResponse(int spindle = 0) { 
		//_spindle = spindle; 
		_revision = 2; 
		_mid = 41; 
	} 
}; 


BEGIN_WSI_INHERITED_SERIALIZATION_OBJECT(FPMessageHeader, FPMessageToolDataResponse)
SET_OBJECT_IDENTIFICATION(M041)
CONTAINS_FLAT_MEMBER(_tool_serialnumber, F7)
CONTAINS_FLAT_MEMBER(_total_cycles, F8)
CONTAINS_FLAT_MEMBER(_calibration_date, F9)
CONTAINS_FLAT_MEMBER(_controller_serialnumber, F10)
CONTAINS_FLAT_MEMBER(_calvalue100, F11)
CONTAINS_FLAT_MEMBER(_lastservice_date, F12)
CONTAINS_FLAT_MEMBER(_cycles_sinceservice, F13)
CONTAINS_FLAT_MEMBER(_tool_type, F14)
CONTAINS_FLAT_MEMBER(_motor_size, F15)
CONTAINS_FLAT_MEMBER(_openend_data, F16)
CONTAINS_FLAT_MEMBER(_software_version, F17)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(FPMessageToolDataResponse)





struct FPMessageDisableTool: FPMessageHeader { 
	FPMessageDisableTool(int spindle = 0) { 
		//_spindle = spindle; 
		_mid = 42; 
	} 
}; 

BEGIN_WSI_INHERITED_SERIALIZATION_OBJECT(FPMessageHeader, FPMessageDisableTool)
SET_OBJECT_IDENTIFICATION(M042)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(FPMessageDisableTool)





struct FPMessageEnableTool: FPMessageHeader { 
	FPMessageEnableTool(int spindle = 0) { 
		//_spindle = spindle; 
		_mid = 43; 
	} 
}; 

BEGIN_WSI_INHERITED_SERIALIZATION_OBJECT(FPMessageHeader, FPMessageEnableTool)
SET_OBJECT_IDENTIFICATION(M043)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(FPMessageEnableTool)











struct OPMessageHeader {
	// Length (4) , MID (4) , REV (3), Spare (9)  ==> 20 characters
	char mLength[4];
	char mMID[4];
	// The MID revision is unique per MID and is used in case where
	// several versions are available for the same MID. Using the
	// revision number the station computer can subscribe or ask for
	// different versions of the same MID. By default the MID revision
	// number is three spaces (revision 1 of the MID). So, if the station
	// computer is interested in the initial revision (revision 1) of the
	// MID, it can send three spaces as MID revision or 001.
	char mSpindle[2];
	char mREV[3];
	char mNoAckFlag;
	char mReserved[6];

	OPMessageHeader() {
	}

	OPMessageHeader(int aLength, int aMID, int aREV, char aNoAckFlag = ' ') {
		sprintf(mLength, "%04d", aLength);
		sprintf(mMID, "%04d", aMID);
		sprintf(mSpindle, "%02d", (int)0);
		sprintf(mREV, "%03d", aREV);
		mNoAckFlag = aNoAckFlag;
		memset(mReserved, ' ', sizeof(mReserved));
	}
	operator const std::string() {
		std::string rc = (const char*)this;
		rc.resize(rc.size() + 1);
		return rc;
	}
};


struct OPMessageEnd {
	char mNull;
	OPMessageEnd(): mNull(0) {
	}
	void reset( ) {
		mNull = 0;
	}
};



struct OPMessageStartCommunication: OPMessageHeader {
	// size = 20, MID = 1, REV = 0
	OPMessageEnd mEnd;
	OPMessageStartCommunication(): OPMessageHeader(20, 1, 0 ) {
	}
};


struct OPMessageDisableTool: OPMessageHeader { // ( MID == 0042 )
	OPMessageEnd mEnd;
	OPMessageDisableTool(): OPMessageHeader(20, 42, 0) {
	}
};

struct OPMessageEnableTool: OPMessageHeader { // ( MID == 0043 )
	OPMessageEnd mEnd;
	OPMessageEnableTool(): OPMessageHeader(20, 43, 0) {
	}
};




#endif //simpleprotocol_wsiclassesH
