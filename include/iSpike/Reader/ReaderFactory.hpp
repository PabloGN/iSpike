/*
 * ReaderFactory.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: cembo
 */

#ifndef READERFACTORY_HPP_
#define READERFACTORY_HPP_

#include <vector>
#include <iSpike/Reader/Reader.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/Reader/YarpVisualReader.hpp>
#include <iSpike/Reader/ReaderDescription.hpp>

/**
 * @class ReaderFactory
 * @brief A Factory of Readers
 *
 * The Reader Factory can list all Readers available in the system and can produce a particular type of a Reader
 */
class ReaderFactory {

private:
  /// A list of available readers
  std::vector<ReaderDescription> readerList;

public:

  /**
   * Default constructor
   * Initialises the list of readers, if you've made a new reader, add it here!
   */
  ReaderFactory()
  {
    this->readerList.push_back(FileAngleReader().getReaderDescription());
    this->readerList.push_back(FileVisualReader().getReaderDescription());
    this->readerList.push_back(YarpAngleReader().getReaderDescription());
    this->readerList.push_back(YarpVisualReader().getReaderDescription());
  }

  /**
   * Returns all readers of a particular type
   * @param readerType The type of a reader we are interested in
   * @return A list of all readers of a particular type
   */
  std::vector<ReaderDescription> getReadersOfType(std::string readerType)
  {
    std::vector<ReaderDescription> result;
    for(int i = 0; i < readerList.size(); i++)
    {
      if(readerList[i].getReaderType() == readerType)
        result.push_back(readerList[i]);
    }
    return result;
  }

  /**
   * Creates and initialises a particular reader
   * @param readerName Type of a Reader we want to create
   * @param readerProperties Initialisation properties for the new Reader
   * @return A new Reader
   */
  Reader* create(std::string readerType, std::map<std::string,Property*> readerProperties)
  {
    Reader* result;
    if(readerType == "File Angle Reader")
    {
      result = new FileAngleReader();
    } else if(readerType == "File Visual Reader")
    {
      result = new FileVisualReader();
    } else if(readerType == "Yarp Angle Reader")
    {
      result = new YarpAngleReader();
    } else if(readerType == "Yarp Visual Reader")
    {
      result = new YarpVisualReader();
    } else {
      throw std::logic_error("Invalid reader type");
    }
    result->initialise(readerProperties);
    return result;
  }
};


#endif /* READERFACTORY_HPP_ */
