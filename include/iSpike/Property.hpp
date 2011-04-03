/*
 * Property.hpp
 *
 *  Created on: 12 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef PROPERTY_HPP_
#define PROPERTY_HPP_

#include <string>
#include <vector>

/**
 * Describes a general Channel, Reader or Writer Property
 */
class Property
{
 public:
    enum ValueType {
          Integer,
          Double,
          String,
          Combo
       };
  protected:
    ValueType type;
    std::string name;
    std::string description;

  public:

    ValueType getType()
    {
      return this->type;
    }

    std::string getName()
    {
      return this->name;
    }

    std::string getDescription()
    {
      return this->description;
    }

};

/**
 * Describer an Integer Property
 */
class IntegerProperty : public Property
{
  private:
    int value;

  public:

    int getValue()
    {
      return this->value;
    }

    void setValue(int value)
    {
      this->value = value;
    }

    IntegerProperty(std::string paramName, int paramValue, std::string paramDescription)
    {
      this->name = paramName;
      this->value = paramValue;
      this->description = paramDescription;
      this->type = Property::Integer;
    }

    IntegerProperty (const IntegerProperty& copy_from_me)
    {
      this->description = copy_from_me.description;
      this->name = copy_from_me.name;
      this->type = copy_from_me.type;
      this->value = copy_from_me.value;
    }
};

/**
 * Describes a Double Property
 */
class DoubleProperty : public Property
{
  private:
    double value;

  public:

    double getValue()
    {
      return this->value;
    }

    void setValue(double value)
    {
      this->value = value;
    }

    DoubleProperty(std::string paramName, double paramValue, std::string paramDescription)
    {
      this->name = paramName;
      this->value = paramValue;
      this->description = paramDescription;
      this->type = Property::Double;
    }

    DoubleProperty (const DoubleProperty& copy_from_me)
    {
      this->description = copy_from_me.description;
      this->name = copy_from_me.name;
      this->type = copy_from_me.type;
      this->value = copy_from_me.value;
    }
};

/**
 * Describes a String Property
 */
class StringProperty : public Property
{
  private:
    std::string value;

  public:

    std::string getValue()
    {
      return this->value;
    }

    void setValue(std::string value)
    {
      this->value = value;
    }

    StringProperty(std::string paramName, std::string paramValue, std::string paramDescription)
    {
      this->name = paramName;
      this->value = paramValue;
      this->description = paramDescription;
      this->type = Property::String;
    }

    StringProperty (const StringProperty& copy_from_me)
    {
      this->description = copy_from_me.description;
      this->name = copy_from_me.name;
      this->type = copy_from_me.type;
      this->value = copy_from_me.value;
    }
};

/**
 * Describes a Combo Property
 */
class ComboProperty : public Property
{
  private:
    std::string value;
    std::vector<std::string> options;

  public:

    std::string getValue()
    {
      return this->value;
    }

    void setValue(int value)
    {
      this->value = this->options.at(value);
    }

    std::vector<std::string> getOptions()
    {
      return this->options;
    }

    ComboProperty(std::string paramName, std::string paramValue, std::string paramDescription,
    		std::vector<std::string> paramOptions)
    {
      this->name = paramName;
      this->value = paramValue;
      this->description = paramDescription;
      this->type = Property::Combo;
      this->options = paramOptions;
    }

    ComboProperty (const ComboProperty& copy_from_me)
    {
      this->description = copy_from_me.description;
      this->name = copy_from_me.name;
      this->type = copy_from_me.type;
      this->value = copy_from_me.value;
      this->options = copy_from_me.options;
    }
};


#endif /* PROPERTY_HPP_ */
