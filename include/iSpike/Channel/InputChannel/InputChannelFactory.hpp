/*
 * InputChannelFactory.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: cembo
 */

#ifndef INPUTCHANNELFACTORY_HPP_
#define INPUTCHANNELFACTORY_HPP_

#include <string>
#include <iSpike/Reader/Reader.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Channel/InputChannel/InputChannel.hpp>
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>

class InputChannelFactory {

private:
  static std::vector<InputChannelDescription> channelList;
  static std::vector<InputChannelDescription> initialiseChannelList()
  {
    std::vector<InputChannelDescription> channelList;
    channelList.push_back(JointInputChannel().getChannelDescription());
    channelList.push_back(VisualInputChannel().getChannelDescription());
    return channelList;
  }

public:

  static std::vector<InputChannelDescription> getAllChannels()
  {
    return channelList;
  }

  static InputChannel* create(std::string channelName, Reader* reader, std::map<std::string,Property*> channelProperties)
  {
    if(channelName == "Joint Input Channel")
    {
      JointInputChannel* channel = new JointInputChannel();
      channel->initialise((AngleReader*)reader, channelProperties);
      return channel;
    } else if(channelName == "Visual Input Channel")
    {
      VisualInputChannel* channel = new VisualInputChannel();
      channel->initialise((VisualReader*)reader, channelProperties);
      return channel;
    } else {
      throw std::logic_error("Invalid channel type");
    }
  }
};


#endif /* INPUTCHANNELFACTORY_HPP_ */
