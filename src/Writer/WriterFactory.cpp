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
}


/** Constructor that also creates a YARP writer */
WriterFactory::WriterFactory(string ip, unsigned port){
	//Store list of available writers
	writerList.push_back(FileAngleWriter().getWriterDescription());
	YarpAngleWriter yarpAngleWriter(ip, port);
	writerList.push_back(yarpAngleWriter.getWriterDescription());

	//Store variables
	ip = ip;
	port = port;
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


/** Creates a particular writer   */
Writer* WriterFactory::create(string writerName, map<string, Property>& writerProperties	){
	Writer* result;
	if(writerName == "File Angle Writer") {
		result = new FileAngleWriter();
	}
	else if(writerName == "Yarp Angle Writer") {
		result = new YarpAngleWriter(this->ip, this->port);
	}
	else {
		throw ISpikeException("Invalid writer name");
	}
	result->initialize(writerProperties);
	return result;
}
