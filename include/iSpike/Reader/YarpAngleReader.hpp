/*
 * YarpAngleReader.hpp
 *
 *  Created on: 22 Feb 2011
 *      Author: Edgars Lazdins
 */

#ifndef YARPANGLEREADER_HPP_
#define YARPANGLEREADER_HPP_

#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Property.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <vector>

/**
 * @class YarpVisualReader
 * @brief YarpVisualReader class
 *
 * This class represents a YarpAngleReader, it retrieves a vector of joint angles from a given yarp port
 * and makes it available upon request
 *
 * @author Edgars Lazdins
 *
 */
class YarpAngleReader : public AngleReader {

private:
  std::vector<double>* buffer;
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
  YarpAngleReader()
  {
    /**
     * First define the properties of this reader
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
          "/icubSim/left_arm/state:o",
          "The Yarp Port name"
        );
    /**
     * Now let's create the description
     */
    this->readerDescription = new ReaderDescription(
          "Yarp Angle Reader",
          "This is a Yarp angle reader",
          "Angle Reader",
          properties
        );
  }

  /**
   * Retrieves the vector of joint angles
   */
  std::vector<double> getData();

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

#endif /* YARPANGLEREADER_HPP_ */
