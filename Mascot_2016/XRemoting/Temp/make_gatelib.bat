cd .\Debug
copy .\IPCGate\*.i *.*
copy .\IPCGate\OSSpecific\*.i *.*

E:\MWOSPPC\DOS\BIN\xcc -l=mt_sys_clib.l -l=mt_os_lib.l -l=mt_unix.l -l=mt_clib.l -l=mt_cplib.l -ldbglog.l -cd -cs=mt_cstart.r -epl -tp=PPC,lcd,ld -b -cw -qp -fd=IPCGate.x mt_cplib.l stdafx_gate.i WSIGate.i ThreadPool.i OSDependent_gate.i IPCGate.i
libgen -co=IPCGate.l IPCGate.x
