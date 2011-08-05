#ifndef OUTPUTCHANNELFACTORY_HPP_
#define OUTPUTCHANNELFACTORY_HPP_

//iSpike includes
#include "iSpike/Channel/OutputChannel/OutputChannel.hpp"
#include <iSpike/Writer/Writer.hpp>

//Other includes
#include <string>
using namespace std;

namespace ispike {

	/** The Output Channel Factory can list all Output Channels available in the system and can produce a particular type of an Output Channel 	  */
	class OutputChannelFactory {
		private:
			/** A list of available Output Channels */
			vector<Description> channelList;

			//============================  METHODS  =======================
			void printOutputChannels();


		public:
			OutputChannelFactory();
			vector<Description> getAllChannels();
			OutputChannel* create(Description& desc, Writer* writer, map<string, Property>& channelProperties);
			map<string, Property> getDefaultProperties(Description& desc);

		};

}

#endif /* OUTPUTCHANNELFACTORY_HPP_ */
