#include <iSpike/Writer/YarpAngleWriter.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>
using namespace ispike;

#include <sstream>
#include <iostream>
using namespace std;

//Names of properties used
#define PORT_NAME_PROP "Port Name"
#define SLEEP_DURATION_PROP "Sleep Duration ms"


/** The default constructor, only initialises the default parameters and the description */
YarpAngleWriter::YarpAngleWriter(string nameserverIP, unsigned nameserverPort){
	// Connect to YARP and get list of ports
	yarpConnection = NULL;
	yarpConnection = new YarpConnection(nameserverIP, nameserverPort);
	map<string, YarpPortDetails>& portMap = yarpConnection->getPortMap();

	//Store port names as properties of this reader
	vector<string> yarpPortNames;
	for (map<string, YarpPortDetails>::iterator iter = portMap.begin(); iter != portMap.end(); iter++){
		yarpPortNames.push_back(iter->first);
	}
	if(yarpPortNames.empty())
		addProperty(Property("undefined", yarpPortNames, PORT_NAME_PROP, "The Yarp Port name", true));
	else
		addProperty(Property(yarpPortNames[0], yarpPortNames, PORT_NAME_PROP, "The Yarp Port name", true));

	//Other properties
	addProperty(Property(Property::Integer, 50, SLEEP_DURATION_PROP, "Amount to sleep in milliseconds in between sending command.", false));

	//Create the description
	writerDescription = Description("Yarp Angle Writer", "This is a Yarp angle writer", "Angle Writer");

	//Initialize variables
	angleChanged = false;
	degreeOfFreedom = -1;
}


/** Destructor */
YarpAngleWriter::~YarpAngleWriter(){
	if(isRunning()){
		requestStop();
		getThreadPointer()->join();
	}
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
	if(this->angle != newAngle){
		this->angle = newAngle;
		angleChanged = true;
	}
}


// Inherited from AngleWriter
void YarpAngleWriter::setDegreeOfFreedom(int dof){
	if(this->degreeOfFreedom != dof){
		this->degreeOfFreedom = dof;
		angleChanged = true;
	}
}


//Inherited from PropertyHolder
void YarpAngleWriter::setProperties(map<string, Property> &properties){
	updateProperties(properties);
}


// Inherited from iSpikeThread
void YarpAngleWriter::start(){
	if(!isInitialized())
		throw ISpikeException("Cannot start YarpAngleWriter thread - it has not been initialized.");
	if(isRunning())
		throw ISpikeException("Cannot start YarpAngleWriter thread - it is already running.");

	this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpAngleWriter::workerFunction, this))));
}


/*--------------------------------------------------------------------*/
/*---------                 PRIVATE METHODS                    -------*/
/*--------------------------------------------------------------------*/

/** Updates the properties with values from the map */
void YarpAngleWriter::updateProperties(map<string, Property>& properties){
	if(!isInitialized())
		portName = updateComboProperty(properties[PORT_NAME_PROP]);

	sleepDuration_ms = updateIntegerProperty(properties[SLEEP_DURATION_PROP]);
}


//Inherited from iSpikeThread
void YarpAngleWriter::workerFunction(){
	setRunning(true);
	clearError();

	try{
		//Connect to YARP
		map<string, YarpPortDetails>& portMap = yarpConnection->getPortMap();
		map<string, YarpPortDetails>::iterator iter = portMap.find(portName);
		if(iter == portMap.end())
			throw ISpikeException("YarpAngleWriter: Cannot find port name in port map");
		string ip = iter->second.getIp();
		unsigned port = iter->second.getPort();

		yarpConnection->connect_to_port(ip, port);
		yarpConnection->write_text("CONNECT foo\n");

		//Main run loop
		unsigned loopCtr = 0;
		while(!isStopRequested()){
			if(loopCtr % 20 == 0)
				LOG(LOG_DEBUG)<<"YarpAngleWriter: Checking for new angle.";
			if(angleChanged){
				//Send new angle
				yarpConnection->write_text("d\n");
				ostringstream commandStream;
				commandStream << "set pos " << degreeOfFreedom << " " << angle << "\n";
				string command = commandStream.str();
				LOG(LOG_DEBUG) << "YarpAngleWriter: Sending command " << command;
				this->yarpConnection->write_text(command);
				LOG(LOG_DEBUG) << "YarpAngleWriter: Received reply " << this->yarpConnection->read_until("\n");

				//Record that we have sent angle
				angleChanged = false;
			}

			//Sleep for the specified amount
			if(sleepDuration_ms > 0)
				boost::this_thread::sleep(boost::posix_time::milliseconds(sleepDuration_ms));
			++loopCtr;
		}
	}
	catch(ISpikeException& ex){
		setError(ex.msg());
	}
	catch(exception& ex){
		LOG(LOG_CRITICAL)<<"YarpAngleWriter exception: "<<ex.what();
		setError("An exception occurred in the YarpAngleWriter.");
	}
	catch(...){
		setError("An unknown exception occurred in the YarpAngleWriter");
	}

	setRunning(false);
}
