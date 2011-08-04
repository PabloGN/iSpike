#include "iSpike/Channel/OutputChannel/OutputChannelFactory.hpp"
#include "iSpike/ISpikeException.hpp"
using namespace ispike;


/** Default constructor
	 Initialises the list of Output Channels, if you've made a new Output Channel, add it here!  */
OutputChannelFactory::OutputChannelFactory()  {
	this->channelList.push_back(JointOutputChannel().getChannelDescription());
}


/** Returns all Output Channels in the system */
vector<OutputChannelDescription> OutputChannelFactory::getAllChannels() {
	return channelList;
}


/** Creates a particular named Output Channel  */
OutputChannel* OutputChannelFactory::create(string channelName, Writer* writer, map<string,Property>& channelProperties) {
	if(channelName == "Joint Output Channel") {
		JointOutputChannel* channel = new JointOutputChannel();
		channel->initialize((AngleWriter*)writer, channelProperties);
		return channel;
	}
	throw ISpikeException("Invalid channel type");
}
