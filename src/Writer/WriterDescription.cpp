#include "iSpike/Writer/WriterDescription.hpp"
using namespace ispike;

/** Empty constructor */
WriterDescription::WriterDescription(){
	name = "Unnamed";
	description = "Undescribed";
	type = "Undefined";
}


/** Constructor */
WriterDescription::WriterDescription(string name, string description, string type) {
	this->name = name;
	this->description = description;
	this->type = type;
}


/** Copy constructor */
WriterDescription::WriterDescription(const WriterDescription& desc) {
	this->name = desc.name;
	this->description = desc.description;
	this->type = desc.type;
}


/** Assignment operator */
WriterDescription& WriterDescription::operator =(const WriterDescription& rhs) {
	if(&rhs == this)
		return *this;

	this->name = rhs.name;
	this->description = rhs.description;
	this->type = rhs.type;

	return *this;
}

