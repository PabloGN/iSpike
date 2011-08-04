#ifndef OUTPUTCHANNELDESCRIPTION_HPP_
#define OUTPUTCHANNELDESCRIPTION_HPP_

#include <string>
using namespace std;

namespace ispike {

	/** Describes an output channel. */
	class OutputChannelDescription {
		public:
			OutputChannelDescription();
			OutputChannelDescription(string channelName, string channelDescription, string writerType);
			OutputChannelDescription & operator= (const OutputChannelDescription & copy_from_me);
			OutputChannelDescription(const OutputChannelDescription& copy_from_me);
			~OutputChannelDescription();

			/** Retrieves the Channel description */
			std::string getChannelDescription() const { return channelDescription; }

			/** Retrives the Channel name */
			std::string getChannelName() const { return channelName; }

			/** Retrieves the type of Writer this Channel accepts */
			std::string getWriterType() const { return writerType; }

		private:
			/**  Name of the Output Channel */
			string channelName;

			/** A description of the channel */
			string channelDescription;

			/** The type of writer this channel accepts */
			string writerType;

	};

}

#endif /* OUTPUTCHANNELDESCRIPTION_HPP_ */
