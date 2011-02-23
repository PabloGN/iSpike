/*
 * YarpAngleReader.hpp
 *
 *  Created on: 22 Feb 2011
 *      Author: cembo
 */

#ifndef YARPANGLEREADER_HPP_
#define YARPANGLEREADER_HPP_

#include <iSpike/Reader/Reader.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Bitmap.hpp>
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
class YarpAngleReader : public Reader {

private:
  std::vector<double>* buffer;
  boost::shared_ptr<boost::thread> threadPointer;
  void workerFunction();
  boost::mutex mutex;
  bool initialised;
  std::string portName;
  YarpConnection* yarpConnection;

public:

  /**
   * Retrieves the vector of joint angles
   */
  std::vector<double> getData();

  /**
   * Constructor
   * @param portName The YARP port where the joints are read from
   */
  YarpAngleReader(std::string portName);

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
