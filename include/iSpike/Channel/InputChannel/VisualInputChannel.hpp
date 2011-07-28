#ifndef VISUALINPUTCHANNEL_HPP_
#define VISUALINPUTCHANNEL_HPP_

//iSpike includes
#include <iSpike/Channel/InputChannel/InputChannel.hpp>
#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/VisualDataReducer/VisualDataReducer.hpp>
#include <iSpike/VisualFilter/VisualFilter.hpp>
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/VisualFilter/MovementFilter.hpp>

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
			vector<int>& getFiring() { return spikeBuffer; }
			void initialize(VisualReader* reader, map<string, Property> properties);
			void setProperties(map<string, Property>& properties);
			void step();


		protected:
			void updateProperties(map<string, Property>& properties, bool updateReadOnly);


		private:
			/** Holds the spikes that fired in the last time step */
			vector<int> spikeBuffer;

			/** Extracts visual data from YARP or file */
			VisualReader* reader;

			/** Foveates the image using log polar */
			VisualDataReducer* dataReducer;

			/** Uses difference of Gaussians to model colour opponency */
			VisualFilter* dogFilter;

			/** Neural simulator to convert currents to spikes. */
			IzhikevichNeuronSim* neuronSim;

			/*double parameterA;
			double parameterB;
			double parameterC;
			double parameterD;
			double currentFactor;
			double constantCurrent;
			double positiveSigma;
			double negativeSigma;
			double positiveFactor;
			double negativeFactor;
			int opponentMapID;

			/ Radius of the central foveated area
			double foveaRadius;
*/
			/** ID of the current image - used to check for changes to the image provided by the reader */
			unsigned currentImageID;

	};
}

#endif /* VISUALINPUTCHANNEL_HPP_ */
