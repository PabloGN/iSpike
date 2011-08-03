#ifndef INPUTCHANNEL_H_
#define INPUTCHANNEL_H_

//iSpike includes
#include <iSpike/Channel/Channel.hpp>
#include <iSpike/Channel/InputChannel/InputChannelDescription.hpp>
#include "iSpike/Reader/AngleReader.hpp"
#include "iSpike/PropertyHolder.hpp"

//Other includes
#include <string>
#include <vector>
using namespace std;

namespace ispike {

	/**
	 * This class represents a directed Channel from a signal source (eg vision)
	 * to a spike output, so spikes can be read from this channel
	 */
	class InputChannel : public Channel {
		protected:
			InputChannelDescription channelDescription;

		public:
			virtual ~InputChannel();
			InputChannelDescription getChannelDescription() { return channelDescription; }

			/*! Returns the latest spikes generated by the channel */
			virtual vector<int>& getFiring() = 0;

			/*! Initializes the channel with a reader and set of properties */
			virtual void initialize(AngleReader* reader, map<string, Property> properties) = 0;
	};

}

#endif /* INPUTCHANNEL_H_ */
