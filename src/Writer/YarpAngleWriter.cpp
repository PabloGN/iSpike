//iSpike includes
#include <iSpike/Writer/YarpAngleWriter.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>
using namespace ispike;

//Other includes
#include <sstream>
#include <iostream>

/** The default constructor, only initialises the default parameters and the description */
YarpAngleWriter::YarpAngleWriter(string nameserverIP, unsigned nameserverPort){
	// Connect to YARP and get list of ports
	yarpConnection == NULL;
	yarpConnection = new YarpConnection(nameserverIP, nameserverPort);
	portMap = yarpConnection->getPortMap();

	//Store port names as properties of this reader
	vector<string> yarpPortNames;
	for (map<string, YarpPortDetails>::iterator iter = portMap.begin(); iter != portMap.end(); iter++){
		yarpPortNames.push_back(iter->first);
	}
	if(yarpPortNames.empty())
		propertyMap["Port Name"] = ComboProperty(yarpPortNames, "undefined", "Port Name","The Yarp Port name", true);
	else
		propertyMap["Port Name"] = ComboProperty(yarpPortNames, yarpPortNames[0], "Port Name","The Yarp Port name", true);

	//Other properties
	propertyMap["Degree Of Freedom"] = IntegerProperty(0, "Degree Of Freedom", "The degree of freedom we would like to control", true);

	//Create the description
	writerDescription = WriterDescription("Yarp Angle Writer", "This is a Yarp angle writer", "Angle Writer");

	//Initialize variables
	angleChanged = false;
	angle = 0.0;
}


/** Destructor */
YarpAngleWriter::~YarpAngleWriter(){
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

// Inherited from Writer
void YarpAngleWriter::initialise(map<string,Property*> properties){
	updateProperties(properties);
	setInitialized(true);
}


// Inherited from AngleWriter
void YarpAngleWriter::setAngle(double angle){
	boost::mutex::scoped_lock lock(this->mutex);
	updateAngle = false;
	if(this->angle != angle)
		updateAngle = true;
	this->angle = angle;
}


// Inherited from iSpikeThread
void YarpAngleWriter::start(){
	if(!isInitialized())
		throw iSpikeException("Cannot start YarpAngleWriter thread - it has not been initialized.");
	this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpAngleWriter::workerFunction, this))));
}


/*--------------------------------------------------------------------*/
/*---------                 PRIVATE METHODS                    -------*/
/*--------------------------------------------------------------------*/

void YarpAngleWriter::updateProperties(map<string, Property>& properties){
	//Store properties
	setPortName(((ComboProperty*)(properties["Port Name"]))->getValue());
	degreeOfFreedom = ((IntegerProperty*)(properties["Degree Of Freedom"]))->getValue();
	int sleepAmount = 1;
	propertyMap = properties;
}

FIXME
void YarpAngleWriter::workerFunction(){
	setRunning(true);

	map<string, YarpPortDetails>::iterator iter = this->portMap->find(this->getPortName());
	string ip;
	unsigned port;
	string type;
	if (iter != this->portMap->end() ){
		ip = iter->second->getIp();
		port = iter->second->getPort();
		type = iter->second->getType();
		LOG(LOG_INFO) << "YarpAngleWriter: Yarp Port IP: " << ip << " Port: " << port;
	}
	else {
		throw ISpikeException("YarpAngleWriter: Yarp IP/Port map is empty!");
	}
	this->yarpConnection->connect_to_port(ip, port);
	this->yarpConnection->write_text("CONNECT foo\n");
	while(!isStopRequested()){
		if(this->angleList->size() > 0) {
			double angle = this->angleList->front();
			{
				boost::mutex::scoped_lock lock(this->mutex);
				this->angleList->pop();
			}
			this->yarpConnection->write_text("d\n");
			ostringstream commandStream;
			commandStream << "set pos " << this->degreeOfFreedom << " " << angle << "\n";
			string command = commandStream.str();
			LOG(LOG_DEBUG) << "YarpAngleWriter: Sent command " << command;
			this->yarpConnection->write_text(command);
			LOG(LOG_DEBUG) << "YarpAngleWriter: Received reply " << this->yarpConnection->read_until("\n");
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(sleepAmount));
	}

	setRunning(false);
}
