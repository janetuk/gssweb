del /f /s /q build
rd /s /q build
mkdir build
cd build
call setenv /x86 /Debug
call c:\firefox-addon-sdk\bin\activate.bat
cmake -DCMAKE_BUILD_TYPE=Debug -G "NMake Makefiles" ../
nmake -f Makefile
