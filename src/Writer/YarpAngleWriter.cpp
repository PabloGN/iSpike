/*
 * YarpAngleWriter.cpp
 *
 *  Created on: 23 Feb 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/Writer/YarpAngleWriter.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include <iSpike/YarpConnection.hpp>
#include <sstream>
#include <iostream>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>

/**
 * The default constructor, only initialises the default parameters and the description
 */
YarpAngleWriter::YarpAngleWriter(std::string nameserverIP, std::string nameserverPort){
	throw ISpikeException("TEST YARP ANGLE WRITER EXCEPTION");
	// First define the properties of this writer
	// Get the available yarp ports
	this->yarpConnection = new YarpConnection(nameserverIP, nameserverPort);
	this->portMap = this->yarpConnection->getPortMap();

	// Iterate over them and add as options
	std::map<std::string, YarpPortDetails*>::iterator iter;
	std::vector<std::string> yarpPortNames;
	for (iter = this->portMap->begin(); iter != this->portMap->end(); iter++){
		yarpPortNames.push_back(iter->first);
	}

	std::map<std::string,Property*> properties;
	properties["Port Name"] = new ComboProperty(
				"Port Name",
				"/icubSim/left_arm/state:o",
				"The Yarp Port name",
				yarpPortNames,
				true
				);
	properties["Degree Of Freedom"] = new IntegerProperty(
				"Degree Of Freedom",
				0,
				"The degree of freedom we would like to control",
				true
				);

	// Now let's create the description
	this->writerDescription = new WriterDescription("Yarp Angle Writer", "This is a Yarp angle writer", "Angle Writer", properties);
}


/** Destructor */
YarpAngleWriter::~YarpAngleWriter(){

}


void YarpAngleWriter::initialise(){
	YarpAngleWriter::initialise(this->getWriterDescription().getWriterProperties());
}


void YarpAngleWriter::initialise(std::map<std::string,Property*> properties){
	this->setPortName(((ComboProperty*)(properties["Port Name"]))->getValue());
	this->degreeOfFreedom = ((IntegerProperty*)(properties["Degree Of Freedom"]))->getValue();
	this->angleList = new std::queue<double>();
	this->initialised = false;
	this->previousAngle = 0;
}


/** Adds an angle to the processing queue */
void YarpAngleWriter::addAngle(double angle){
	boost::mutex::scoped_lock lock(this->mutex);
	if(abs(angle - this->previousAngle) < 0.5)
		return;
	if(this->angleList->size() > 0)
	{
		double lastAngle = this->angleList->back();
		if(abs(angle - lastAngle) < 0.5)
		{
			return;
		}
	}
	this->previousAngle = angle;
	this->angleList->push(angle);
}


/**
* Initialises the reader and starts the main thread
*/
void YarpAngleWriter::start(){
	if(!initialised)
	{
		this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpAngleWriter::workerFunction, this))));
		initialised = true;
	}
}


void YarpAngleWriter::workerFunction(){
	int sleepAmount = 1;
	std::map<std::string, YarpPortDetails*>::iterator iter = this->portMap->find(this->getPortName());
	std::string ip;
	std::string port;
	std::string type;
	if (iter != this->portMap->end() )
	{
		ip = iter->second->getIp();
		port = iter->second->getPort();
		type = iter->second->getType();
		LOG(LOG_INFO) << "YarpAngleWriter: Yarp Port IP: " << ip << " Port: " << port;
	} else {
		throw ISpikeException("YarpAngleWriter: Yarp IP/Port map is empty!");
	}
	this->yarpConnection->connect_to_port(ip, port);
	this->yarpConnection->write_text("CONNECT foo\n");
	while(true)
	{
		if(this->angleList->size() > 0)
		{
			double angle = this->angleList->front();
			{
				boost::mutex::scoped_lock lock(this->mutex);
				this->angleList->pop();
			}
			this->yarpConnection->write_text("d\n");
			std::ostringstream commandStream;
			commandStream << "set pos " << this->degreeOfFreedom << " " << angle << "\n";
			std::string command = commandStream.str();
			LOG(LOG_DEBUG) << "YarpAngleWriter: Sent command " << command;
			this->yarpConnection->write_text(command);
			LOG(LOG_DEBUG) << "YarpAngleWriter: Received reply " << this->yarpConnection->read_until("\n");
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(sleepAmount));
	}
}
