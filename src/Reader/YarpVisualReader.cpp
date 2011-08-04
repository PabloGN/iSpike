//iSpike includes
#include <iSpike/Reader/YarpVisualReader.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>
using namespace ispike;

//Other includes
#include <boost/asio.hpp>
#include <iostream>
using namespace std;

//Property names
#define PORT_NAME_PROP "Port Name"

/** Constructor */
YarpVisualReader::YarpVisualReader(string nameserverIP, unsigned nameserverPort) {
	// Connect to YARP and get list of ports
	yarpConnection == NULL;//Initialize to null so that it is deleted correctly if an exception is thrown
	yarpConnection = new YarpConnection(nameserverIP, nameserverPort);

	//Store port names as properties of this reader
	vector<string> yarpPortNames;
	map<string, YarpPortDetails>& portMap = yarpConnection->getPortMap();
	for (map<string, YarpPortDetails>::iterator iter = portMap.begin(); iter != portMap.end(); ++iter){
		yarpPortNames.push_back(iter->first);
	}
	if(yarpPortNames.empty())
		addProperty(ComboProperty(yarpPortNames, "undefined", PORT_NAME_PROP, "The Yarp Port name", true));
	else
		addProperty(ComboProperty(yarpPortNames, yarpPortNames[0], PORT_NAME_PROP, "The Yarp Port name", true));

	//Create the description
	readerDescription = ReaderDescription("Yarp Visual Reader", "This is a Yarp visual reader", "Visual Reader");

	//Initialize variables
	portName = "Undefined";
}


/** Destructor */
YarpVisualReader::~YarpVisualReader() {
	if(isInitialized())	{
		requestStop();
		getThreadPointer()->join();
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
Bitmap& YarpVisualReader::getBitmap(){
	boost::mutex::scoped_lock lock(threadMutex);
	if(returnBitmap1)
		return *bitmap1;
	return *bitmap2;
}


//Inherited from Reader
void YarpVisualReader::initialize(map<string, Property>& properties){
	updateProperties(properties);

	bitmap1 = new Bitmap(0,0,0);
	bitmap2 = new Bitmap(0,0,0);
	returnBitmap1 = true;

	setInitialized(true);
}


//Inherited from PropertyHolder
void YarpVisualReader::setProperties(map<string, Property> &properties){
	updateProperties(properties);
}


//Inherited from iSpikeThread
void YarpVisualReader::start() {
	if(!isInitialized())
		throw ISpikeException("YarpVisualReader cannot be started because it has not been initialized.");
	if(isRunning())
		throw ISpikeException("Cannot start YarpVisualReader - it is already running.");

	this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpVisualReader::workerFunction, this))));
}

/*--------------------------------------------------------------------*/
/*---------                PRIVATE METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Swaps the returned bitmaps in a thread safe way */
void YarpVisualReader::swapBitmap(){
	boost::mutex::scoped_lock lock(threadMutex);
	if(returnBitmap1)
		returnBitmap1 = false;
	else
		returnBitmap1 = true;

	//Image has changed so increment the image ID
	++imageID;
}

/** Updates the properties */
void YarpVisualReader::updateProperties(map<string, Property>& properties){
	bool updateReadOnly = !isInitialized();
	if((updateReadOnly && !propertyMap[PORT_NAME_PROP].isReadOnly()) || !updateReadOnly)
		portName = updatePropertyValue(dynamic_cast<ComboProperty&>(properties[PORT_NAME_PROP]));
}


//Inherited from iSpikeThread
void YarpVisualReader::workerFunction(){
	setRunning(true);
	clearError();

	try{
		//Connect to port
		map<string, YarpPortDetails>& portMap = yarpConnection->getPortMap();
		map<string, YarpPortDetails>::iterator iter = portMap.find(getComboPropertyValue(PORT_NAME_PROP));
		if (iter == portMap.end() )
			throw ISpikeException("YarpVisualReader: Yarp IP/Port map is empty!");

		string ip = iter->second.getIp();
		unsigned port = iter->second.getPort();
		LOG(LOG_INFO) << "YarpVisualReader: Yarp Port IP: " << ip << " Port: " << port;

		yarpConnection->connect_to_port(ip, port);
		yarpConnection->prepare_to_read_binary();

		//Main run loop
		while(!isStopRequested()) {
			//Load new version of image into new buffer
			if(returnBitmap1){
				yarpConnection->read_image(*bitmap2);
				swapBitmap();
			}
			else{
				yarpConnection->read_image(*bitmap1);
				swapBitmap();
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

