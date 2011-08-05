//iSpike includes
#include <iSpike/Reader/ReaderFactory.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/Reader/YarpVisualReader.hpp>
#include "iSpike/ISpikeException.hpp"
using namespace ispike;

/** Default constructor
 * Initialises the list of readers, if you've made a new reader, add it here! */
ReaderFactory::ReaderFactory(){
	this->readerList.push_back(FileAngleReader().getReaderDescription());
	this->readerList.push_back(FileVisualReader().getReaderDescription());
}


/** Constructor that creates YARP readers as well */
ReaderFactory::ReaderFactory(string ip, unsigned port){
	this->readerList.push_back(FileAngleReader().getReaderDescription());
	this->readerList.push_back(FileVisualReader().getReaderDescription());

	YarpAngleReader yarpAngleReader(ip, port);
	this->readerList.push_back(yarpAngleReader.getReaderDescription());

	YarpVisualReader yarpVisualReader(ip, port);
	this->readerList.push_back(yarpVisualReader.getReaderDescription());

	this->ip = ip;
	this->port = port;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Returns readers of a particular type */
vector<Description> ReaderFactory::getReadersOfType(string readerType){
	vector<Description> result;
	for(int i = 0; i < readerList.size(); i++) {
		if(readerList[i].getType() == readerType)
			result.push_back(readerList[i]);
	}
	return result;
}


/** Creates and initialises a particular reader */
Reader* ReaderFactory::create(string readerName, map<string, Property>& readerProperties){
	Reader* result;
	if(readerName == "File Angle Reader") {
		result = new FileAngleReader();
	}
	else if(readerName == "File Visual Reader"){
		result = new FileVisualReader();
	}
	else if(readerName == "Yarp Angle Reader") {
		result = new YarpAngleReader(ip, port);
	}
	else if(readerName == "Yarp Visual Reader"){
		result = new YarpVisualReader(ip, port);
	}
	else {
		throw std::logic_error("Invalid reader type");
	}
	result->initialize(readerProperties);
	return result;
}

