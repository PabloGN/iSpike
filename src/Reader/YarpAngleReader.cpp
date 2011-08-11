//iSpike includes
#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>
using namespace ispike;

//Other includes
#include <ostream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
using namespace std;

//Property names
#define PORT_NAME_PROP "Port Name"
#define SLEEP_DURATION_PROP "Sleep Duration ms"

//Debug output
//#define DEBUG

/** Constructor */
YarpAngleReader::YarpAngleReader(string nameserverIP, unsigned nameserverPort){
	// Connect to YARP and get list of ports
	yarpConnection = NULL;//Set to null so that it is deleted correctly if an exception is thrown
	yarpConnection = new YarpConnection(nameserverIP, nameserverPort);
	map<string, YarpPortDetails> portMap = yarpConnection->getPortMap();

	//Store port names as properties of this reader
	vector<string> yarpPortNames;
	for (map<string, YarpPortDetails>::iterator iter = portMap.begin(); iter != portMap.end(); ++iter){
		yarpPortNames.push_back(iter->first);
	}
	if(yarpPortNames.empty())
		addProperty(Property("undefined", yarpPortNames, PORT_NAME_PROP, "The Yarp Port name", true));
	else
		addProperty(Property(yarpPortNames[0], yarpPortNames, PORT_NAME_PROP, "The Yarp Port name", true));
	addProperty(Property(Property::Integer, 50, SLEEP_DURATION_PROP, "Amount to sleep in milliseconds in between refreshing angle.", false));

	//Create the description
	readerDescription = Description("Yarp Angle Reader", "This is a Yarp angle reader", "Angle Reader");

	//Initialize variables
	portName = "Undefined";
	degreeOfFreedom = 0;
}


/*! Destructor */
YarpAngleReader::~YarpAngleReader(){
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

// Inherited from Reader
void YarpAngleReader::initialize(map<string, Property>& properties){
	updateProperties(properties);
	setAngle(0.0);
	setInitialized(true);
}


//Inherited from PropertyHolder
void YarpAngleReader::setProperties(map<string, Property> &properties){
	updateProperties(properties);
}


// Inherited from AngleReader
void YarpAngleReader::start(){
	if(!isInitialized())
		throw ISpikeException("YarpAngleReader thread cannot be started - it has not been initialized.");
	if(isRunning())
		throw ISpikeException("Cannot start YarpAngleReader thread - it is already running.");

	setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpAngleReader::workerFunction, this))));
}


/*--------------------------------------------------------------------*/
/*---------                 PRIVATE METHODS                    -------*/
/*--------------------------------------------------------------------*/

/** Updates the properties */
void YarpAngleReader::updateProperties(map<string, Property>& properties){
	if(!isInitialized())
		portName = updateComboProperty(properties[PORT_NAME_PROP]);

	sleepDuration_ms = updateIntegerProperty(properties[SLEEP_DURATION_PROP]);

}


/** Function passed to thread - similar to run() method */
void YarpAngleReader::workerFunction(){
	setRunning(true);
	clearError();

	//Variables to be used in the loop
	vector<double> buffer;
	double tmpAngle;

	try{
		//Connect to port
		map<string, YarpPortDetails>::iterator iter = yarpConnection->getPortMap().find(portName);
		if (iter == yarpConnection->getPortMap().end() )
			throw ISpikeException("YarpAngleReader: Cannot find port name!");

		string ip = iter->second.getIp();
		unsigned port = iter->second.getPort();
		LOG(LOG_INFO) << "YarpAngleReader: Yarp Port IP: " << ip << " Port: " << port;


		//Main run loop
		unsigned loopCtr = 0;
		while(!isStopRequested()){
			//Get angle data from YARP server
			yarpConnection->connect_to_port(ip, port);
			yarpConnection->prepare_to_read_text();
			string contentLine = this->yarpConnection->getSocketString();

			//Extract the angles
			buffer.clear();
			if(contentLine.length() > 0){
				boost::regex splitString("\\s+");
				list<string> lines;
				boost::regex_split(back_inserter(lines), contentLine, splitString);
				lines.pop_front();
				lines.pop_front();
				while(lines.size() > 0)	{
					string current_string = *(lines.begin());
					lines.pop_front();
					try{
						tmpAngle = boost::lexical_cast<double>(current_string);
					}
					catch (exception& e) {
						LOG(LOG_CRITICAL)<<"Error occured in YarpAngleReader: '"<<current_string<<"'"<<"Cannot convert this to double: "<<current_string;
						throw new ISpikeException("Error at YarpAngleReader lexical_cast");
					}
					buffer.push_back(tmpAngle);
				}
			}

			//Set the angle to the appropriate degree of freedom
			if(degreeOfFreedom >= buffer.size())
				throw ISpikeException("Degree of freedom is greater than the buffer size.");

			//Logging output
			if(loopCtr % 20 ==0){
				ostringstream tmpOss;
				for(int i=0; i<buffer.size();  ++i){
					tmpOss<<buffer.at(i)<<" ";
				}
				LOG(LOG_INFO)<<"Angles: "<<tmpOss.str()<<"; degreeOfFreedom="<<degreeOfFreedom<<"; angle="<<getAngle();
			}

			//Set the angle
			setAngle(buffer[degreeOfFreedom]);

			//Sleep for a period before refreshing the angle
			if(sleepDuration_ms > 0)
				boost::this_thread::sleep(boost::posix_time::milliseconds(sleepDuration_ms));

			++loopCtr;
		}
	}
	catch(ISpikeException& ex){
		setError(ex.msg());
	}
	catch(...){
		setError("An unknown exception occurred in the YarpAngleReader");
	}

	setRunning(false);
}

