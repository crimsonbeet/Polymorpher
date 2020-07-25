cd .\Debug
copy .\Main\*.i *.*

E:\MWOSPPC\DOS\BIN\xcc -il="b=20000" -l=WSIClassFactory.l -mt=safelibs -qnx -l=d_mt_sys_clib.l -l=d_mt_unix.l -l=mt_socket.l -cs=mt_cstart.r -tp=PPC,lcd,ld -b -cw -qp -olmtq -olM=512 -olp=777 -olgu=1.0 -fd=Main.exe Main.i WSIClassFactory.il
