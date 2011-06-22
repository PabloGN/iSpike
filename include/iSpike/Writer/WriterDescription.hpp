/*
 * WriterDescription.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef WRITERDESCRIPTION_HPP_
#define WRITERDESCRIPTION_HPP_

#include <string>
#include <map>
#include <iSpike/Property.hpp>

/**
 * @class WriterDescription
 * @brief Writer Description class
 *
 * This class describes a Writer.
 *
 * @author Edgars Lazdins
 *
 */
class WriterDescription {

private:
  /// Name of the Writer
  std::string writerName;

  ///Writer's description
  std::string writerDescription;

  ///Type of a Writer this is
  std::string writerType;

  ///A map of the Writer's properties
  std::map<std::string,Property*> writerProperties;

public:

  /**
   *  Default constructor, creates a new Writer Description with the given parameters
   *  @param writerName Name of the Writer
   *  @param writerDescription Description for the Writer
   *  @param writerType The type of a Writer this is
   *  @param writerProperties A map of the Writer's properties
   */
  WriterDescription(std::string writerName, std::string writerDescription, std::string writerType, std::map<std::string,Property*> writerProperties)
  {
    this->writerName = writerName;
    this->writerDescription = writerDescription;
    this->writerType = writerType;
    this->writerProperties = writerProperties;
  }

  WriterDescription(const WriterDescription& copy_from_me)
  {
    this->writerName = copy_from_me.writerName;
    this->writerDescription = copy_from_me.writerDescription;
    this->writerType = copy_from_me.writerType;
    this->writerProperties = std::map<std::string,Property*>();
    for(std::map<std::string,Property*>::const_iterator iter = copy_from_me.writerProperties.begin(); iter != copy_from_me.writerProperties.end(); ++iter)
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
      this->writerProperties[iter->first] = newProperty;
    }
  }

  ~WriterDescription()
  {
    for(std::map<std::string,Property*>::iterator iter = writerProperties.begin(); iter != writerProperties.end(); ++iter)
    {
      delete iter->second;
    }
    writerProperties.clear();
  }

  /**
   * Returns the Writer's description
   * @return Description of the Reader
   */
  std::string getWriterDescription() const
  {
      return writerDescription;
  }

  /**
   * Returns the name of the Writer
   * @return Name of the Writer
   */
  std::string getWriterName() const
  {
      return writerName;
  }

  /**
   * Returns the Writer's type
   * @return Writer's type
   */
  std::string getWriterType() const
  {
      return writerType;
  }

  /**
   * Returns a map of the Writer's properties
   * @return Map of the Writer's properties
   */
  std::map<std::string,Property*> getWriterProperties() const
  {
   /* std::map<std::string,Property*> result = std::map<std::string,Property*>();
    for(std::map<std::string,Property*>::const_iterator iter = this->writerProperties.begin(); iter != this->writerProperties.end(); ++iter)
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
    return this->writerProperties;
  }

};

#endif /* WRITERDESCRIPTION_HPP_ */
