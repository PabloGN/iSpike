/*
 * FileAngleReader.hpp
 *
 *  Created on: 9 Mar 2011
 *      Author: cembo
 */

#ifndef FILEANGLEREADER_HPP_
#define FILEANGLEREADER_HPP_

#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Reader/ReaderDescription.hpp>
#include <iSpike/Property.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
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

  static ReaderDescription readerDescription;

  static std::map<std::string,Property*> initialiseProperties()
  {
    std::map<std::string,Property*> properties;
    properties["Degree Of Freedom"] = new IntegerProperty(
          "Degree Of Freedom",
          0,
          "The degree of freedom to read from this joint"
        );
    return properties;
  }

  /**
   * Retrieves the vector of joint angles
   */
  std::vector<double> getData();

  /**
   * Constructor
   * @param fileName The filename where the joints are read from
   */
  FileAngleReader(std::map<std::string,Property*> properties = FileAngleReader::readerDescription.getReaderProperties());

  /**
   * Initialises the reader and starts the main thread
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
