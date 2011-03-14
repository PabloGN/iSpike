/*
 * ReaderDescription.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: cembo
 */

#ifndef READERDESCRIPTION_HPP_
#define READERDESCRIPTION_HPP_

#include <string>
#include <map>
#include <iSpike/Property.hpp>

class ReaderDescription {

private:
    std::string readerName;
    std::string readerDescription;
    std::string readerType;
    std::map<std::string,Property*> readerProperties;

public:

    ReaderDescription(std::string readerName, std::string readerDescription, std::string readerType, std::map<std::string,Property*> readerProperties)
    {
      this->readerName = readerName;
      this->readerDescription = readerDescription;
      this->readerType = readerType;
      this->readerProperties = readerProperties;
    }

    std::string getReaderDescription() const
    {
        return readerDescription;
    }

    std::string getReaderName() const
    {
        return readerName;
    }

    std::string getReaderType() const
    {
        return readerType;
    }

    std::map<std::string,Property*> getReaderProperties() const
    {
      return readerProperties;
    }

};


#endif /* READERDESCRIPTION_HPP_ */
