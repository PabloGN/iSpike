#ifndef INPUTCHANNELDESCRIPTION_HPP_
#define INPUTCHANNELDESCRIPTION_HPP_

//Other includes
#include <string>
using namespace std;

namespace ispike {

	/** This class describes an input channel. */
	class InputChannelDescription {
		public:
			InputChannelDescription(string channelName, string channelDescription, string readerType);
			InputChannelDescription(const InputChannelDescription& copy_from_me);
			~InputChannelDescription();
			InputChannelDescription & operator= (const InputChannelDescription & copy_from_me);

			/** Retrieves the Channel description */
			string getChannelDescription() const { return channelDescription;	}

			/**  Retrives the Channel name	*/
			string getChannelName() const { return channelName; }

			/** Retrieves the type of Reader this Channel accepts */
			string getReaderType() const { return readerType;	}


		private:
			/// Name of the Input Channel
			string channelName;

			/// A description of the channel
			string channelDescription;

			/// The type of reader this channel accepts
			string readerType;

	};

}

#endif /* INPUTCHANNELDESCRIPTION_HPP_ */
