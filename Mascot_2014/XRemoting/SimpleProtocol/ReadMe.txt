========================================================================
    CONSOLE APPLICATION : SimpleProtocol Project Overview
========================================================================

In current setup: 
1.
Visual studio compiles and links to Debug directory.
Eclipse compiles and links to Release directory

2.
Desktop application uses SimpleProtocolSecondary.txt for communication settings.
GC application uses SimpleProtocol.txt for communication settings.
Both applications use MyConfig.txt to setup g_myconfiguration object. Desktop application uses it only for demonstration, so it would be possible to walk the code in debug mode.
MyConfig.txt and SimpleProtocol.txt have to be copied to /h0/mpro on GC.

3.
In order to connect Desktop application to GC application, modify Address='10.120.9.182:18121' entry in Producer object in file SimpleProtocolSecondary.txt. 
Note: Modify only ip-address, the port number should be 18121. 

4. 
Desktop application declares itself as logserver. Upon connecting to GC application, the desktop application starts receiving log-messages from GC application. 
Log messages are sent via IPCLog() call in SimpleProtocol.cpp. 
If desktop application disconnects, the GC application redirects the log messages to its console. A file can be specified instead of console, but I did not want to complicate things. 
Note: same socket is used for sending/receiving requests, responces and log messages. 
The log messages are a plain unformatted text. Any text can be sent as log message. 
I attached the log messages to this application after the demo because I did not want to complicate already complicated things during the show. 

I optimized further the code so only 4 threads are created by GC application.




Overview:

IPCObject.dll represents communication object model. 
IPCClassFactory.dll represents serialization object model. 

SimpleProtocol.txt contains configuration of IPCObject. 
In order to set up IP-address of the TMA, modify Address field of the Subscriber object (line starting with >< Subscriber ). 
By default it set to local address (127.0.0.1) port 18120. 


The communication object model provided in this project is based on a concept of a Subscriber. 
A Subscriber is a wrapper around a socket. It defines the message types that can be sent via its socket. 
A Subscriber-object defines the second party of communication: ip address/port, the data types it can receive, the data types it will return. 
The second party will receive unsolicited messages. 
The second party sends a message back only in the context of received message: it sends a message as a response on received message. 
Therefore the second party will not send unsolicited messages to the first party. 
In other words, a send-receive transaction can be initiated only by the first party, that is the party that owns the Subscriber object. 

The Subscriber is quite limiting in its functionality, therefore a Producer was invented. 

A Producer also defines the second party of communication. It acts as a Subscriber from the owner's point of view. 
But it also injects a Subscriber to the second party. 
The Subscriber injected to the second party points to the first party. 
Once Producer finishes with its activation, both parties have a Subscriber-objects pointing to each other. 


A Subscriber therefore can be static or dynamic. 
The static subscriber is defined as a part of configuration of the program. 
The static subscriber can be Persistent. A persistent subscriber always tries to reestablish a broken connection. 
The dynamic Subscriber is the one that was injected by Producer. 

If a client wants to connect to a server, and then send requests and receive notifications, it needs only a Producer, one per server. 
A send-receive transactions can be simultaneously started on both ends. 
It means that a request send by one party will not be taken for a response by the other party. 



A Listener is an object that listens to connection requests. 
It is responsible for accepting connection requests and receiving messages. 
Listener owns a thread that monitors (select) its server-socket. 
It also monitors (select) all Subscribers except a static non-persistent Subscriber. 
Unsolicited messages sent through a Subscriber are received by Listener. 

Previously the only Subscribers monitored by Listener were those introduced by Producer. 
In order to accomodate the SimpleProtocol the Persistent Subscribers were added to Listener. 
This change affected the overall performance. The performance suffered mainly because additional synchronization objects (locks) had to be used. 


A Connector owns one Listener and many Subscribers and Producers. 






