#include "iSpike/Property.hpp"
using namespace ispike;

/** Property constructor */
Property::Property(ValueType type, string name, string description, bool readOnly){
	this->type = type;
	this->name = name;
	this->description = description;
	this->readOnly = readOnly;
}

/** Property copy constructor */
Property::Property(const Property& prop){
	this->type = prop.type;
	this->name = prop.name;
	this->description = prop.description;
	this->readOnly = prop.readOnly;
}

/** Property assignment operator */
Property::Property& Property::operator=(const Property& rhs){
	//Check for self assignment
	if(this == &rhs)
		return *this;

	this->type = rhs.type;
	this->name = rhs.name;
	this->description = rhs.description;
	this->readOnly = rhs.readOnly;
	return *this;
}


/** IntegerProperty constructor */
IntegerProperty::IntegerProperty(int paramValue, string name, string description, bool readOnly) : Property(Property::Integer, name, description, readOnly) {
	this->value = paramValue;
}

/** IntegerProperty copy constructor */
IntegerProperty::IntegerProperty(const IntegerProperty& intProp) : Property(*((Property*)this)){
	this->value = intProp.getValue();
}

/*! IntegerProperty assignment operator */
IntegerProperty& IntegerProperty::operator=(const IntegerProperty& rhs){
	//Check for self assignment
	if(this == &rhs)
		return *this;

	//Call superclass assignment
	Property::operator=(rhs);

	//Copy our values and return
	this->value = rhs.value;
	return *this;
}


/** DoubleProperty constructor */
DoubleProperty::DoubleProperty(double paramValue, string name, string description, bool readOnly) : Property(Property::Double, name, description, readOnly) {
	this->value = paramValue;
}

/** DoubleProperty copy constructor */
DoubleProperty::DoubleProperty(const DoubleProperty& doubProp) : Property(*((Property*)this)){
	this->value = doubProp.getValue();
}

/*! DoubleProperty assignment operator */
DoubleProperty& DoubleProperty::operator=(const DoubleProperty& rhs){
	//Check for self assignment
	if(this == &rhs)
		return *this;

	//Call superclass assignment
	Property::operator=(rhs);

	//Copy our values and return
	this->value = rhs.value;
	return *this;
}


/** StringProperty constructor */
StringProperty::StringProperty(string paramValue, string name, string description, bool readOnly) : Property(Property::String, name, description, readOnly) {
	this->value = paramValue;
}

/** StringProperty copy constructor */
StringProperty::StringProperty(const StringProperty& strProp) : Property(*((Property*)this)){
	this->value = strProp.getValue();
}

/*! StringProperty assignment operator */
StringProperty& StringProperty::operator=(const StringProperty& rhs){
	//Check for self assignment
	if(this == &rhs)
		return *this;

	//Call superclass assignment
	Property::operator=(rhs);

	//Copy our values and return
	this->value = rhs.value;
	return *this;
}


/** ComboProperty constructor */
ComboProperty::ComboProperty(vector<string> paramOptions, string paramValue, string name, string description, bool readOnly) : Property(Property::Combo, name, description, readOnly) {
	this->value = paramValue;
	this->options = paramOptions;
}

/** ComboProperty copy constructor */
ComboProperty::ComboProperty(const ComboProperty& strProp) : Property(*((Property*)this)){
	this->value = strProp.value;
	this->options = strProp.options;
}

/*! ComboProperty assignment operator */
ComboProperty& ComboProperty::operator=(const ComboProperty& rhs){
	//Check for self assignment
	if(this == &rhs)
		return *this;

	//Call superclass assignment
	Property::operator=(rhs);

	//Copy our values and return
	this->value = rhs.value;
	this->options = rhs.options;
	return *this;
}
