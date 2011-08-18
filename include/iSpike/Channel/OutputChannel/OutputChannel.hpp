#ifndef OUTPUTCHANNEL_H_
#define OUTPUTCHANNEL_H_

#include <iSpike/Channel/Channel.hpp>
#include <iSpike/Description.hpp>
#include <iSpike/Writer/Writer.hpp>

namespace ispike {

	/** Represents a Channel that can be written to e.g. it is possible
		to send spikes to this channel, which are decoded into something else */
	class OutputChannel : public Channel {
		public:

			virtual ~OutputChannel() {}
			Description getChannelDescription() { return channelDescription; }

			/*! Initialize the channel with a reader and set of properties
			 *
			 * The Channel assumes ownership of the Writer object.
			 * */
			virtual void initialize(Writer* writer, map<string, Property>& properties) = 0;

			/**  Sets the current spike pattern  */
			virtual void setFiring(const std::vector<unsigned>& buffer) = 0;

		protected:

			/** Description of the channel */
			Description channelDescription;

	};
}

#endif /* OUTPUTCHANNEL_H_ */
