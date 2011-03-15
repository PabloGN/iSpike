/*
 * WriterDescription.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: cembo
 */

#ifndef WRITERDESCRIPTION_HPP_
#define WRITERDESCRIPTION_HPP_

#include <string>
#include <map>
#include <iSpike/Property.hpp>

/**
 * @class WriterDescription
 * @brief Writer Description class
 *
 * This class describes a Writer.
 *
 * @author Edgars Lazdins
 *
 */
class WriterDescription {

private:
  /// Name of the Writer
  std::string writerName;

  ///Writer's description
  std::string writerDescription;

  ///Type of a Writer this is
  std::string writerType;

  ///A map of the Writer's properties
  std::map<std::string,Property*> writerProperties;

public:

  /**
   *  Default constructor, creates a new Writer Description with the given parameters
   *  @param writerName Name of the Writer
   *  @param writerDescription Description for the Writer
   *  @param writerType The type of a Writer this is
   *  @param writerProperties A map of the Writer's properties
   */
  WriterDescription(std::string writerName, std::string writerDescription, std::string writerType, std::map<std::string,Property*> writerProperties)
  {
    this->writerName = writerName;
    this->writerDescription = writerDescription;
    this->writerType = writerType;
    this->writerProperties = writerProperties;
  }

  /**
   * Returns the Writer's description
   * @return Description of the Reader
   */
  std::string getWriterDescription() const
  {
      return writerDescription;
  }

  /**
   * Returns the name of the Writer
   * @return Name of the Writer
   */
  std::string getWriterName() const
  {
      return writerName;
  }

  /**
   * Returns the Writer's type
   * @return Writer's type
   */
  std::string getWriterType() const
  {
      return writerType;
  }

  /**
   * Returns a map of the Writer's properties
   * @return Map of the Writer's properties
   */
  std::map<std::string,Property*> getWriterProperties() const
  {
    return writerProperties;
  }

};

#endif /* WRITERDESCRIPTION_HPP_ */
