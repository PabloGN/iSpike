/*
 * ReaderFactory.cpp
 *
 *  Created on: 13 Mar 2011
 *      Author: Edgars Lazdins
 */
#include <iSpike/Reader/ReaderFactory.hpp>
#include "iSpike/ISpikeException.hpp"

/**
 * Default constructor
 * Initialises the list of readers, if you've made a new reader, add it here!
 */
ReaderFactory::ReaderFactory(){
	LOG(LOG_DEBUG) << "Reader Before.";
	this->readerList.push_back(FileAngleReader().getReaderDescription());
	this->readerList.push_back(FileVisualReader().getReaderDescription());
	LOG(LOG_DEBUG) << "FileVisualReaderProperties: " << this->readerList.back().getReaderProperties().begin()->second->getName();
	LOG(LOG_DEBUG) << "Reader After ReaderList Length: " << this->readerList.size();
}


/** Constructor that creates YARP readers as well */
ReaderFactory::ReaderFactory(std::string ip, std::string port){
	this->readerList.push_back(FileAngleReader().getReaderDescription());
	this->readerList.push_back(FileVisualReader().getReaderDescription());
	LOG(LOG_DEBUG) << "Probe1";
	try{
		YarpAngleReader yarpAngleReader(ip, port);
		LOG(LOG_DEBUG) << "Probe2";
		this->readerList.push_back(yarpAngleReader.getReaderDescription());

		LOG(LOG_DEBUG) << "Probe3";
		YarpVisualReader yarpVisualReader(ip, port);
		LOG(LOG_DEBUG) << "Probe4";
		this->readerList.push_back(yarpVisualReader.getReaderDescription());
	}
	catch(ISpikeException& ex){
		LOG(LOG_DEBUG) << "Ispike excpetion thrown by yarp angle reader. Message: " << ex.what();
		return;
	}
	catch(...){
		LOG(LOG_DEBUG) << "EXCEPTION THROWN::::::";
	}

	this->ip = ip;
	this->port = port;
}


/** Returns readers of a particular type */
std::vector<ReaderDescription> ReaderFactory::getReadersOfType(std::string readerType){
	LOG(LOG_DEBUG) << "getReadersOfType: " << this->readerList.size();
	std::vector<ReaderDescription> result;
	for(int i = 0; i < readerList.size(); i++)
	{
		LOG(LOG_DEBUG) << readerList[i].getReaderProperties().begin()->second->getName();
		if(readerList[i].getReaderType() == readerType)
			result.push_back(readerList[i]);
	}
	return result;
}


/**
 * Creates and initialises a particular reader
 * @param readerName Type of a Reader we want to create
 * @param readerProperties Initialisation properties for the new Reader
 * @return A new Reader
 */
Reader* ReaderFactory::create(std::string readerName, property_map readerProperties){
	Reader* result;
	if(readerName == "File Angle Reader") {
		result = new FileAngleReader();
	}
	else if(readerName == "File Visual Reader")  {
		result = new FileVisualReader();
	}
	else if(readerName == "Yarp Angle Reader") {
		result = new YarpAngleReader(this->ip, this->port);
	}
	else if(readerName == "Yarp Visual Reader"){
		result = new YarpVisualReader(this->ip, this->port);
	}
	else {
		throw std::logic_error("Invalid reader type");
	}
	result->initialise(readerProperties);
	return result;
}

