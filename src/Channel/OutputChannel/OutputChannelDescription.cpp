//iSpike includes
#include "iSpike/Channel/OutputChannel/OutputChannelDescription.hpp"
using namespace ispike;


/** Empty constructor */
OutputChannelDescription::OutputChannelDescription(){
	channelName = "Unnamed";
	channelDescription = "Undescribed";
	writerType = "Undefined";
}


/** Constructor */
OutputChannelDescription::OutputChannelDescription(string channelName, string channelDescription, string writerType){
	this->channelName = channelName;
	this->channelDescription = channelDescription;
	this->writerType = writerType;
}


/** Copy constructor */
OutputChannelDescription::OutputChannelDescription(const OutputChannelDescription& copy_from_me){
	this->channelName = copy_from_me.channelName;
	this->channelDescription = copy_from_me.channelDescription;
	this->writerType = copy_from_me.writerType;
}


/** Destructor */
OutputChannelDescription::~OutputChannelDescription(){
}


/** Assignment operator */
OutputChannelDescription & OutputChannelDescription::operator= (const OutputChannelDescription & copy_from_me){
	if (this != &copy_from_me){ // protect against invalid self-assignment
		this->channelName = copy_from_me.channelName;
		this->channelDescription = copy_from_me.channelDescription;
		this->writerType = copy_from_me.writerType;
	}

	// by convention, always return *this
	return *this;
}

