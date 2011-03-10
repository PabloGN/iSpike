cd ./build
del * /Q
REM cmake -G "MinGW Makefiles" ../
cmake -G "Visual Studio 9 2008" ../
