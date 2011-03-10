/*
 * FileAngleWriter.hpp
 *
 *  Created on: 9 Mar 2011
 *      Author: cembo
 */

#ifndef FILEANGLEWRITER_HPP_
#define FILEANGLEWRITER_HPP_

#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <iSpike/Writer/AngleWriter.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <iSpike/YarpConnection.hpp>
#include <boost/lexical_cast.hpp>

class FileAngleWriter : public AngleWriter {

private:
  std::queue<double>* angleList;
  boost::shared_ptr<boost::thread> threadPointer;
  void workerFunction();
  boost::mutex mutex;
  bool initialised;
  std::string fileName;

public:

  FileAngleWriter(std::string fileName);

  /**
   * Adds an angle to the processing queue
   */
  void addAngle(double angle);

  /**
   * Initialises the writer and starts the main thread
   */
  void start();

  std::string getFileName()
  {
    return this->fileName;
  }

  void setFileName(std::string fileName)
  {
    this->fileName = fileName;
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


#endif /* FILEANGLEWRITER_HPP_ */
