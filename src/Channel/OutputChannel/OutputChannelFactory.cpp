#include "iSpike/Channel/OutputChannel/OutputChannelFactory.hpp"
#include "iSpike/ISpikeException.hpp"
using namespace ispike;


/**
 * Default constructor
 * Initialises the list of Output Channels, if you've made a new Output Channel, add it here!  */
OutputChannelFactory::OutputChannelFactory()  {
	this->channelList.push_back(JointOutputChannel().getChannelDescription());
}


/**
 * Returns all Output Channels in the system
 * @return All available Output Channels
 */
std::vector<OutputChannelDescription> OutputChannelFactory::getAllChannels() {
	return channelList;
}


/**
 * Creates a particular Output Channel
 * @param channelName Type of Output Channel to create
 * @param writer A writer for the new Output Channel
 * @param channelProperties A map of properties for the new Output Channel
 * @return An initialised Output Channel of a given type
 */
OutputChannel* OutputChannelFactory::create(std::string channelName, Writer* writer, std::map<std::string,Property*> channelProperties) {
	if(channelName == "Joint Output Channel") {
		JointOutputChannel* channel = new JointOutputChannel();
		channel->initialize((AngleWriter*)writer, channelProperties);
		return channel;
	}
	else {
		throw iSpikeException("Invalid channel type");
	}
}
