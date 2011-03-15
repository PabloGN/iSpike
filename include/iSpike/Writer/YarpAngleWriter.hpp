/*
 * YarpAngleWriter.hpp
 *
 *  Created on: 23 Feb 2011
 *      Author: cembo
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

class YarpAngleWriter : public AngleWriter {

private:
  std::queue<double>* angleList;
  boost::shared_ptr<boost::thread> threadPointer;
  void workerFunction();
  boost::mutex mutex;
  bool initialised;
  std::string portName;
  YarpConnection* yarpConnection;

public:

  /*
   * The default constructor, only initialises the default parameters and the description
   */
  YarpAngleWriter()
  {
    /**
     * First define the properties of this writer
     */
    std::map<std::string,Property*> properties;
    properties["Yarp IP"] = new StringProperty(
          "Yarp IP",
          "127.0.0.1",
          "The Yarp nameserver IP Address"
        );
    properties["Yarp Port"] = new StringProperty(
          "Yarp Port",
          "10000",
          "The Yarp nameserver Port"
        );
    properties["Port Name"] = new StringProperty(
          "Port Name",
          "/icubSim/left_arm/rpc:i",
          "The Yarp Port name"
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
