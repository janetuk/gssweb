cd json_gssapi
del /f /s /q build
rd /s /q build
mkdir build
cd build
call setenv /x64 /Debug
cmake -DCMAKE_BUILD_TYPE=Debug -G "NMake Makefiles" ../
nmake -f Makefile
