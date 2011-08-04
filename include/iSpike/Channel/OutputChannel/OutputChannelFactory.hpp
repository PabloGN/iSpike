#ifndef OUTPUTCHANNELFACTORY_HPP_
#define OUTPUTCHANNELFACTORY_HPP_

//iSpike includes
#include <iSpike/Writer/Writer.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>

//Other includes
#include <string>
using namespace std;

namespace ispike {

	/** The Output Channel Factory can list all Output Channels available in the system and can produce a particular type of an Output Channel 	  */
	class OutputChannelFactory {
		private:
			/** A list of available Output Channels */
			vector<OutputChannelDescription> channelList;

		public:
			OutputChannelFactory();
			vector<OutputChannelDescription> getAllChannels();
			OutputChannel* create(string channelName, Writer* writer, map<string, Property>& channelProperties);
		};

}

#endif /* OUTPUTCHANNELFACTORY_HPP_ */
