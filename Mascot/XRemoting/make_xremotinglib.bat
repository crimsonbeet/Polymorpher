cd .\Release
copy .\IPCGate\*.i *.*
copy .\IPCGate\OSSpecific\*.i *.*
copy .\IPCClassFactory\*.i *.*
copy .\IPCClassFactory\OSSpecific\*.i *.*
copy .\IPCInterface\*.i *.*
copy .\IPCInterface\OSSpecific\*.i *.*
copy .\IPCObject\*.i *.*
copy .\IPCObject\OSSpecific\*.i *.*

%MWOS%\DOS\BIN\xcc -il="b=1024" -beb=1000k -mt=safelibs -qnx -eil -l=mt_sys_clib.l -l=mt_os_lib.l -l=mt_unix.l -cs=mt_cstart.r -tp=PPC,lcd,ld -b -qp -fd=XRemoting.il stdafx_ipcobject.i PollClient.i OSDependent_ipcobject.i IPCSockObject.i IPCObjectInterface.i IPCObject.i ClassesXRemoting.i ClassesXMedia.i stdafx_classfactory.i WSIWriter.i WSISerializableStruct.i WSIParser.i WSIObject.i WSIMaterializer.i WSIMarshaller.i WSIClassFactory.i FepWriter.i FepParser.i DartWriter.i DartParser.i XMLWriter.i XMLParser.i IPCClassFactory.i WSIClassFactoryInterfaces.i OSDependent_classfactory.i stdafx_gate.i WSIGate.i ThreadPool.i OSDependent_gate.i IPCGate.i stdafx_interface.i OSDependent_interface.i IPCInterface.i RedirectCout.i HTTPParser.i HTTPWriter.i JSONParser.i JSONWriter.i

del *.i
rem del *.ii
del .\IPCGate\*.i 
del .\IPCGate\OSSpecific\*.i 
del .\IPCClassFactory\*.i 
del .\IPCClassFactory\OSSpecific\*.i 
del .\IPCInterface\*.i 
del .\IPCInterface\OSSpecific\*.i 
del .\IPCObject\*.i 
del .\IPCObject\OSSpecific\*.i 