//iSpike includes
#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>
using namespace ispike;

//Other includes
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

/** Constructor */
YarpAngleReader::YarpAngleReader(string nameserverIP, unsigned nameserverPort){
	// Connect to YARP and get list of ports
	yarpConnection == NULL;
	yarpConnection = new YarpConnection(nameserverIP, nameserverPort);
	portMap = yarpConnection->getPortMap();

	//Store port names as properties of this reader
	vector<string> yarpPortNames;
	for (map<string, YarpPortDetails*>::iterator iter = this->portMap->begin(); iter != this->portMap->end(); iter++){
		yarpPortNames.push_back(iter->first);
	}
	if(yarpPortNames.empty())
		propertyMap["Port Name"] = ComboProperty(yarpPortNames, "undefined", "Port Name","The Yarp Port name", true);
	else
		propertyMap["Port Name"] = ComboProperty(yarpPortNames, yarpPortNames[0], "Port Name","The Yarp Port name", true);

	//Create the description
	readerDescription = ReaderDescription("Yarp Angle Reader", "This is a Yarp angle reader", "Angle Reader");
}


/*! Destructor */
YarpAngleReader::~YarpAngleReader(){
	if(isRunning()){
		requestStop();
		threadPointer->join();
	}
	if(yarpConnection != NULL)
		delete yarpConnection;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

// Inherited from AngleReader
vector<double> YarpAngleReader::getData(){
	boost::mutex::scoped_lock lock(this->mutex);
	return buffer;
}


// Inherited from Reader
void YarpAngleReader::initialize(map<string, Property>& properties){
	updateProperties(properties);
	buffer.clear();
	setInitialized(true);
}


// Inherited from AngleReader
void YarpAngleReader::start(){
	if(!isInitialized())
		throw iSpikeException("YarpAngleReader thread cannot be started - it has not been initialized.");
	setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpAngleReader::workerFunction, this))));
}


/*--------------------------------------------------------------------*/
/*---------                 PRIVATE METHODS                    -------*/
/*--------------------------------------------------------------------*/

void YarpAngleReader::updateProperties(map<string, Property>& properties){
	this->setPortName(((ComboProperty)properties["Port Name"]).getValue());
	propertyMap = properties;
}

/** Function passed to thread - similar to run() method */
void YarpAngleReader::workerFunction(){
	setRunning(true);
	clearError();
	try{
		//Connect to port
		map<string, YarpPortDetails>::iterator iter = portMap.find(this->getPortName());
		string ip;
		unsigned port;
		string type;
		if (iter != this->portMap->end() ) {
			ip = iter->second->getIp();
			port = iter->second->getPort();
			type = iter->second->getType();
			LOG(LOG_INFO) << "YarpAngleReader: Yarp Port IP: " << ip << " Port: " << port;
		}
		else {
			throw ISpikeException("YarpAngleReader: Yarp IP/Port map is empty!");
		}
		yarpConnection->connect_to_port(ip, port);
		yarpConnection->prepare_to_read_text();
		while(!isStopRequested()){
			//string contentLine = this->yarpConnection->read_until("\n");
			//Get angle data from YARP server
			string contentLine = this->yarpConnection->getSocketString();

			//Lock mutex while buffer is being filled
			boost::mutex::scoped_lock lock(this->mutex);

			//Fill buffer with new data
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
					double angle = 0;
					try{
						angle = boost::lexical_cast<double>(current_string);
					}
					catch (exception& e) {
						cout << "Error occured in YarpAngleReader: '" << current_string << "'" << endl;
						cout << "Cannot convert this to double: " << current_string;
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

