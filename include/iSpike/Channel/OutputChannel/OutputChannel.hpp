#ifndef OUTPUTCHANNEL_H_
#define OUTPUTCHANNEL_H_

//iSpike includes
#include <iSpike/Channel/Channel.hpp>
#include <iSpike/Description.hpp>
#include "iSpike/Writer/Writer.hpp"

//Other includes
#include <vector>
using namespace std;

namespace ispike {

	/** Represents a Channel that can be written to e.g. it is possible
		to send spikes to this channel, which are decoded into something else */
	class OutputChannel : public Channel {
		public:
			virtual ~OutputChannel() {}
			Description getChannelDescription() { return channelDescription; }

			/*! Initializes the channel with a reader and set of properties */
			virtual void initialize(Writer* writer, map<string, Property>& properties) = 0;

			/**  Sets the current spike pattern  */
			virtual void setFiring(vector<int>& buffer) = 0;


		protected:
			/** Description of the channel */
			Description channelDescription;

	};
}

#endif /* OUTPUTCHANNEL_H_ */
