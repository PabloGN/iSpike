/*
 * OutputChannelFactory.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: cembo
 */

#ifndef OUTPUTCHANNELFACTORY_HPP_
#define OUTPUTCHANNELFACTORY_HPP_

#include <string>
#include <iSpike/Writer/Writer.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>

class OutputChannelFactory {

private:
  std::vector<OutputChannelDescription> channelList;

public:

  OutputChannelFactory()
  {
    this->channelList.push_back(JointOutputChannel().getChannelDescription());
  }

  std::vector<OutputChannelDescription> getAllChannels()
  {
    return channelList;
  }

  OutputChannel* create(std::string channelName, Writer* writer, std::map<std::string,Property*> channelProperties)
  {
    if(channelName == "Joint Output Channel")
    {
      JointOutputChannel* channel = new JointOutputChannel();
      channel->initialise((AngleWriter*)writer, channelProperties);
      return channel;
    } else {
      throw std::logic_error("Invalid channel type");
    }
  }
};

#endif /* OUTPUTCHANNELFACTORY_HPP_ */
