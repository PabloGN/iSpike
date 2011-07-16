/*
 * FileAngleReader.hpp
 *
 *  Created on: 9 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef FILEANGLEREADER_HPP_
#define FILEANGLEREADER_HPP_

#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Property.hpp>
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
	property_map properties;
    properties["Filename"] = new StringProperty(
          "Filename",
          "anglesIn.txt",
          "The file where the angles will be read from",
          true
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

  /*~FileAngleReader()
  {
    delete this->buffer;
  }*/

  /**
   * Retrieves the vector of joint angles
   */
  std::vector<double> getData();

  /**
   * Initialises the properties of the reader
   */
  void initialise()
  {
	  initialise(this->readerDescription->getReaderProperties());
  }

  /**
   * Initialises the properties of the reader
   */
  void initialise(property_map properties);

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

};

#endif /* FILEANGLEREADER_HPP_ */
