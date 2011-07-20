//iSpike includes
#include "iSpike/Channel/InputChannel/InputChannelDescription.hpp"
using namespace ispike;

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


/** Assignment operator */
OutputChannelDescription::OutputChannelDescription & operator= (const OutputChannelDescription & copy_from_me){
	if (this != &copy_from_me){ // protect against invalid self-assignment
		this->channelName = copy_from_me.channelName;
		this->channelDescription = copy_from_me.channelDescription;
		this->writerType = copy_from_me.writerType;
	}

	// by convention, always return *this
	return *this;
}


/** Destructor */
OutputChannelDescription::~OutputChannelDescription(){
}
