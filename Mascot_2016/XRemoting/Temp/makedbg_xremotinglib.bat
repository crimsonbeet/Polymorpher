cd .\Debug
copy .\IPCGate\*.i *.*
copy .\IPCGate\OSSpecific\*.i *.*
copy .\IPCClassFactory\*.i *.*
copy .\IPCClassFactory\OSSpecific\*.i *.*
copy .\IPCInterface\*.i *.*
copy .\IPCInterface\OSSpecific\*.i *.*
copy .\IPCObject\*.i *.*
copy .\IPCObject\OSSpecific\*.i *.*
copy .\SimpleProtocol\*.i *.*

E:\MWOSPPC\DOS\BIN\xcc -il="b=20000" -mt=safelibs -qnx -epl -g -l=mt_sys_clib.l -l=mt_os_lib.l -l=mt_unix.l -cs=mt_cstart.r -tp=PPC,lcd,ld -b -cw -qp -fd=XRemoting.x stdafx_ipcobject.i PollClient.i OSDependent_ipcobject.i IPCSockObject.i IPCObjectInterface.i IPCObject.i ClassesXRemoting.i ClassesXMedia.i stdafx_classfactory.i WSIWriter.i WSISerializableStruct.i WSIParser.i WSIObject.i WSIMaterializer.i WSIMarshaller.i WSIClassFactory.i SimpleWriter.i SimpleParser.i IPCClassFactory.i WSIClassFactoryInterfaces.i OSDependent_classfactory.i stdafx_gate.i WSIGate.i ThreadPool.i OSDependent_gate.i IPCGate.i stdafx_interface.i OSDependent_interface.i IPCInterface.i RedirectCout.i SimpleXClasses.i
libgen -co=XRemoting.l XRemoting.x

E:\MWOSPPC\DOS\BIN\xcc -il="b=20000" -mt=safelibs -qnx -eil -g -l=mt_sys_clib.l -l=mt_os_lib.l -l=mt_unix.l -cs=mt_cstart.r -tp=PPC,lcd,ld -b -cw -qp -fd=XRemoting.il stdafx_ipcobject.i PollClient.i OSDependent_ipcobject.i IPCSockObject.i IPCObjectInterface.i IPCObject.i ClassesXRemoting.i ClassesXMedia.i stdafx_classfactory.i WSIWriter.i WSISerializableStruct.i WSIParser.i WSIObject.i WSIMaterializer.i WSIMarshaller.i WSIClassFactory.i SimpleWriter.i SimpleParser.i IPCClassFactory.i WSIClassFactoryInterfaces.i OSDependent_classfactory.i stdafx_gate.i WSIGate.i ThreadPool.i OSDependent_gate.i IPCGate.i stdafx_interface.i OSDependent_interface.i IPCInterface.i RedirectCout.i SimpleXClasses.i

REM  "e:\MWOSPPC\OS9000\PPC\LIB\HOST3\mt_cplibnx.il"
REM  "e:\MWOSPPC\OS9000\PPC\LIB\HOST3\mt_cplibnx.il"
REM -l=mt_clib.l -l=mt_cplib.l -ldbglog.l 
REM -l=mt_clib.l -l=mt_cplib.l -ldbglog.l 
REM  -il=m
REM -cd
REM 