cd .\Debug
copy .\SimpleProtocol\*.i *.*

E:\MWOSPPC\DOS\BIN\xcc -il="b=20000" -mt=safelibs -qnx -g -l=d_mt_sys_clib.l -l=d_mt_unix.l -l=mt_socket.l -cs=mt_cstart.r -tp=PPC,lcd,ld -b -cw -qp -olmtq -olM=512 -olp=777 -olgu=1.0 -fd=SimpleProtocol.exe stdafx_simpleprotocol.i SimpleProtocol.i XRemoting.il


REM -l=mt_clib.l -l=mt_cplib.l 

REM -l=XRemoting.l -ldbglog.l

REM -l=mt_os_lib.l 

REM -l=XRemoting.l 

REM  -il=lXRemoting.il