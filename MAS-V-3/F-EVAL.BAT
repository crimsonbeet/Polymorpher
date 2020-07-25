set __oinc=%include%
set include=e:\zortech\include
rem ztc -o+time -cpp -mx -bx -a1 -3 f-eval.cpp tom_aray.cpp fqeval.cpp fxeval.cpp dxbout.cpp _scale.cpp
ztc -cpp -mx -bx -a1 -3 f-eval.cpp tom_aray.cpp fqeval.cpp fxeval.cpp dxbout.cpp _scale.cpp
set include=%__oinc%
set __oinc=
