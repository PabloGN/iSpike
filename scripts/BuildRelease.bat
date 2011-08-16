:: Set release directory
SET releaseDirectory=release\ispike-2.0

:: Set location of iSpike library - this varies depending on CMake configuration
SET iSpikeLibraryDirectory=src

:: Delete current release
rmdir  %releaseDirectory% /s /q
mkdir %releaseDirectory%
mkdir %releaseDirectory%\build

:: Copy library to release
mkdir %releaseDirectory%\lib
copy %iSpikeLibraryDirectory%\libiSpike.dll %releaseDirectory%\lib\libiSpike.dll /Y

:: Third party libraries
xcopy thirdparty\lib\*.dll %releaseDirectory%\thirdparty\ /s

:: Copy license and readme
copy LICENSE %releaseDirectory%\LICENSE /Y
copy README %releaseDirectory%\README /Y

:: Copy include files to release directory
xcopy include\*.hpp  %releaseDirectory%\include\ /s
xcopy include\*.h  %releaseDirectory%\include\ /s

:: Copy src files to release directory
xcopy src\*.cpp %releaseDirectory%\src\ /s

:: Copy CMake files
copy CMakeLists.txt %releaseDirectory%\CMakeLists.txt /Y
copy src\CMakeLists.txt %releaseDirectory%\src\CMakeLists.txt /Y
copy libraryinfo.h.in.cmake %releaseDirectory%\libraryinfo.h.in.cmake /Y

:: Documentation
mkdir %releaseDirectory%\doc
xcopy docs\html %releaseDirectory%\doc\html\ /s
xcopy docs\man %releaseDirectory%\doc\man\ /s
copy docs\LazdinsFidjelandGamez_iSpike.pdf %releaseDirectory%\doc\LazdinsFidjelandGamez_iSpike.pdf /Y
copy docs\ispike-manual-2.0.pdf %releaseDirectory%\doc\ispike-manual-2.0.pdf /Y

