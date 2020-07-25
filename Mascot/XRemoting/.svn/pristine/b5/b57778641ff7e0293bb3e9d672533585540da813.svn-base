// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef ipcobjectH 
#define ipcobjectH 


typedef enum { 
	IPCObj_Response, 
	IPCObj_ReceiveObj, 
	IPCObj_LogMessage, 
	IPCObj_Connect, 
	IPCObj_InvalidHeader, // 2011-03-28 Simple message
	IPCObj_FepMessage, // 2011-04-28
	IPCObj_DartMessage, // 2011-04-28
	IPCObj_HttpMessage, // 2011-05-17
	IPCObj_RawData // 2011-08-10
} __IPCOBJECTSENDACTION; 


typedef enum { 
	IPCReq_NOTPROCESSED, 
	IPCReq_PROCESSING, 
	IPCReq_SENT, 
	IPCReq_CONFIRMED, 
	IPCReq_CLOSED 
} __IPCOBJECTPRODUCERSTATE; 



#endif //ipcobjectH 
