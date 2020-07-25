cd .\Debug
copy .\IPCGate\*.i *.*
copy .\IPCGate\OSSpecific\*.i *.*
copy .\IPCClassFactory\*.i *.*
copy .\IPCClassFactory\OSSpecific\*.i *.*

E:\MWOSPPC\DOS\BIN\xcc -il="b=20000" -mt=safelibs -qnx -epl -g -l=mt_sys_clib.l -l=mt_os_lib.l -l=mt_unix.l -cs=mt_cstart.r -tp=PPC,lcd,ld -b -cw -qp -fd=WSIClassFactory.x stdafx_classfactory.i WSIWriter.i WSIParser.i WSIObject.i WSIMaterializer.i WSIMarshaller.i WSIClassFactory.i WSIClassFactoryInterfaces.i OSDependent_classfactory.i
libgen -co=WSIClassFactory.l WSIClassFactory.x

E:\MWOSPPC\DOS\BIN\xcc -il="b=20000" -mt=safelibs -qnx -eil -g -l=mt_sys_clib.l -l=mt_os_lib.l -l=mt_unix.l -cs=mt_cstart.r -tp=PPC,lcd,ld -b -cw -qp -fd=WSIClassFactory.il stdafx_classfactory.i WSIWriter.i WSIParser.i WSIObject.i WSIMaterializer.i WSIMarshaller.i WSIClassFactory.i WSIClassFactoryInterfaces.i OSDependent_classfactory.i

