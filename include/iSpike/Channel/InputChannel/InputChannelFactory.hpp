#ifndef INPUTCHANNELFACTORY_HPP_
#define INPUTCHANNELFACTORY_HPP_

#include <string>
#include <iSpike/Reader/Reader.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Channel/InputChannel/InputChannel.hpp>
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>

namespace ispike {

	/**
	 * @class InputChannelFactory
	 * @brief A Factory of Input Channels
	 *
	 * The Input Channel Factory can list all Input Channels available in the system and can produce a particular type of an Input Channel  */
	class InputChannelFactory {
		private:
			/** A list of available Input Channels */
			std::vector<InputChannelDescription> channelList;

		public:
			InputChannelFactory();
			std::vector<InputChannelDescription> getAllChannels();
			InputChannel* create(std::string channelName, Reader* reader, std::map<std::string,Property*> channelProperties);
	};

}

#endif /* INPUTCHANNELFACTORY_HPP_ */
