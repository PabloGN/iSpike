/*
 * OutputChannelFactory.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef OUTPUTCHANNELFACTORY_HPP_
#define OUTPUTCHANNELFACTORY_HPP_

#include <string>
#include <iSpike/Writer/Writer.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>

/**
 * @class OutputChannelFactory
 * @brief A Factory of Output Channels
 *
 * The Output Channel Factory can list all Output Channels available in the system and can produce a particular type of an Output Channel
 */
class OutputChannelFactory {

private:
  /// A list of available Output Channels
  std::vector<OutputChannelDescription> channelList;

public:

  /**
   * Default constructor
   * Initialises the list of Output Channels, if you've made a new Output Channel, add it here!
   */
  OutputChannelFactory()
  {
    this->channelList.push_back(JointOutputChannel().getChannelDescription());
  }

  /**
   * Returns all Output Channels in the system
   * @return All available Output Channels
   */
  std::vector<OutputChannelDescription> getAllChannels()
  {
    return channelList;
  }

  /**
   * Creates a particular Output Channel
   * @param channelType Type of Output Channel to create
   * @param writer A writer for the new Output Channel
   * @param channelProperties A map of properties for the new Output Channel
   * @return An initialised Output Channel of a given type
   */
  OutputChannel* create(std::string channelType, Writer* writer, std::map<std::string,Property*> channelProperties)
  {
    if(channelType == "Joint Output Channel")
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
