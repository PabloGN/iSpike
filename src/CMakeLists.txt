cmake_minimum_required(VERSION 2.8)

# Set Boost location
SET (BOOST_ROOT "C:/Users/daogamez/Home/Programs/spikestream/trunk/extlib/boost")

SET (Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0" "1.40" "1.40.0" "1.41" "1.41.0" "1.42" "1.42.0" "1.43" "1.43.0" "1.44" "1.44.0" "1.45" "1.45.0" "1.46" "1.46.0" "1.47" "1.47.0" "1.48" "1.48.0" "1.49" "1.49.0" "1.50" "1.50.0")
# Minimum library versions:
#
# system: 1.30.0
# regex: 1.18.0
# thread: 1.25.0
#
FIND_PACKAGE (Boost 1.30.0 COMPONENTS "system" "regex" "thread" REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
SET(Boost_USE_MULTITHREADED ON)

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

SET(project_SRCS
		Channel/InputChannel/JointInputChannel.cpp
		Channel/InputChannel/VisualInputChannel.cpp
		Channel/InputChannel/InputChannelFactory.cpp
		Channel/OutputChannel/JointOutputChannel.cpp
		Channel/OutputChannel/OutputChannelFactory.cpp
		Common.cpp
		Log/Log.cpp
		NeuronSim/IzhikevichNeuronSim.cpp
		Reader/FileAngleReader.cpp
		Reader/FileVisualReader.cpp
		Reader/ReaderFactory.cpp
		Reader/YarpAngleReader.cpp
		Reader/YarpVisualReader.cpp
		VisualDataReducer/LogPolarVisualDataReducer.cpp
		VisualFilter/DOGVisualFilter.cpp
		Writer/FileAngleWriter.cpp
		Writer/WriterFactory.cpp
		Writer/YarpAngleWriter.cpp
		Description.cpp
		Bitmap.cpp 
		Property.cpp
		PropertyHolder.cpp
		YarpConnection.cpp
		YarpPortDetails.cpp
		version.cpp
	)

IF (${ISPIKE_BUILD_STATIC_LIB})
	ADD_LIBRARY(iSpike_static STATIC ${project_SRCS})
	TARGET_LINK_LIBRARIES(iSpike_static ${project_LIBS})
	SET_TARGET_PROPERTIES(iSpike_static
		PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}"
		DEFINE_SYMBOL BOOST_THREAD_USE_LIB
		CLEAN_DIRECT_OUTPUT 1)
	INSTALL(TARGETS iSpike_static DESTINATION lib)
ENDIF(${ISPIKE_BUILD_STATIC_LIB})

IF (${ISPIKE_BUILD_SHARED_LIB})
	ADD_LIBRARY(iSpike SHARED ${project_SRCS})
	TARGET_LINK_LIBRARIES(iSpike ${project_LIBS})
	SET_TARGET_PROPERTIES(iSpike
		PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}"
		DEFINE_SYMBOL BOOST_THREAD_USE_DLL
		CLEAN_DIRECT_OUTPUT 1)
	INSTALL(TARGETS iSpike DESTINATION lib)
ENDIF(${ISPIKE_BUILD_SHARED_LIB})
