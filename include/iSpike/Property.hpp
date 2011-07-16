/*
 * Property.hpp
 *
 *  Created on: 12 Mar 2011
 *      Author: Edgars Lazdins
 */

#ifndef PROPERTY_HPP_
#define PROPERTY_HPP_

#include <map>
#include <string>
#include <vector>
#include <iSpike/Log/Log.hpp>
#include <boost/lexical_cast.hpp>

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
		bool readOnly;

	public:

		virtual std::string toString()
		{

		}

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

		bool isReadOnly()
		{
			return this->readOnly;
		}

		virtual ~Property()
		{

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

		std::string toString()
		{
			return boost::lexical_cast<std::string>(this->value);
		}

		int getValue()
		{
			return this->value;
		}

		void setValue(int value)
		{
			this->value = value;
		}

		IntegerProperty(std::string paramName, int paramValue, std::string paramDescription, bool readOnly)
		{
			this->name = paramName;
			this->value = paramValue;
			this->description = paramDescription;
			this->type = Property::Integer;
			this->readOnly = readOnly;
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

		std::string toString()
		{
			return boost::lexical_cast<std::string>(this->value);
		}

		double getValue()
		{
			return this->value;
		}

		void setValue(double value)
		{
			this->value = value;
		}

		DoubleProperty(std::string paramName, double paramValue, std::string paramDescription, bool readOnly)
		{
			this->name = paramName;
			this->value = paramValue;
			this->description = paramDescription;
			this->type = Property::Double;
			this->readOnly = readOnly;
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

		std::string toString()
		{
			return this->value;
		}

		std::string getValue()
		{
			return this->value;
		}

		void setValue(std::string value)
		{
			LOG(LOG_DEBUG) << "Setting value to " << value;
			this->value = value;
		}

		StringProperty(std::string paramName, std::string paramValue, std::string paramDescription, bool readOnly)
		{
			this->name = paramName;
			this->value = paramValue;
			this->description = paramDescription;
			this->type = Property::String;
			this->readOnly = readOnly;
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

		std::string toString()
		{
			return this->value;
		}

		std::string getValue()
		{
			return this->value;
		}

		/**
		 * Fills in the value from the options
		 */
		void setValue(int value)
		{
			LOG(LOG_DEBUG) << "Setting value to " << this->options.at(value);
			this->value = this->options.at(value);
		}

		/**
		 * Fills in the value directly
		 */
		void setValue(std::string value)
		{
			this->value = value;
		}

		std::vector<std::string> getOptions()
		{
			return this->options;
		}

		ComboProperty(std::string paramName, std::string paramValue, std::string paramDescription,
				std::vector<std::string> paramOptions, bool readOnly)
		{
			this->name = paramName;
			this->value = paramValue;
			this->description = paramDescription;
			this->type = Property::Combo;
			this->options = paramOptions;
			this->readOnly = readOnly;
		}
};



typedef std::map< std::string, Property*> property_map;


#endif /* PROPERTY_HPP_ */
