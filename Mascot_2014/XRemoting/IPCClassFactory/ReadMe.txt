========================================================================
    DYNAMIC LINK LIBRARY : IPCClassFactory Project Notes
========================================================================

2008-05-16 11:40 PM

Fighting Linux shared library issues. 

1) global objects.

CWsiCollection with static member should be renamed to CWsiBaseCollection.
Only Get, and Insert member functions are left of old member functions. 

CWsiRootSerializer will still insert itself to that static member of CWsiBaseCollection. 

New CWsiCollection class will have non static map member. 
It will have Get, and Merge memeber functions. 
Merge member function will be used when XRemoting class initializes itself. 
XRemoting will have CWsiCollection member, initially empty. 
During initialization phase the XRemoting copies(merges) content of the base collection to its local collection. 

CWsiRootSerializer will have an emty Init function, so it will not build member serializers. ???
CWsiRootSerializer will have Create function that creates normal CWsiSafeSerializer. 
The Create function will be used by CWsiCollection::Merge function (non static), 
so the new collection will have local thread safe serializers.

??? 2008-05-17 11:20 AM 
CWSIMemCallback and CWSICallback still require a fully operational root serializer. 


