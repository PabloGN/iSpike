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


/** Constructor */
YarpVisualReader(string nameserverIP, string nameserverPort) {
	/**
   * First define the properties of this reader
   */
	/**
   * Get the available yarp ports
   */
	LOG(LOG_DEBUG) << "before yarp connection";
	this->yarpConnection = new YarpConnection(nameserverIP, nameserverPort);
	LOG(LOG_DEBUG) << "getting port map";
	this->portMap = this->yarpConnection->getPortMap();

	/**
   * Iterate over them and add as options
   */
	LOG(LOG_DEBUG) << "iterating";
	map<string, YarpPortDetails*>::iterator iter;
	vector<string> yarpPortNames;
	for (iter = this->portMap->begin(); iter != this->portMap->end(); iter++)
	{
		yarpPortNames.push_back(iter->first);
	}

	property_map properties;
	properties["Port Name"] =
			boost::shared_ptr<Property>(new ComboProperty(
											"Port Name",
											"/icubSim/left_arm/state:o",
											"The Yarp Port name",
											yarpPortNames,
											true
											));
	/**
   * Now let's create the description
   */
	this->readerDescription.reset(new ReaderDescription(
									  "Yarp Visual Reader",
									  "This is a Yarp visual reader",
									  "Visual Reader",
									  properties
									  ));
	LOG(LOG_DEBUG) << "exiting";
}

/** Destructor */
~YarpVisualReader() {
	if(isInitialized())
	{
		this->threadPointer->interrupt();
		this->threadPointer->join();
		delete this->threadPointer.get();
		delete this->buffer;
	}
}


Bitmap YarpVisualReader::getData()
{
	boost::mutex::scoped_lock lock(this->mutex);
	return *(this->buffer);
}

void YarpVisualReader::start()
{
	if(!initialised)
	{
		this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpVisualReader::workerFunction, this))));
		initialised = true;
	}
}

void YarpVisualReader::workerFunction()
{
	map<string, YarpPortDetails*>::iterator iter = this->portMap->find(this->getPortName());
	string ip;
	string port;
	string type;
	if (iter != this->portMap->end() )
	{
		ip = iter->second->getIp();
		port = iter->second->getPort();
		type = iter->second->getType();
		LOG(LOG_INFO) << "YarpVisualReader: Yarp Port IP: " << ip << " Port: " << port;
	} else {
		throw ISpikeException("YarpVisualReader: Yarp IP/Port map is empty!");
	}
	this->yarpConnection->connect_to_port(ip, port);
	this->yarpConnection->prepare_to_read_binary();
	while(true)
	{
		Bitmap* image = this->yarpConnection->read_image();
		if(image != NULL)
		{
			boost::mutex::scoped_lock lock(this->mutex);
			if(this->buffer != NULL)
				free(this->buffer->getContents());
			this->buffer = image;
		}
	}
}

void YarpVisualReader::initialise(property_map properties)
{
	LOG(LOG_DEBUG) << "start of initialisation";
	this->setPortName(static_cast<ComboProperty*>(properties["Port Name"].get())->getValue());
	this->buffer = new Bitmap(0,0,0,NULL);
	this->initialised = false;
	LOG(LOG_DEBUG) << "exiting initialisation";
}
