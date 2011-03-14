/*
 * FileAngleReader.hpp
 *
 *  Created on: 9 Mar 2011
 *      Author: cembo
 */

#ifndef FILEANGLEREADER_HPP_
#define FILEANGLEREADER_HPP_

#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Property.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>

/**
 * @class FileAngleReader
 * @brief FileAngleReader class
 *
 * This class represents a FileAngleReader, it retrieves a vector of joint angles from a given file
 * and makes it available upon request
 *
 * @author Edgars Lazdins
 *
 */
class FileAngleReader : public AngleReader {

private:
  std::vector<double>* buffer;
  boost::shared_ptr<boost::thread> threadPointer;
  void workerFunction();
  boost::mutex mutex;
  bool initialised;
  std::string fileName;

public:

  /*
   * The default constructor, only initialises the default parameters and the description
   */
  FileAngleReader()
  {
    /**
     * First define the properties of this reader
     */
    std::map<std::string,Property*> properties;
    properties["Filename"] = new StringProperty(
          "Filename",
          "anglesIn.txt",
          "The file where the angles will be read from"
        );
    /**
     * Now let's create the description
     */
    this->readerDescription = new ReaderDescription(
          "File Angle Reader",
          "This is a file angle reader",
          "Angle Reader",
          properties
        );
  }

  /**
   * Retrieves the vector of joint angles
   */
  std::vector<double> getData();

  /**
   * Initialises the properties of the reader
   */
  void initialise(std::map<std::string,Property*> properties);

  /**
   * Starts the reader
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

#endif /* FILEANGLEREADER_HPP_ */
