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
			vector< vector<int> >& getFiring() { return buffer; }
			void initialize(VisualReader* reader, map<string, Property> properties);
			void setProperties(map<string, Property>& properties);
			void step();


		protected:
			void updateProperties(map<string, Property>& properties, bool updateReadOnly);


		private:
			vector< vector<int> > buffer;
			VisualReader* reader;
			void workerFunction();
			bool stopRequested;
			VisualDataReducer* dataReducer;
			VisualFilter* dogFilter;
			IzhikevichNeuronSim* neuronSim;
			double parameterA;
			double parameterB;
			double parameterC;
			double parameterD;
			double currentFactor;
			double constantCurrent;
			double plusSigma;
			double minusSigma;
			double ratio1;
			double ratio2;
			int xOffset;
			int yOffset;
			int opponentMapID;

			/** Radius of the central foveated area */
			double foveaRadius;

			/** Map holding new properties, for updating when thread has finished processing the current time step*/
			map<string, Property> newPropertyMap;

			/** Flag to indicate that properties should be updated */
			bool copyProperties;

			/** ID of the current image - used to check for changes to the image provided by the reader */
			unsigned currentImageID;

	};
}

#endif /* VISUALINPUTCHANNEL_HPP_ */
