//iSpike includes
#include <iSpike/Reader/YarpVisualReader.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>
using namespace ispike;

//Other includes
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
using namespace std;

//Property names
#define PORT_NAME_PROP "Port Name"

/** Constructor */
YarpVisualReader(string nameserverIP, unsigned nameserverPort) {
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
		addProperty(ComboProperty(yarpPortNames, "undefined", PORT_NAME_PROP, "The Yarp Port name", true));
	else
		addProperty(ComboProperty(yarpPortNames, yarpPortNames[0], PORT_NAME_PROP, "The Yarp Port name", true));

	//Create the description
	readerDescription = ReaderDescription("Yarp Visual Reader", "This is a Yarp visual reader", "Visual Reader");
}


/** Destructor */
~YarpVisualReader() {
	if(isInitialized())	{
		threadPointer->interrupt();
		threadPointer->join();
		delete threadPointer.get();
		delete bitmap1;
		delete bitmap2;
	}
	if(yarpConnection != NULL)
		delete yarpConnection;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

//Inherited from VisualReader
Bitmap& YarpVisualReader::getData(){
	if(returnBitmap1)
		return bitmap1;
	return bitmap2;
}


//Inherited from iSpikeThread
void YarpVisualReader::start() {
	if(!isInitialized())
		throw ISpikeException("YarpVisualReader cannot be started because it has not been initialized.");
	this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpVisualReader::workerFunction, this))));
}


//Inherited from iSpikeThread
void YarpVisualReader::workerFunction(){
	setRunning(true);
	clearError();

	try{
		//Connect to port
		map<string, YarpPortDetails>::iterator iter = portMap->find(getComboPropertyValue(PORT_NAME_PROP));
		string ip;
		unsigned port;
		if (iter != portMap->end() ) 	{
			ip = iter->second->getIp();
			port = iter->second->getPort();
			LOG(LOG_INFO) << "YarpVisualReader: Yarp Port IP: " << ip << " Port: " << port;
		}
		else {
			throw ISpikeException("YarpVisualReader: Yarp IP/Port map is empty!");
		}
		yarpConnection->connect_to_port(ip, port);
		yarpConnection->prepare_to_read_binary();

		//Main run loop
		while(!isStopRequested()) {
			//Load new version of image into new buffer
			if(returnBitmap1){
				yarpConnection->read_image(bitmap2);
				returnBitmap1 = false;
			}
			else{
				yarpConnection->read_image(bitmap1);
				returnBitmap1 = true;
			}
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


//Inherited from Reader
void YarpVisualReader::initialize(map<string, Property>& properties){
	if(properties.count(PORT_NAME_PROP) == 0)
		throw ISpikeException("Property Port Name cannot be found.");
	if((updateReadOnly && !propertyMap[PORT_NAME_PROP].isReadOnly()) || !updateReadOnly)
		setPortName(updatePropertyValue(properties[PORT_NAME_PROP]));

	bitmap1 = new Bitmap(0,0,0);
	bitmap2 = new Bitmap(0,0,0);
	returnBitmap1 = true;

	setInitialized(true);
}
