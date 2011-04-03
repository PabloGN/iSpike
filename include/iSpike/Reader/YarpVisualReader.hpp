/*
 * VisualReader.hpp
 *
 *  Created on: 1 Jan 2011
 *      Author: Edgars Lazdins
 */

#ifndef YARPVISUALREADER_HPP_
#define YARPVISUALREADER_HPP_

#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Bitmap.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <map>
#include <iSpike/YarpPortDetails.hpp>

/**
 * @class YarpVisualReader
 * @brief YarpVisualReader class
 *
 * This class represents a YarpVisualReader, it retrieves an image from a given yarp port
 * and makes it available upon request
 *
 * @author Edgars Lazdins
 *
 */
class YarpVisualReader : public VisualReader {

private:
  Bitmap* buffer;
  boost::shared_ptr<boost::thread> threadPointer;
  void workerFunction();
  boost::mutex mutex;
  bool initialised;
  std::string portName;
  YarpConnection* yarpConnection;
  std::map<std::string, YarpPortDetails*>* portMap;

public:

  /*
   * The default constructor, only initialises the default parameters and the description
   */
  YarpVisualReader(std::string nameserverIP, std::string nameserverPort)
  {
    /**
     * First define the properties of this reader
     */
	 /**
	 * Get the available yarp ports
	 */
	  this->yarpConnection = new YarpConnection(nameserverIP, nameserverPort);
	  this->portMap = this->yarpConnection->getPortMap();

    /**
     * Iterate over them and add as options
     */

	 std::map<std::string, YarpPortDetails*>::iterator iter;
	 std::vector<std::string> yarpPortNames;
	for (iter = this->portMap->begin(); iter != this->portMap->end(); iter++)
	{
		yarpPortNames.push_back(iter->first);
	}

	 std::map<std::string,Property*> properties;
	 properties["Port Name"] = new ComboProperty(
		   "Port Name",
		   "/icubSim/left_arm/state:o",
		   "The Yarp Port name",
		   yarpPortNames
	 );
    /**
     * Now let's create the description
     */
    this->readerDescription = new ReaderDescription(
          "Yarp Visual Reader",
          "This is a Yarp visual reader",
          "Visual Reader",
          properties
        );
  }

  ~YarpVisualReader()
  {
    if(this->initialised)
    {
      this->threadPointer->interrupt();
      this->threadPointer->join();
      delete this->threadPointer.get();
      delete this->buffer;
    }
  }

  /**
   * Retrieves the image
   */
  Bitmap getData();

  void initialise(std::map<std::string,Property*> properties);

  /**
   * Initialises the reader and starts the main thread
   */
  void start();

  std::string getPortName()
  {
    return this->portName;
  }

  void setPortName(std::string portName)
  {
    this->portName = portName;
  }

  bool getInitialised() const
  {
      return initialised;
  }

  void setInitialised(bool initialised)
  {
      this->initialised = initialised;
  }

  boost::shared_ptr<boost::thread> getThreadPointer() const
  {
      return threadPointer;
  }

  void setThreadPointer(boost::shared_ptr<boost::thread> threadPointer)
  {
      this->threadPointer = threadPointer;
  }

};

#endif /* YARPVISUALREADER_HPP_ */
