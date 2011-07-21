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
using namespace std;

//Names of properties used
#define PORT_NAME_PROP "Port Name"
#define SLEEP_DURATION_PROP "Sleep Duration ms"

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
		addProperty(ComboProperty(yarpPortNames, "undefined", PORT_NAME_PROP, "The Yarp Port name", true));
	else
		addProperty(ComboProperty(yarpPortNames, yarpPortNames[0], PORT_NAME_PROP, "The Yarp Port name", true));

	//Other properties
	addProperty(IntegerProperty(20, SLEEP_DURATION_PROP, "Amount to sleep in milliseconds in between sending command.", false));

	//Create the description
	writerDescription = WriterDescription("Yarp Angle Writer", "This is a Yarp angle writer", "Angle Writer");

	//Initialize variables
	angleChanged = false;
}


/** Destructor */
YarpAngleWriter::~YarpAngleWriter(){
	if(yarpConnection != NULL)
		delete yarpConnection;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

// Inherited from Writer
void YarpAngleWriter::initialize(map<string,Property>& properties){
	updateProperties(properties);
	setInitialized(true);
}


// Inherited from AngleWriter
void YarpAngleWriter::setAngle(double newAngle){
	if(this->angle != newAngle)
		angleChanged = true;
	this->angle = newAngle;
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

/** Updates the properties with values from the map */
void YarpAngleWriter::updateProperties(map<string, Property>& properties){
	if(properties.count(PORT_NAME_PROP) == 0)
		throw ISpikeException("Property Port Name cannot be found.");
	if((updateReadOnly && !propertyMap[PORT_NAME_PROP].isReadOnly()) || !updateReadOnly)
		portName = updatePropertyValue(properties[PORT_NAME_PROP]);

	if(properties.count(SLEEP_DURATION_PROP) == 0)
		throw ISpikeException("Property Sleep Duration cannot be found.");
	if((updateReadOnly && !propertyMap[SLEEP_DURATION_PROP].isReadOnly()) || !updateReadOnly)
		sleepDuration_ms = updatePropertyValue(properties[SLEEP_DURATION_PROP]);
}

//Inherited from iSpikeThread
void YarpAngleWriter::workerFunction(){
	setRunning(true);
	clearError();

	try{
		//Connect to YARP
		map<string, YarpPortDetails>::iterator iter = this->portMap->find(this->getPortName());
		string ip;
		unsigned port;
		if (iter != this->portMap->end() ){
			ip = iter->second->getIp();
			port = iter->second->getPort();
			LOG(LOG_INFO) << "YarpAngleWriter: Yarp Port IP: " << ip << " Port: " << port;
		}
		else {
			throw ISpikeException("YarpAngleWriter: Yarp IP/Port map is empty!");
		}
		yarpConnection->connect_to_port(ip, port);
		yarpConnection->write_text("CONNECT foo\n");

		//Main run loop
		while(!isStopRequested()){
			if(angleChanged){
				//Send new angle
				this->yarpConnection->write_text("d\n");
				ostringstream commandStream;
				commandStream << "set pos " << this->degreeOfFreedom << " " << angle << "\n";
				string command = commandStream.str();
				LOG(LOG_DEBUG) << "YarpAngleWriter: Sent command " << command;
				this->yarpConnection->write_text(command);
				LOG(LOG_DEBUG) << "YarpAngleWriter: Received reply " << this->yarpConnection->read_until("\n");

				//Record that we have sent angle
				angleChanged = false;
			}

			//Sleep for the specified amount
			boost::this_thread::sleep(boost::posix_time::milliseconds(sleepDuration_ms));
		}
	}
	catch(ISpikeException& ex){
		setError(ex.msg());
	}
	catch(...){
		setError("An unknown exception occurred in the YarpVisualReader");
	}

	setRunning(false);
}
