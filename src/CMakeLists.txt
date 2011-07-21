cmake_minimum_required(VERSION 2.8)

SET (Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0" "1.40" "1.40.0" "1.41" "1.41.0" "1.42" "1.42.0" "1.43" "1.43.0" "1.44" "1.44.0" "1.45" "1.45.0" "1.46" "1.46.0" "1.47" "1.47.0" "1.48" "1.48.0" "1.49" "1.49.0" "1.50" "1.50.0")
FIND_PACKAGE (Boost 1.42 COMPONENTS "system" "regex" "thread" REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
SET(Boost_USE_MULTITHREADED ON)

SET(project_SRCS
		Channel/Channel.cpp
		Channel/InputChannel/InputChannelDescription.cpp
		Channel/InputChannel/InputChannel.cpp
		Channel/InputChannel/JointInputChannel.cpp
		Channel/InputChannel/VisualInputChannel.cpp
		Channel/InputChannel/InputChannelFactory.cpp
		Channel/OutputChannel/OutputChannelDescription.cpp 
		Channel/OutputChannel/JointOutputChannel.cpp
		Channel/OutputChannel/OutputChannel.cpp
		Channel/OutputChannel/OutputChannelFactory.cpp
		Common.cpp
		Log/Log.cpp
		NeuronSim/IzhikevichNeuronSim.cpp
		Reader/ConsoleAngleReader.cpp
		Reader/FileAngleReader.cpp
		Reader/FileVisualReader.cpp
		Reader/Reader.cpp
		Reader/ReaderFactory.cpp
		Reader/YarpAngleReader.cpp
		Reader/YarpAudioReader.cpp
		Reader/YarpTextReader.cpp
		Reader/YarpVisualReader.cpp
		Reader/ReaderDescription.cpp
		VisualDataReducer/LogpolarVisualDataReducer.cpp
		VisualFilter/DOGVisualFilter.cpp
		VisualFilter/MovementFilter.cpp
		Writer/FileAngleWriter.cpp
		Writer/WriterFactory.cpp
		Writer/YarpAngleWriter.cpp
		Writer/WriterDescription.cpp
		Bitmap.cpp 
		Property.cpp
		PropertyHolder.cpp
		YarpConnection.cpp
		YarpPortDetails.cpp
		version.cpp
	)


IF(WIN32)
    SET (project_LIBS ${Boost_LIBRARIES} ${Log4Cxx_LIBRARIES} "ws2_32")
ELSE()
    SET (project_LIBS ${Boost_LIBRARIES} ${Log4Cxx_LIBRARIES})
ENDIF()

IF (NOT DEFINED project_build_shared)
   IF (CMAKE_CROSSCOMPILING)
       SET(project_build_shared 0)
   ELSE (CMAKE_CROSSCOMPILING)
       SET(project_build_shared 1)
   ENDIF (CMAKE_CROSSCOMPILING)
ENDIF (NOT DEFINED project_build_shared)

OPTION(ISPIKE_BUILD_STATIC_LIB "Create a static library version of iSpike" FALSE)
OPTION(ISPIKE_BUILD_SHARED_LIB "Create a shared library version of iSPike" TRUE)

IF (ISPIKE_BUILD_STATIC_LIB)
	ADD_LIBRARY(iSpike_static STATIC ${project_SRCS})
	TARGET_LINK_LIBRARIES(iSpike_static ${project_LIBS})
	SET_TARGET_PROPERTIES(iSpike_static
		PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}"
		CLEAN_DIRECT_OUTPUT 1)
	INSTALL(TARGETS iSpike_static DESTINATION lib)
ENDIF(ISPIKE_BUILD_STATIC_LIB)

IF (ISPIKE_BUILD_SHARED_LIB)
	ADD_LIBRARY(iSpike SHARED ${project_SRCS})
	TARGET_LINK_LIBRARIES(iSpike ${project_LIBS})
	SET_TARGET_PROPERTIES(iSpike
		PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}"
		CLEAN_DIRECT_OUTPUT 1)
	INSTALL(TARGETS iSpike DESTINATION lib)
ENDIF(ISPIKE_BUILD_SHARED_LIB)
