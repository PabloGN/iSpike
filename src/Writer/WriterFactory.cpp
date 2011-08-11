//iSpike includes
#include "iSpike/ISpikeException.hpp"
#include <iSpike/Writer/WriterFactory.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Writer/YarpAngleWriter.hpp>
#include <iSpike/Writer/FileAngleWriter.hpp>
using namespace ispike;

/** Default constructor
 * Initialises the list of writers, if you've made a new writer, add it here! */
WriterFactory::WriterFactory(){
	//Store list of available writers
	writerList.push_back(FileAngleWriter().getWriterDescription());
	printWriters();
}


/** Constructor that also creates a YARP writer */
WriterFactory::WriterFactory(string ip, unsigned port){
	//Store list of available writers
	writerList.push_back(FileAngleWriter().getWriterDescription());
	writerList.push_back(YarpAngleWriter(ip, port).getWriterDescription());
	printWriters();

	//Store variables
	this->ip = ip;
	this->port = port;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Returns all writers of a particular type */
vector<Description> WriterFactory::getWritersOfType(string writerType){
	vector<Description> result;
	for(int i = 0; i < writerList.size(); i++){
		if(writerList[i].getType() == writerType)
			result.push_back(writerList[i]);
	}
	return result;
}


/*! Returns the default properties of a particular writer */
map<string, Property> WriterFactory::getDefaultProperties(Description& desc){
	if(desc.getName() == "File Angle Writer") {
		return FileAngleWriter().getProperties();
	}
	else if(desc.getName() == "Yarp Angle Writer") {
		return YarpAngleWriter(this->ip, this->port).getProperties();
	}
	throw ISpikeException("Invalid writer");
}


/** Creates a particular writer   */
Writer* WriterFactory::create(Description& desc, map<string, Property>& writerProperties){
	Writer* result;
	if(desc.getName() == "File Angle Writer") {
		result = new FileAngleWriter();
	}
	else if(desc.getName() == "Yarp Angle Writer") {
		result = new YarpAngleWriter(this->ip, this->port);
	}
	else {
		throw ISpikeException("Invalid writer");
	}
	result->initialize(writerProperties);
	return result;
}


/*--------------------------------------------------------------------*/
/*---------                 PRIVATE METHODS                    -------*/
/*--------------------------------------------------------------------*/

/** Prints out the available writers */
void WriterFactory::printWriters(){
	for(size_t i=0; i<writerList.size(); ++i)
		cout<<"Writer: "<<writerList[i].getName()<<", "<<writerList[i].getDescription()<<endl;
}
