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
    channelList.push_back(JointInputChannel::channelDescription);
    return channelList;
  }

public:

  static std::vector<InputChannelDescription> getAllChannels()
  {
    return channelList;
  }

  static InputChannel* create(std::string channelName, Reader* reader)
  {
    if(channelName == "Joint Input Channel")
      return new JointInputChannel((AngleReader*)reader);
  }
};


#endif /* INPUTCHANNELFACTORY_HPP_ */
