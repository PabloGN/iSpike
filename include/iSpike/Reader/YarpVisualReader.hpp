/*
 * VisualReader.hpp
 *
 *  Created on: 1 Jan 2011
 *      Author: cembo
 */

#ifndef YARPVISUALREADER_HPP_
#define YARPVISUALREADER_HPP_

#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Bitmap.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

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

public:

  /**
   * Retrieves the image
   */
  Bitmap getData();

  /**
   * Constructor
   * @param portName The YARP port where the image is read from
   */
  YarpVisualReader(std::string portName, std::string yarpIP, std::string yarpPort);

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
