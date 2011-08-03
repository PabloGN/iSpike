#ifndef INPUTCHANNELFACTORY_HPP_
#define INPUTCHANNELFACTORY_HPP_

//iSpike includes
#include <iSpike/Reader/Reader.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Channel/InputChannel/InputChannel.hpp>
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>

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
			vector<InputChannelDescription> channelList;

		public:
			InputChannelFactory();
			vector<InputChannelDescription> getAllChannels();
			InputChannel* create(string channelName, Reader* reader, map<string,Property*> channelProperties);
	};

}

#endif /* INPUTCHANNELFACTORY_HPP_ */
