#include "iSpike/Channel/InputChannel/InputChannelFactory.hpp"
#include "iSpike/ISpikeException.hpp"
using namespace ispike;


/**
* Default constructor
* Initialises the list of Input Channels, if you've made a new Input Channel, add it here!  */
InputChannelFactory(){
	LOG(LOG_DEBUG) << "InputChannelFactory: Adding channel descriptions to list.";
	this->channelList.push_back(JointInputChannel().getChannelDescription());
	this->channelList.push_back(VisualInputChannel().getChannelDescription());
	LOG(LOG_DEBUG) << "InputChannelFactory: Channel descriptions successfully added to list.";
}


/**
* Returns all Input Channels in the system
* @return All available Input Channels
*/
std::vector<InputChannelDescription> getAllChannels() {
	return channelList;
}


/** Creates a particular Input Channel */
InputChannel* create(string channelName, Reader* reader, map<string, Property> channelProperties) {
	if(channelName == "Joint Input Channel"){
		JointInputChannel* channel = new JointInputChannel();
		channel->initialize((AngleReader*)reader, channelProperties);
		return channel;
	}
	else if(channelName == "Visual Input Channel"){
		VisualInputChannel* channel = new VisualInputChannel();
		channel->initialize((VisualReader*)reader, channelProperties);
		return channel;
	}
	else {
		throw iSpikeException("Invalid channel name");
	}
}
