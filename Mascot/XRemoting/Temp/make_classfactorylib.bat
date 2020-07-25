cd .\Release
copy .\IPCGate\*.i *.*
copy .\IPCGate\OSSpecific\*.i *.*
copy .\IPCClassFactory\*.i *.*
copy .\IPCClassFactory\OSSpecific\*.i *.*

E:\MWOSPPC\DOS\BIN\xcc -il="b=20000" -mt=safelibs -qnx -epl -l=mt_sys_clib.l -l=mt_os_lib.l -l=mt_unix.l -cs=mt_cstart.r -tp=PPC,lcd,ld -b -cw -qp -fd=WSIClassFactory.x stdafx_classfactory.i WSIWriter.i WSISerializableStruct.i WSIParser.i WSIObject.i WSIMaterializer.i WSIMarshaller.i WSIClassFactory.i SimpleWriter.i SimpleParser.i IPCClassFactory.i WSIClassFactoryInterfaces.i OSDependent_classfactory.i stdafx_gate.i WSIGate.i ThreadPool.i OSDependent_gate.i IPCGate.i
libgen -co=IPCClassFactory.l IPCClassFactory.x

E:\MWOSPPC\DOS\BIN\xcc -il="b=20000" -mt=safelibs -qnx -eil -l=mt_sys_clib.l -l=mt_os_lib.l -l=mt_unix.l -cs=mt_cstart.r -tp=PPC,lcd,ld -b -cw -qp -fd=WSIClassFactory.il stdafx_classfactory.i WSIWriter.i WSISerializableStruct.i WSIParser.i WSIObject.i WSIMaterializer.i WSIMarshaller.i WSIClassFactory.i SimpleWriter.i SimpleParser.i IPCClassFactory.i WSIClassFactoryInterfaces.i OSDependent_classfactory.i stdafx_gate.i WSIGate.i ThreadPool.i OSDependent_gate.i IPCGate.i

