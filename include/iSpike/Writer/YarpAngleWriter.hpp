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
#include <iSpike/Writer/Writer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <iSpike/YarpConnection.hpp>

class YarpAngleWriter : public Writer {

private:
  std::queue<double>* angleList;
  boost::shared_ptr<boost::thread> threadPointer;
  void workerFunction();
  boost::mutex mutex;
  bool initialised;
  std::string portName;
  YarpConnection* yarpConnection;

public:

  /**
   * Constructor
   * @param portName The YARP port where the joints commands are written to
   */
  YarpAngleWriter(std::string portName, std::string yarpIP, std::string yarpPort);

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

#endif /* YARPANGLEWRITER_HPP_ */
