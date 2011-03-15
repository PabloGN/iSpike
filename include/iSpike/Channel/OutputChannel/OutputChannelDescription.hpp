/*
 * OutputChannelDescription.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: cembo
 */

#ifndef OUTPUTCHANNELDESCRIPTION_HPP_
#define OUTPUTCHANNELDESCRIPTION_HPP_

#include <string>

class OutputChannelDescription {

private:
    std::string channelName;
    std::string channelDescription;
    std::string writerType;
    std::map<std::string,Property*> channelProperties;

public:

    OutputChannelDescription(std::string channelName, std::string channelDescription, std::string writerType, std::map<std::string,Property*> channelProperties)
    {
      this->channelName = channelName;
      this->channelDescription = channelDescription;
      this->writerType = writerType;
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

    std::string getWriterType() const
    {
        return writerType;
    }

    std::map<std::string,Property*> getChannelProperties() const
    {
      return channelProperties;
    }

};


#endif /* OUTPUTCHANNELDESCRIPTION_HPP_ */
