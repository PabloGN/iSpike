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

class WriterDescription {

private:
    std::string writerName;
    std::string writerDescription;
    std::string writerType;
    std::map<std::string,Property*> writerProperties;

public:

    WriterDescription(std::string writerName, std::string writerDescription, std::string writerType, std::map<std::string,Property*> writerProperties)
    {
      this->writerName = writerName;
      this->writerDescription = writerDescription;
      this->writerType = writerType;
      this->writerProperties = writerProperties;
    }

    std::string getWriterDescription() const
    {
        return writerDescription;
    }

    std::string getWriterName() const
    {
        return writerName;
    }

    std::string getWriterType() const
    {
        return writerType;
    }

    std::map<std::string,Property*> getWriterProperties() const
    {
      return writerProperties;
    }

};

#endif /* WRITERDESCRIPTION_HPP_ */
