#include "iSpike/Channel/OutputChannel/OutputChannelFactory.hpp"
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include "iSpike/ISpikeException.hpp"
using namespace ispike;


/** Default constructor
	 Initialises the list of Output Channels, if you've made a new Output Channel, add it here!  */
OutputChannelFactory::OutputChannelFactory()  {
	this->channelList.push_back(JointOutputChannel().getChannelDescription());
	printOutputChannels();
}


/** Returns all Output Channels in the system */
vector<Description> OutputChannelFactory::getAllChannels() {
	return channelList;
}


/** Creates a particular named Output Channel  */
OutputChannel* OutputChannelFactory::create(Description& desc, Writer* writer, map<string,Property>& channelProperties) {
	if(desc.getName() == "Joint Output Channel") {
		JointOutputChannel* channel = new JointOutputChannel();
		channel->initialize((AngleWriter*)writer, channelProperties);
		return channel;
	}
	throw ISpikeException("Invalid channel type");
}


/*! Returns the default properties of a particular output channel */
map<string, Property> OutputChannelFactory::getDefaultProperties(Description& desc){
	if(desc.getName() == "Joint Output Channel") {
		return JointOutputChannel().getProperties();
	}
	throw ISpikeException("Invalid output channel");
}



/** Prints out the available input channels */
void OutputChannelFactory::printOutputChannels(){
	for(size_t i=0; i<channelList.size(); ++i)
		cout<<"Output Channel: "<<channelList[i].getName()<<", "<<channelList[i].getDescription()<<endl;
}


