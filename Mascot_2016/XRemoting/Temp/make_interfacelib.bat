cd .\Debug
copy .\IPCInterface\*.i *.*
copy .\IPCInterface\OSSpecific\*.i *.*

E:\MWOSPPC\DOS\BIN\xcc -l=mt_sys_clib.l -l=mt_os_lib.l -l=mt_unix.l -l=mt_clib.l -l=mt_cplib.l -ldbglog.l -cd -cs=mt_cstart.r -epl -tp=PPC,lcd,ld -b -cw -qp -fd=IPCInterface.x mt_cplib.l stdafx_interface.i RedirectCout.i OSDependent_interface.i IPCInterface.i 
libgen -co=IPCInterface.l IPCInterface.x
