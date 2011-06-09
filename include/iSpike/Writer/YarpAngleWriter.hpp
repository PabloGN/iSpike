/*
 * YarpAngleWriter.hpp
 *
 *  Created on: 23 Feb 2011
 *      Author: Edgars Lazdins
 */

#ifndef YARPANGLEWRITER_HPP_
#define YARPANGLEWRITER_HPP_

#include <queue>
#include <string>
#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/Writer/WriterDescription.hpp>
#include <map>
#include <iSpike/YarpPortDetails.hpp>

class YarpAngleWriter : public AngleWriter {

private:
  std::queue<double>* angleList;
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
  YarpAngleWriter(std::string nameserverIP, std::string nameserverPort)
  {
    /**
     * First define the properties of this writer
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
			yarpPortNames,
			true
		  );
    /**
     * Now let's create the description
     */
    this->writerDescription = new WriterDescription(
          "Yarp Angle Writer",
          "This is a Yarp angle writer",
          "Angle Writer",
          properties
        );
  }

  void initialise();

  void initialise(std::map<std::string,Property*> properties);

  /**
   * Adds an angle to the processing queue
   */
  void addAngle(double angle);

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

  bool getInitialised()
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

#endif /* YARPANGLEWRITER_HPP_ */
