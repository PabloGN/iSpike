#include <iSpike/Writer/WriterFactory.hpp>
#include "iSpike/ISpikeException.hpp"


/**
 * Default constructor
 * Initialises the list of writers, if you've made a new writer, add it here!
 */
WriterFactory::WriterFactory(){
	writerList.push_back(FileAngleWriter().getWriterDescription());
}


/** Constructor that also creates a YARP writer */
WriterFactory::WriterFactory(std::string ip, std::string port){
	writerList.push_back(FileAngleWriter().getWriterDescription());
	LOG(LOG_DEBUG) << "Probe1a";
	try{
		YarpAngleWriter yarpAngleWriter(ip, port);
		LOG(LOG_DEBUG) << "Probe2a";
		//writerList.push_back(yarpAngleWriter.getWriterDescription());
	}
	catch(ISpikeException& ex){
		LOG(LOG_DEBUG) << "Ispike excpetion thrown by yarp angle writer. Message: "<<ex.what();
	}
	catch(...){
		LOG(LOG_DEBUG) << "EXCEPTION THROWN::::::";
	}
	this->ip = ip;
	this->port = port;
}


/**
 * Returns all writers of a particular type
 * @param writerType The type of Writer we are interested in
 * @return All writers of the given type
 */
std::vector<WriterDescription> WriterFactory::getWritersOfType(std::string writerType){
	std::vector<WriterDescription> result;
	for(int i = 0; i < writerList.size(); i++){
		if(writerList[i].getWriterType() == writerType)
			result.push_back(writerList[i]);
	}
	return result;
}


/**
 * Creates a particular writer
 * @param writerName Type of a Writer we want to create
 * @param writerProperties A map of properties for the new Writer
 * @return An initialised Writer of a given type
 */
Writer* WriterFactory::create(std::string writerName, std::map<std::string,Property*> writerProperties	){
	Writer* result;
	if(writerName == "File Angle Writer") {
		result = new FileAngleWriter();
	}
	else if(writerName == "Yarp Angle Writer") {
		result = new YarpAngleWriter(this->ip, this->port);
	}
	else {
		throw std::logic_error("Invalid writer type");
	}
	result->initialise(writerProperties);
	return result;
}
