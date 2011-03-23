/*
 * InputChannelDescription.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef INPUTCHANNELDESCRIPTION_HPP_
#define INPUTCHANNELDESCRIPTION_HPP_

#include <string>

/**
 * @class InputChannelDescription
 * @brief Input Channel Description class
 *
 * This class describes an input channel.
 *
 * @author Edgars Lazdins
 *
 */
class InputChannelDescription {

private:
  /// Name of the Input Channel
  std::string channelName;

  /// A description of the channel
  std::string channelDescription;

  /// The type of reader this channel accepts
  std::string readerType;

  /// A map of the channel's properties
  std::map<std::string,Property*> channelProperties;

public:

  /**
   *  Default constructor, creates a new Input Channel Description with the given parameters
   *  @param channelName Name of the Input Channel
   *  @param channelDescription A description of the Input Channel
   *  @param readerType The type of Reader this Input Channel accepts
   *  @param channelProperties A map of Channel's properties
   */
  InputChannelDescription(std::string channelName, std::string channelDescription, std::string readerType, std::map<std::string,Property*> channelProperties)
  {
    this->channelName = channelName;
    this->channelDescription = channelDescription;
    this->readerType = readerType;
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
   * Retrieves the type of Reader this Channel accepts
   * @return The type of reader this channel accepts
   */
  std::string getReaderType() const
  {
      return readerType;
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

#endif /* INPUTCHANNELDESCRIPTION_HPP_ */
