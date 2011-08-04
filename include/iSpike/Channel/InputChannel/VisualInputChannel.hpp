#ifndef VISUALINPUTCHANNEL_HPP_
#define VISUALINPUTCHANNEL_HPP_

//iSpike includes
#include <iSpike/Channel/InputChannel/InputChannel.hpp>
#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/VisualDataReducer/LogpolarVisualDataReducer.hpp>
#include <iSpike/VisualFilter/DOGVisualFilter.hpp>
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iSpike/Property.hpp>

//Other includes
#include <string>
#include <vector>
using namespace std;

namespace ispike {

	/** Converts an image input into a spike pattern */
	class VisualInputChannel : public InputChannel {
		public:
			VisualInputChannel();
			virtual ~VisualInputChannel();
			vector<int>& getFiring() { return neuronSim.getSpikes(); }
			void initialize(Reader* reader, map<string, Property>& properties);
			void setProperties(map<string, Property>& properties);
			void step();


		protected:
			void updateProperties(map<string, Property>& properties);


		private:
			/** Extracts visual data from YARP or file */
			VisualReader* reader;

			/** Foveates the image using log polar */
			LogPolarVisualDataReducer* dataReducer;

			/** Uses difference of Gaussians to model colour opponency */
			DOGVisualFilter* dogFilter;

			/** Neural simulator to convert currents to spikes. */
			IzhikevichNeuronSim neuronSim;

			/** Factor by which incoming spikes are multiplied */
			double currentFactor;

			/** Constant current fed into the neurons */
			double constantCurrent;

			/** ID of the current image - used to check for changes to the image provided by the reader */
			unsigned currentImageID;

	};
}

#endif /* VISUALINPUTCHANNEL_HPP_ */
