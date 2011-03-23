/*
 * WriterFactory.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef WRITERFACTORY_HPP_
#define WRITERFACTORY_HPP_

#include <vector>
#include <iSpike/Writer/Writer.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Writer/YarpAngleWriter.hpp>
#include <iSpike/Writer/FileAngleWriter.hpp>
#include <iSpike/Writer/WriterDescription.hpp>
#include <iSpike/Property.hpp>

/**
 * @class WriterFactory
 * @brief A Factory of Writers
 *
 * The Writer Factory can list all Writers available in the system and can produce a particular type of a Writer
 */
class WriterFactory {

private:
  /// A list of available writers
  std::vector<WriterDescription> writerList;

public:

  /**
   * Default constructor
   * Initialises the list of writers, if you've made a new writer, add it here!
   */
  WriterFactory()
  {
    writerList.push_back(FileAngleWriter().getWriterDescription());
    writerList.push_back(YarpAngleWriter().getWriterDescription());
  }

  /**
   * Returns all writers of a particular type
   * @param writerType The type of Writer we are interested in
   * @return All writers of the given type
   */
  std::vector<WriterDescription> getWritersOfType(std::string writerType)
  {
    std::vector<WriterDescription> result;
    for(int i = 0; i < writerList.size(); i++)
    {
      if(writerList[i].getWriterType() == writerType)
        result.push_back(writerList[i]);
    }
    return result;
  }

  /**
   * Creates a particular writer
   * @param writerType Type of a Writer we want to create
   * @param writerProperties A map of properties for the new Writer
   * @return An initialised Writer of a given type
   */
  Writer* create(std::string writerType, std::map<std::string,Property*> writerProperties)
  {
    Writer* result;
    if(writerType == "File Angle Writer")
    {
      result = new FileAngleWriter();
    } else if(writerType == "Yarp Angle Writer")
    {
      result = new YarpAngleWriter();
    } else {
      throw std::logic_error("Invalid writer type");
    }
    result->initialise(writerProperties);
    return result;
  }
};


#endif /* WRITERFACTORY_HPP_ */
