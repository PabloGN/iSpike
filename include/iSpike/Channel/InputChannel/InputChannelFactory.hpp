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

/**
 * @class InputChannelFactory
 * @brief A Factory of Input Channels
 *
 * The Input Channel Factory can list all Input Channels available in the system and can produce a particular type of an Input Channel
 */
class InputChannelFactory {

private:
  /// A list of available Input Channels
  std::vector<InputChannelDescription> channelList;

public:

  /**
   * Default constructor
   * Initialises the list of Input Channels, if you've made a new Input Channel, add it here!
   */
  InputChannelFactory()
  {
    this->channelList.push_back(JointInputChannel().getChannelDescription());
    this->channelList.push_back(VisualInputChannel().getChannelDescription());
  }

  /**
   * Returns all Input Channels in the system
   * @return All available Input Channels
   */
  std::vector<InputChannelDescription> getAllChannels()
  {
    return channelList;
  }

  /**
   * Creates a particular Input Channel
   * @param channelType Type of Input Channel to create
   * @param reader A reader for the new Input Channel
   * @param channelProperties A map of properties for the new Input Channel
   * @return An initialised Input Channel of a given type
   */
  InputChannel* create(std::string channelType, Reader* reader, std::map<std::string,Property*> channelProperties)
  {
    if(channelType == "Joint Input Channel")
    {
      JointInputChannel* channel = new JointInputChannel();
      channel->initialise((AngleReader*)reader, channelProperties);
      return channel;
    } else if(channelType == "Visual Input Channel")
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
