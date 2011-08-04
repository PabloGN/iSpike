#ifndef JOINTINPUTCHANNEL_HPP_
#define JOINTINPUTCHANNEL_HPP_

//iSpike includes
#include <iSpike/Channel/InputChannel/InputChannel.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Channel/InputChannel/InputChannelDescription.hpp>
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>
#include "iSpike/iSpikeThread.hpp"

//Other includes
#include <string>
#include <vector>
#include <map>
using namespace std;

namespace ispike {

	/** Converts a joint angle input into a spike pattern */
	class JointInputChannel : public InputChannel {
		public:
			JointInputChannel();
			virtual ~JointInputChannel();
			double getCurrentAngle(){ return currentAngle; }
			vector<int>& getFiring() { return neuronSim.getSpikes(); }
			void initialize(Reader* reader, map<string, Property>& properties);
			void setProperties(map<string, Property>& properties);
			void step();


		private:
			//==========================  VARIABLES  ==========================
			/** Reads angle from data source */
			AngleReader* reader;

			double currentAngle;
			double sd;
			double minAngle;
			double maxAngle;

			/*! Izhikevich neuron simulator */
			IzhikevichNeuronSim neuronSim;

			/** Factor by which input current to neurons is multiplied */
			double currentFactor;

			/** Constant current injected into neurons */
			double constantCurrent;


			//==============================  METHODS  =========================
			void updateProperties(map<string, Property>& properties);

	};

}

#endif /* JOINTINPUTCHANNEL_HPP_ */
