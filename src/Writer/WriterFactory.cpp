//iSpike includes
#include <iSpike/Writer/WriterFactory.hpp>
#include "iSpike/ISpikeException.hpp"
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Writer/YarpAngleWriter.hpp>
#include <iSpike/Writer/FileAngleWriter.hpp>
using namespace ispike;

/** Default constructor
 * Initialises the list of writers, if you've made a new writer, add it here! */
WriterFactory::WriterFactory(){
	//Store list of available writers
	writerList.push_back(FileAngleWriter().getWriterDescription());
}


/** Constructor that also creates a YARP writer */
WriterFactory::WriterFactory(string ip, unsigned port){
	//Store list of available writers
	writerList.push_back(FileAngleWriter().getWriterDescription());
	YarpAngleWriter yarpAngleWriter(ip, port);
	writerList.push_back(yarpAngleWriter.getWriterDescription());

	//Store variables
	this->ip = ip;
	this->port = port;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Returns all writers of a particular type
 * @param writerType The type of Writer we are interested in
 * @return All writers of the given type */
vector<WriterDescription> WriterFactory::getWritersOfType(string writerType){
	vector<WriterDescription> result;
	for(int i = 0; i < writerList.size(); i++){
		if(writerList[i].getWriterType() == writerType)
			result.push_back(writerList[i]);
	}
	return result;
}


/** Creates a particular writer
 * @param writerName Type of a Writer we want to create
 * @param writerProperties A map of properties for the new Writer
 * @return An initialised Writer of a given type  */
Writer* WriterFactory::create(string writerName, map<string, Property> writerProperties	){
	Writer* result;
	if(writerName == "File Angle Writer") {
		result = new FileAngleWriter();
	}
	else if(writerName == "Yarp Angle Writer") {
		result = new YarpAngleWriter(this->ip, this->port);
	}
	else {
		throw logic_error("Invalid writer type");
	}
	result->initialise(writerProperties);
	return result;
}
