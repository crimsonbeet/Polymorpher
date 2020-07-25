set _NT_SYMBOL_PATH=srv*C:\XRemoting\symstore.pri
set _NT_SOURCE_PATH=srv*C:\XRemoting\symstore.pri

set MY_PROGRAM_NAME=C:\Debug\TMMT22\SOURCES\Debug\TESTVectors.exe
set MY_PROGRAM_ARGUMENT=C:\Debug\TMMT22\SOURCES\TESTVectors

call RemoteDebug.bat

rem start /B windbg.exe -n -Q -premote tcp:server=10.120.9.167,port=5001
exit
