cd .\Debug
copy .\IPCObject\*.i *.*
copy .\IPCObject\OSSpecific\*.i *.*

E:\MWOSPPC\DOS\BIN\xcc -l=mt_sys_clib.l -l=mt_os_lib.l -l=mt_unix.l -l=mt_clib.l -l=mt_cplib.l -ldbglog.l -cd -cs=mt_cstart.r -epl -tp=PPC,lcd,ld -b -cw -qp -fd=IPCObject.x mt_cplib.l stdafx_ipcobject.i PollClient.i OSDependent_ipcobject.i IPCSockObject.i IPCObjectInterface.i IPCObject.i ClassesXRemoting.i ClassesXMedia.i 
libgen -co=IPCObject.l IPCObject.x
