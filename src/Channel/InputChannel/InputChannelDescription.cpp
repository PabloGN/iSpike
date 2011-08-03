//iSpike includes
#include "iSpike/Channel/InputChannel/InputChannelDescription.hpp"
using namespace ispike;

/** Empty constructor */
InputChannelDescription::InputChannelDescription(){
	channelName = "Unnamed";
	channelDescription = "Undescribed";
	readerType = "Unknown";
}


/**  Default constructor */
InputChannelDescription::InputChannelDescription(string channelName, string channelDescription, string readerType){
	this->channelName = channelName;
	this->channelDescription = channelDescription;
	this->readerType = readerType;
}


/** Copy constructor */
InputChannelDescription::InputChannelDescription(const InputChannelDescription& copy_from_me){
	this->channelName = copy_from_me.channelName;
	this->channelDescription = copy_from_me.channelDescription;
	this->readerType = copy_from_me.readerType;
}


/** Assignment operator */
InputChannelDescription & InputChannelDescription::operator= (const InputChannelDescription & copy_from_me) {
	if (this != &copy_from_me) {// protect against invalid self-assignment
		this->channelName = copy_from_me.channelName;
		this->channelDescription = copy_from_me.channelDescription;
		this->readerType = copy_from_me.readerType;
	}

	// by convention, always return *this
	return *this;
}


/** Destructor */
InputChannelDescription::~InputChannelDescription() {
}

