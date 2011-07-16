/*
 * ConsoleAngleReader.hpp
 *
 *  Created on: 9 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef CONSOLEANGLEREADER_HPP_
#define CONSOLEANGLEREADER_HPP_

#include <boost/scoped_ptr.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Property.hpp>
#include <string>
#include <vector>

/**
 * @class ConsoleAngleReader
 * @brief ConsoleAngleReader class
 *
 * This class represents a ConsoleAngleReader, it retrieves a vector of joint angles from the console
 * and makes it available upon request
 *
 * @author Edgars Lazdins
 *
 */
class ConsoleAngleReader : public AngleReader {

private:
  bool initialised;

public:

  /*
   * The default constructor, only initialises the default parameters and the description
   */
  ConsoleAngleReader()
  {
    property_map properties;
    /**
     * Now let's create the description
     */
    this->readerDescription.reset(new ReaderDescription(
          "Console Angle Reader",
          "This is a console angle reader",
          "Angle Reader",
          properties
        ));
  }

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
