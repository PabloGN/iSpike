#include "iSpike/Property.hpp"
using namespace ispike;

/** Empty constructor */
Property::Property(){
	type = Property::Undefined;
	name = "Unnamed";
	description = "Undescribed";
	readOnly = false;

	intVal = 0;
	doubleVal = 0.0;
	stringVal = "Undefined";
}


/** Integer property constructor */
Property::Property(int value, string name, string description, bool readOnly){
	this->type = Property::Integer;
	this->intVal = value;
	this->name = name;
	this->description = description;
	this->readOnly = readOnly;

	doubleVal = 0.0;
	stringVal = "Undefined";
}


/** Double property constructor */
Property::Property(double value, string name, string description, bool readOnly){
	this->type = Property::Double;
	this->doubleVal = value;
	this->name = name;
	this->description = description;
	this->readOnly = readOnly;

	this->intVal = 0;
	this->stringVal = "Undefined";
}


/** String property constructor */
Property::Property(string value, string name, string description, bool readOnly){
	this->type = Property::String;
	this->stringVal = value;
	this->name = name;
	this->description = description;
	this->readOnly = readOnly;

	this->intVal = 0;
	this->doubleVal = 0.0;
}


/** Combo property constructor */
Property::Property(string value, vector<string> options, string name, string description, bool readOnly){
	this->type = Property::Combo;
	this->options = options;
	this->stringVal = value;
	this->name = name;
	this->description = description;
	this->readOnly = readOnly;

	this->intVal = 0;
	this->doubleVal = 0.0;
}


/** Property copy constructor */
Property::Property(const Property& prop){
	this->type = prop.type;
	this->name = prop.name;
	this->description = prop.description;
	this->readOnly = prop.readOnly;
	this->intVal = prop.intVal;
	this->doubleVal = prop.doubleVal;
	this->stringVal = prop.stringVal;
	this->options = prop.options;
}


/** Property destructor */
Property::~Property(){
}


/** Property assignment operator */
Property& Property::operator=(const Property& rhs){
	//Check for self assignment
	if(this == &rhs)
		return *this;

	this->type = rhs.type;
	this->name = rhs.name;
	this->description = rhs.description;
	this->readOnly = rhs.readOnly;
	this->intVal = rhs.intVal;
	this->doubleVal = rhs.doubleVal;
	this->stringVal = rhs.stringVal;
	this->options = rhs.options;

	return *this;
}


