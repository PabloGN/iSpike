/*
 * FileAngleWriter.hpp
 *
 *  Created on: 9 Mar 2011
 *      Author: Edgars Lazdins
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
#include <iSpike/Property.hpp>
#include <iSpike/Writer/WriterDescription.hpp>

class FileAngleWriter : public AngleWriter {

private:
  std::queue<double>* angleList;
  boost::shared_ptr<boost::thread> threadPointer;
  void workerFunction();
  boost::mutex mutex;
  bool initialised;
  std::string fileName;

public:

  FileAngleWriter()
  {
    /**
     * First define the properties of this writer
     */
    std::map<std::string,Property*> properties;
    properties["Filename"] = new StringProperty(
          "Filename",
          "anglesOut.txt",
          "The file where the angles will be written to"
        );
    /**
     * Now let's create the description
     */
    this->writerDescription = new WriterDescription(
          "File Angle Writer",
          "This is a file angle writer",
          "Angle Writer",
          properties
        );
  }

  void initialise(std::map<std::string,Property*> properties);

  void initialise()
  {
    initialise(writerDescription->getWriterProperties());
  }

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
