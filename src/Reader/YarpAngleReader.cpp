/*
 * YarpAngleReader.cpp
 *
 *  Created on: 22 Feb 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>

/*!
 * The default constructor, only initialises the default parameters and the description
 */
YarpAngleReader::YarpAngleReader(std::string nameserverIP, std::string nameserverPort){
	//	throw ISpikeException("TEST YARP ANGLE READER EXCEPTION");

	this->initialised = false;
	// First define the properties of this reader
	// Get the available yarp ports
	LOG(LOG_DEBUG) << "before yarp connection";
	this->yarpConnection = new YarpConnection(nameserverIP, nameserverPort);
	LOG(LOG_DEBUG) << "before getting portmap";
	this->portMap = this->yarpConnection->getPortMap();

	//Iterate over them and add as options
	LOG(LOG_DEBUG) << "iterating";
	std::map<std::string, YarpPortDetails*>::iterator iter;
	std::vector<std::string> yarpPortNames;
	for (iter = this->portMap->begin(); iter != this->portMap->end(); iter++){
		yarpPortNames.push_back(iter->first);
	}
	LOG(LOG_DEBUG) << "filling in properties";

	property_map properties;
	properties["Port Name"] = boost::shared_ptr<Property>(
			new ComboProperty("Port Name",
				"/icubSim/left_arm/state:o",
				"The Yarp Port name",
				yarpPortNames,
				true));

	//Now let's create the description
	this->readerDescription = new ReaderDescription("Yarp Angle Reader", "This is a Yarp angle reader",	"Angle Reader", properties);
	LOG(LOG_DEBUG) << "exiting";
}


/*! Destructor */
YarpAngleReader::~YarpAngleReader(){
	LOG(LOG_DEBUG) << "destroying angle reader";
	if(isRunning()){
		requestStop();
		threadPointer->join();
	}
	LOG(LOG_DEBUG) << "destruction complete";
}


/**
 * Retrieves the vector of joint angles
 */
std::vector<double> YarpAngleReader::getData(){
	boost::mutex::scoped_lock lock(this->mutex);
	return buffer;
}


/**
 * Initialises the Reader with the default properties
 */
void YarpAngleReader::initialise(){
	YarpAngleReader::initialise(this->getReaderDescription().getReaderProperties());
	buffer.clear();
	LOG(LOG_DEBUG) << "INITIALIZATION FUNCTION 1 - WHAT IS GOING ON HERE";
}


/**
 * Creates a new empty buffer and initialises the port name
 */
void YarpAngleReader::initialise(property_map properties){
	this->setPortName(static_cast<ComboProperty*>(properties["Port Name"].get())->getValue());
	LOG(LOG_DEBUG) << "INITIALIZATION FUNCTION 2. YARP Port: " << this->getPortName();
	buffer.clear();
	this->initialised = false;
}


/** Function passed to thread - similar to run() method */
void YarpAngleReader::workerFunction(){
	setRunning(true);
	clearError();
	try{
		//Connect to port
		std::map<std::string, YarpPortDetails*>::iterator iter = this->portMap->find(this->getPortName());
		std::string ip;
		std::string port;
		std::string type;
		if (iter != this->portMap->end() ) {
			ip = iter->second->getIp();
			port = iter->second->getPort();
			type = iter->second->getType();
			LOG(LOG_INFO) << "YarpAngleReader: Yarp Port IP: " << ip << " Port: " << port;
		}
		else {
			throw ISpikeException("YarpAngleReader: Yarp IP/Port map is empty!");
		}
		this->yarpConnection->connect_to_port(ip, port);
		this->yarpConnection->prepare_to_read_text();
		while(!isStopRequested()){
			//std::string contentLine = this->yarpConnection->read_until("\n");
			//Get angle data from YARP server
			std::string contentLine = this->yarpConnection->getSocketString();

			//Lock mutex while buffer is being filled
			boost::mutex::scoped_lock lock(this->mutex);

			//Fill buffer with new data
			buffer.clear();
			if(contentLine.length() > 0){
				boost::regex splitString("\\s+");
				std::list<std::string> lines;
				boost::regex_split(std::back_inserter(lines), contentLine, splitString);
				lines.pop_front();
				lines.pop_front();
				while(lines.size() > 0)	{
					std::string current_string = *(lines.begin());
					lines.pop_front();
					double angle = 0;
					try{
						angle = boost::lexical_cast<double>(current_string);
					}
					catch (std::exception& e) {
						std::cout << "Error occured in YarpAngleReader: '" << current_string << "'" << std::endl;
						std::cout << "Cannot convert this to double: " << current_string;
						throw new ISpikeException("Error at YarpAngleReader lexical_cast");
					}
					LOG(LOG_INFO) << "Angle: " << angle;
					buffer.push_back(angle);
				}
			}
		}
	}
	catch(ISpikeException& ex){
		setError(ex.Msg());
	}
	catch(...){
		setError("An unknown exception occurred in the YarpAngleReader");
	}
	setRunning(false);
}


/**
* Initialises the reader and starts the main thread
*/
void YarpAngleReader::start(){
	if(!initialised){
		this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpAngleReader::workerFunction, this))));
		initialised = true;
	}
}

