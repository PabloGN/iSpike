/*
 * OutputChannelDescription.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef OUTPUTCHANNELDESCRIPTION_HPP_
#define OUTPUTCHANNELDESCRIPTION_HPP_

#include <string>

/**
 * @class OutputChannelDescription
 * @brief Output Channel Description class
 *
 * This class describes an output channel.
 *
 * @author Edgars Lazdins
 *
 */
class OutputChannelDescription {

private:
  /// Name of the Output Channel
  std::string channelName;

  /// A description of the channel
  std::string channelDescription;

  /// The type of writer this channel accepts
  std::string writerType;

  /// A map of the channel's properties
  std::map<std::string,Property*> channelProperties;

public:

  /**
   *  Default constructor, creates a new Output Channel Description with the given parameters
   *  @param channelName Name of the Output Channel
   *  @param channelDescription A description of the Output Channel
   *  @param writerType The type of Writer this Output Channel accepts
   *  @param channelProperties A map of Channel's properties
   */
  OutputChannelDescription(std::string channelName, std::string channelDescription, std::string writerType, std::map<std::string,Property*> channelProperties)
  {
    this->channelName = channelName;
    this->channelDescription = channelDescription;
    this->writerType = writerType;
    this->channelProperties = channelProperties;
  }

  /**
   * Retrieves the Channel description
   * @return Channel description
   */
  std::string getChannelDescription() const
  {
      return channelDescription;
  }

  /**
   * Retrives the Channel name
   * @return Channel name
   */
  std::string getChannelName() const
  {
      return channelName;
  }

  /**
   * Retrieves the type of Writer this Channel accepts
   * @return The type of writer this channel accepts
   */
  std::string getWriterType() const
  {
      return writerType;
  }

  /**
   * Retrieves the properties of the channel
   * @return Channel properties
   */
  std::map<std::string,Property*> getChannelProperties() const
  {
    return channelProperties;
  }

};


#endif /* OUTPUTCHANNELDESCRIPTION_HPP_ */
