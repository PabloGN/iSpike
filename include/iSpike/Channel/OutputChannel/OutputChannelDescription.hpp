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

  OutputChannelDescription & operator= (const OutputChannelDescription & copy_from_me)
  {
    if (this != &copy_from_me) // protect against invalid self-assignment
    {
      this->channelName = copy_from_me.channelName;
      this->channelDescription = copy_from_me.channelDescription;
      this->writerType = copy_from_me.writerType;
      this->channelProperties = std::map<std::string,Property*>();
      for(std::map<std::string,Property*>::const_iterator iter = copy_from_me.channelProperties.begin(); iter != copy_from_me.channelProperties.end(); ++iter)
      {
        Property* newProperty;
        switch(iter->second->getType())
        {
        case Property::Integer:
          newProperty = new IntegerProperty(iter->second->getName(), ((IntegerProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
          break;
        case Property::Double:
          newProperty = new DoubleProperty(iter->second->getName(), ((DoubleProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
          break;
        case Property::String:
          newProperty = new StringProperty(iter->second->getName(), ((StringProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
          break;
        case Property::Combo:
          newProperty = new ComboProperty(iter->second->getName(), ((ComboProperty*)(iter->second))->getValue(), iter->second->getDescription(), ((ComboProperty*)(iter->second))->getOptions(), iter->second->isReadOnly());
          break;
        }
        this->channelProperties[iter->first] = newProperty;
      }
    }
    // by convention, always return *this
    return *this;
  }

  OutputChannelDescription(const OutputChannelDescription& copy_from_me)
  {
    this->channelName = copy_from_me.channelName;
    this->channelDescription = copy_from_me.channelDescription;
    this->writerType = copy_from_me.writerType;
    this->channelProperties = std::map<std::string,Property*>();
    for(std::map<std::string,Property*>::const_iterator iter = copy_from_me.channelProperties.begin(); iter != copy_from_me.channelProperties.end(); ++iter)
    {
      Property* newProperty;
      switch(iter->second->getType())
      {
      case Property::Integer:
        newProperty = new IntegerProperty(iter->second->getName(), ((IntegerProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
        break;
      case Property::Double:
        newProperty = new DoubleProperty(iter->second->getName(), ((DoubleProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
        break;
      case Property::String:
        newProperty = new StringProperty(iter->second->getName(), ((StringProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
        break;
      case Property::Combo:
        newProperty = new ComboProperty(iter->second->getName(), ((ComboProperty*)(iter->second))->getValue(), iter->second->getDescription(), ((ComboProperty*)(iter->second))->getOptions(), iter->second->isReadOnly());
        break;
      }
      this->channelProperties[iter->first] = newProperty;
    }
  }

  ~OutputChannelDescription()
  {
    for(std::map<std::string,Property*>::iterator iter = channelProperties.begin(); iter != channelProperties.end(); ++iter)
    {
      delete iter->second;
    }
    channelProperties.clear();
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
    /*std::map<std::string,Property*> result = std::map<std::string,Property*>();
    for(std::map<std::string,Property*>::const_iterator iter = this->channelProperties.begin(); iter != this->channelProperties.end(); ++iter)
    {
      Property* newProperty;
      switch(iter->second->getType())
      {
      case Property::Integer:
        newProperty = new IntegerProperty(iter->second->getName(), ((IntegerProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
        break;
      case Property::Double:
        newProperty = new DoubleProperty(iter->second->getName(), ((DoubleProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
        break;
      case Property::String:
        newProperty = new StringProperty(iter->second->getName(), ((StringProperty*)(iter->second))->getValue(), iter->second->getDescription(), iter->second->isReadOnly());
        break;
      case Property::Combo:
        newProperty = new ComboProperty(iter->second->getName(), ((ComboProperty*)(iter->second))->getValue(), iter->second->getDescription(), ((ComboProperty*)(iter->second))->getOptions(), iter->second->isReadOnly());
        break;
      }
      result[iter->first] = newProperty;
      }
      return result;*/
    return this->channelProperties;
  }

};


#endif /* OUTPUTCHANNELDESCRIPTION_HPP_ */
