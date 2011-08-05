#ifndef INPUTCHANNELFACTORY_HPP_
#define INPUTCHANNELFACTORY_HPP_

//iSpike includes
#include <iSpike/Reader/Reader.hpp>
#include <iSpike/Channel/InputChannel/InputChannel.hpp>

//Other includes
#include <string>
using namespace std;

namespace ispike {

	/**
	 * @class InputChannelFactory
	 * @brief A Factory of Input Channels
	 *
	 * The Input Channel Factory can list all Input Channels available in the system and can produce a particular type of an Input Channel  */
	class InputChannelFactory {
		private:
			/** A list of available Input Channels */
			vector<Description> channelList;

			//============================  METHODS  =======================
			void printInputChannels();


		public:
			InputChannelFactory();
			vector<Description> getAllChannels();
			InputChannel* create(Description& desc, Reader* reader, map<string, Property>& channelProperties);
			map<string, Property> getDefaultProperties(Description& desc);
	};

}

#endif /* INPUTCHANNELFACTORY_HPP_ */
