/*
 * InputChannelDescription.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: cembo
 */

#ifndef INPUTCHANNELDESCRIPTION_HPP_
#define INPUTCHANNELDESCRIPTION_HPP_

#include <string>

class InputChannelDescription {

private:
    std::string channelName;
    std::string channelDescription;
    std::string readerType;
    std::map<std::string,Property*> channelProperties;

public:

    InputChannelDescription(std::string channelName, std::string channelDescription, std::string readerType, std::map<std::string,Property*> channelProperties)
    {
      this->channelName = channelName;
      this->channelDescription = channelDescription;
      this->readerType = readerType;
      this->channelProperties = channelProperties;
    }

    std::string getChannelDescription() const
    {
        return channelDescription;
    }

    std::string getChannelName() const
    {
        return channelName;
    }

    std::string getReaderType() const
    {
        return readerType;
    }

    std::map<std::string,Property*> getChannelProperties() const
    {
      return channelProperties;
    }

};

#endif /* INPUTCHANNELDESCRIPTION_HPP_ */
