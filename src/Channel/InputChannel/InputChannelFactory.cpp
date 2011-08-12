//iSpike includes
#include "iSpike/Channel/InputChannel/InputChannelFactory.hpp"
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include "iSpike/ISpikeException.hpp"
#include "iSpike/Log/Log.hpp"
using namespace ispike;


/** Default constructor
	Initialises the list of Input Channels, if you've made a new Input Channel, add it here!  */
InputChannelFactory::InputChannelFactory(){
	channelList.push_back(JointInputChannel().getChannelDescription());
	channelList.push_back(VisualInputChannel().getChannelDescription());
	printInputChannels();
}


/** Returns all Input Channels in the system */
vector<Description> InputChannelFactory::getAllChannels() {
	return channelList;
}


/** Creates a particular Input Channel */
InputChannel* InputChannelFactory::create(Description& desc, Reader* reader, map<string, Property>& channelProperties) {
	if(desc.getName() == "Joint Input Channel"){
		JointInputChannel* channel = new JointInputChannel();
		channel->initialize((AngleReader*)reader, channelProperties);
		return channel;
	}
	else if(desc.getName() == "Visual Input Channel"){
		VisualInputChannel* channel = new VisualInputChannel();
		channel->initialize((VisualReader*)reader, channelProperties);
		return channel;
	}
	throw ISpikeException("Invalid channel name");
}



/*! Returns the default properties of a particular input channel */
map<string, Property> InputChannelFactory::getDefaultProperties(Description& desc){
	if(desc.getName() == "Joint Input Channel") {
		return JointInputChannel().getProperties();
	}
	else if(desc.getName() == "Visual Input Channel") {
		return VisualInputChannel().getProperties();
	}
	throw ISpikeException("Invalid input channel");
}


/** Prints out the available input channels */
void InputChannelFactory::printInputChannels(){
	for(size_t i=0; i<channelList.size(); ++i)
		LOG(LOG_DEBUG)<<"Input Channel: "<<channelList[i].getName()<<", "<<channelList[i].getDescription();
}
