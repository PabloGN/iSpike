:: Locations to use
SET SpikeStreamRoot=D:\Home\Programs\spikestream\trunk

:: Copy library to external library location
copy lib\libiSpike.dll %SpikeStreamRoot%\extlib\ispike\lib\libiSpike.dll /Y

:: Copy libray to bin location
copy lib\libiSpike.dll  %SpikeStreamRoot%\bin\libiSpike.dll /Y

:: Delete include files from external library
rmdir  %SpikeStreamRoot%\extlib\ispike\include\iSpike /s /q

:: Copy include files to external library
xcopy include\*.hpp  %SpikeStreamRoot%\extlib\ispike\include /s
