:: Set release directory
SET releaseDirectory=releases\2.1

:: Set location of iSpike library - this varies depending on CMake configuration
SET iSpikeLibraryDirectory=build\src

:: Delete current release
rmdir  %releaseDirectory% /s /q
mkdir %releaseDirectory%
mkdir %releaseDirectory%\build

:: Copy library to release
mkdir %releaseDirectory%\lib
copy %iSpikeLibraryDirectory%\libiSpike.dll %releaseDirectory%\lib\libiSpike.dll /Y

:: Copy license and readme
copy COPYING %releaseDirectory%\COPYING /Y
copy README %releaseDirectory%\README /Y
copy LICENSE %releaseDirectory%\LICENSE /Y

:: Copy include files to release directory
xcopy include\*.hpp  %releaseDirectory%\include\ /s
xcopy include\*.h  %releaseDirectory%\include\ /s

:: Copy src files to release directory
xcopy src\*.cpp %releaseDirectory%\src\ /s

:: Copy CMake files
mkdir %releaseDirectory%\docs
copy CMakeLists.txt %releaseDirectory%\CMakeLists.txt /Y
copy src\CMakeLists.txt %releaseDirectory%\src\CMakeLists.txt /Y
copy docs\CMakeLists.txt %releaseDirectory%\docs\CMakeLists.txt /Y
copy libraryinfo.h.in.cmake %releaseDirectory%\libraryinfo.h.in.cmake /Y

:: Documentation
xcopy docs\api\html %releaseDirectory%\docs\api\ /s
copy docs\ispike-manual-2.1.pdf %releaseDirectory%\docs\ispike-manual-2.1.pdf /Y

