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

  InputChannelDescription & operator= (const InputChannelDescription & copy_from_me)
  {
    if (this != &copy_from_me) // protect against invalid self-assignment
    {
      this->channelName = copy_from_me.channelName;
      this->channelDescription = copy_from_me.channelDescription;
      this->readerType = copy_from_me.readerType;
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

  ~InputChannelDescription()
  {
    for(std::map<std::string,Property*>::iterator iter = channelProperties.begin(); iter != channelProperties.end(); ++iter)
    {
      delete iter->second;
    }
    channelProperties.clear();
  }

  InputChannelDescription(const InputChannelDescription& copy_from_me)
  {
    this->channelName = copy_from_me.channelName;
    this->channelDescription = copy_from_me.channelDescription;
    this->readerType = copy_from_me.readerType;
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

#endif /* INPUTCHANNELDESCRIPTION_HPP_ */
