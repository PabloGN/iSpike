#ifndef OUTPUTCHANNEL_H_
#define OUTPUTCHANNEL_H_

//iSpike includes
#include <iSpike/Channel/Channel.hpp>
#include <iSpike/Channel/OutputChannel/OutputChannelDescription.hpp>

//Other includes
#include <boost/scoped_ptr.hpp>
#include <string>
#include <vector>

namespace ispike {

	/** Represents a Channel that can be written to e.g. it is possible
		to send spikes to this channel, which are decoded into something else */
	class OutputChannel : public Channel {
		public:
			OutputChannel()  : Channel() {}
			OutputChannelDescription getChannelDescription() { return *(channelDescription); }

			/**  Sets the current spike pattern  */
			virtual void setFiring(std::vector<int>& buffer) = 0;


		protected:
			boost::scoped_ptr<OutputChannelDescription> channelDescription;

	};
}

#endif /* OUTPUTCHANNEL_H_ */
