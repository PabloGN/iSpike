//iSpike includes
#include <iSpike/Reader/ReaderFactory.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/Reader/YarpVisualReader.hpp>
#include "iSpike/ISpikeException.hpp"
#include "iSpike/Log/Log.hpp"
using namespace ispike;

//Other includes
#include <iostream>
using namespace std;

/** Default constructor
 * Initialises the list of readers, if you've made a new reader, add it here! */
ReaderFactory::ReaderFactory(){
	this->readerList.push_back(FileAngleReader().getReaderDescription());
	this->readerList.push_back(FileVisualReader().getReaderDescription());
	printReaders();
}


/** Constructor that creates YARP readers as well */
ReaderFactory::ReaderFactory(string ip, unsigned port){
	this->readerList.push_back(FileAngleReader().getReaderDescription());
	this->readerList.push_back(FileVisualReader().getReaderDescription());
	this->readerList.push_back(YarpAngleReader(ip, port).getReaderDescription());
	this->readerList.push_back(YarpVisualReader(ip, port).getReaderDescription());
	printReaders();

	this->ip = ip;
	this->port = port;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Returns readers of a particular type */
vector<Description> ReaderFactory::getReadersOfType(string readerType){
	vector<Description> result;
	for(unsigned i = 0; i < readerList.size(); i++) {
		if(readerList[i].getType() == readerType)
			result.push_back(readerList[i]);
	}
	return result;
}


/*! Returns the default properties of a particular reader */
map<string, Property> ReaderFactory::getDefaultProperties(Description& desc){
	if(desc.getName() == "File Angle Reader") {
		return FileAngleReader().getProperties();
	}
	else if(desc.getName() == "File Visual Reader"){
		return FileVisualReader().getProperties();
	}
	else if(desc.getName() == "Yarp Angle Reader") {
		return YarpAngleReader(ip, port).getProperties();
	}
	else if(desc.getName() == "Yarp Visual Reader"){
		return YarpVisualReader(ip, port).getProperties();
	}
	throw ISpikeException("Invalid writer");
}


/** Creates and initialises a particular reader */
Reader* ReaderFactory::create(Description& desc, map<string, Property>& readerProperties){
	Reader* result;
	if(desc.getName() == "File Angle Reader") {
		result = new FileAngleReader();
	}
	else if(desc.getName() == "File Visual Reader"){
		result = new FileVisualReader();
	}
	else if(desc.getName() == "Yarp Angle Reader") {
		result = new YarpAngleReader(ip, port);
	}
	else if(desc.getName() == "Yarp Visual Reader"){
		result = new YarpVisualReader(ip, port);
	}
	else {
		throw ISpikeException("Invalid reader type");
	}
	result->initialize(readerProperties);
	return result;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Prints out the available readers */
void ReaderFactory::printReaders(){
	for(size_t i=0; i<readerList.size(); ++i)
		LOG(LOG_DEBUG)<<"Reader: "<<readerList[i].getName()<<", "<<readerList[i].getDescription();
}

