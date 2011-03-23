/*
 * ReaderDescription.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef READERDESCRIPTION_HPP_
#define READERDESCRIPTION_HPP_

#include <string>
#include <map>
#include <iSpike/Property.hpp>

/**
 * @class ReaderDescription
 * @brief Reader Description class
 *
 * This class describes a Reader.
 *
 * @author Edgars Lazdins
 *
 */
class ReaderDescription {

private:
  ///Name of the Reader
  std::string readerName;

  ///Description of the Reader
  std::string readerDescription;

  /// Type of the Reader
  std::string readerType;

  ///Reader's properties
  std::map<std::string,Property*> readerProperties;

public:

  /**
   *  Default constructor, creates a new Reader Description with the given parameters
   *  @param readerName Name of the Reader
   *  @param readerDescription Description for the Reader
   *  @param readerType The type of a Reader this is
   *  @param readerProperties A map of the Reader's properties
   */
  ReaderDescription(std::string readerName, std::string readerDescription, std::string readerType, std::map<std::string,Property*> readerProperties)
  {
    this->readerName = readerName;
    this->readerDescription = readerDescription;
    this->readerType = readerType;
    this->readerProperties = readerProperties;
  }

  /**
   * Returns the Reader's description
   * @return Description of the Reader
   */
  std::string getReaderDescription() const
  {
      return readerDescription;
  }

  /**
   * Returns the name of the Reader
   * @return Name of the Reader
   */
  std::string getReaderName() const
  {
      return readerName;
  }

  /**
   * Returns the Reader's type
   * @return Reader's type
   */
  std::string getReaderType() const
  {
      return readerType;
  }

  /**
   * Returns a map of the Reader's properties
   * @return Map of the Reader's properties
   */
  std::map<std::string,Property*> getReaderProperties() const
  {
    return readerProperties;
  }

};


#endif /* READERDESCRIPTION_HPP_ */
