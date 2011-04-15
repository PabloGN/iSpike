/*
 * ReaderDescription.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef READERDESCRIPTION_HPP_
#define READERDESCRIPTION_HPP_

#include <string>
#include <map>
#include <iSpike/Property.hpp>
#include <iSpike/Log/Log.hpp>

/**
 * @class ReaderDescription
 * @brief Reader Description class
 *
 * This class describes a Reader.
 *
 * @author Edgars Lazdins
 *
 */
class ReaderDescription {

private:
  ///Name of the Reader
  std::string readerName;

  ///Description of the Reader
  std::string readerDescription;

  /// Type of the Reader
  std::string readerType;

  ///Reader's properties
  std::map<std::string,Property*> readerProperties;

public:

  /**
   *  Default constructor, creates a new Reader Description with the given parameters
   *  @param readerName Name of the Reader
   *  @param readerDescription Description for the Reader
   *  @param readerType The type of a Reader this is
   *  @param readerProperties A map of the Reader's properties
   */
  ReaderDescription(std::string readerName, std::string readerDescription, std::string readerType, std::map<std::string,Property*> readerProperties)
  {
    this->readerName = readerName;
    this->readerDescription = readerDescription;
    this->readerType = readerType;
    this->readerProperties = readerProperties;
  }

  ReaderDescription(const ReaderDescription& copy_from_me)
  {
    this->readerName = copy_from_me.readerName;
    this->readerDescription = copy_from_me.readerDescription;
    this->readerType = copy_from_me.readerType;
    this->readerProperties = std::map<std::string,Property*>();
    for(std::map<std::string,Property*>::const_iterator iter = copy_from_me.readerProperties.begin(); iter != copy_from_me.readerProperties.end(); ++iter)
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
      this->readerProperties[iter->first] = newProperty;
    }
  }

  ~ReaderDescription()
  {
    for(std::map<std::string,Property*>::iterator iter = readerProperties.begin(); iter != readerProperties.end(); ++iter)
    {
      delete iter->second;
    }
    readerProperties.clear();
  }

  /**
   * Returns the Reader's description
   * @return Description of the Reader
   */
  std::string getReaderDescription() const
  {
      return readerDescription;
  }

  /**
   * Returns the name of the Reader
   * @return Name of the Reader
   */
  std::string getReaderName() const
  {
      return readerName;
  }

  /**
   * Returns the Reader's type
   * @return Reader's type
   */
  std::string getReaderType() const
  {
      return readerType;
  }

  /**
   * Returns a map of the Reader's properties
   * @return Map of the Reader's properties
   */
  std::map<std::string,Property*> getReaderProperties() const
  {
    std::map<std::string,Property*> result = std::map<std::string,Property*>();
    for(std::map<std::string,Property*>::const_iterator iter = this->readerProperties.begin(); iter != this->readerProperties.end(); ++iter)
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
      return result;
  }

};


#endif /* READERDESCRIPTION_HPP_ */
