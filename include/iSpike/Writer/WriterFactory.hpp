/*
 * WriterFactory.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: cembo
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

class WriterFactory {

private:
  std::vector<WriterDescription> writerList;

public:

  /*
   * Default constructor
   * Initialises the list of writers, if you've made a new writer, add it here!
   */
  WriterFactory()
  {
    writerList.push_back(FileAngleWriter().getWriterDescription());
    writerList.push_back(YarpAngleWriter().getWriterDescription());
  }

  /*
   * Returns all writers of a particular type
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

  /*
   * Creates a particular writer
   */
  Writer* create(std::string writerName, std::map<std::string,Property*> writerProperties)
  {
    Writer* result;
    if(writerName == "File Angle Writer")
    {
      result = new FileAngleWriter();
    } else if(writerName == "Yarp Angle Writer")
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
