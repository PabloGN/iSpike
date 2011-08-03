#include <iSpike/Reader/ReaderDescription.hpp>
using namespace ispike;

/** Empty constructor */
ReaderDescription::ReaderDescription(){
	name = "Unnamed";
	description = "Undescribed";
	type = "Unknown";
}

/** Constructor */
ReaderDescription::ReaderDescription(string readerName, string readerDescription, string readerType) {
	this->name = readerName;
	this->description = readerDescription;
	this->type = readerType;
}


/** Copy constructor */
ReaderDescription::ReaderDescription(const ReaderDescription& desc) {
	this->name = desc.name;
	this->description = desc.description;
	this->type = desc.type;
}


/** Assignment operator */
ReaderDescription& ReaderDescription::operator =(const ReaderDescription& rhs) {
	if(&rhs == this)
		return *this;

	this->name = rhs.name;
	this->description = rhs.description;
	this->type = rhs.type;

	return *this;
}

