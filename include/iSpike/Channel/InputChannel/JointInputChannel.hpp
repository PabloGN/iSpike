#ifndef JOINTINPUTCHANNEL_HPP_
#define JOINTINPUTCHANNEL_HPP_

//iSpike includes
#include <iSpike/Channel/InputChannel/InputChannel.hpp>
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>

//Other includes
#include <boost/math/distributions/normal.hpp>
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
			vector<int>& getFiring() { return neuronSim.getSpikes(); }
			void initialize(Reader* reader, map<string, Property>& properties);
			void setProperties(map<string, Property>& properties);
			void step();


		private:
			//==========================  VARIABLES  ==========================
			/** Reads angle from data source */
			AngleReader* reader;

			/** Minimum angle of joint */
			double minAngle;

			/** Maximum angle of joint */
			double maxAngle;

			/** Standard deviation */
			double standardDeviation;

			/** Angle at centre of each neuron's perceptual field */
			vector<double> neuronAngles;

			/** Normal distribution */
			boost::math::normal_distribution<double> normalDistribution;

			/*! Izhikevich neuron simulator */
			IzhikevichNeuronSim neuronSim;

			/** Factor by which input current to neurons is multiplied */
			double currentFactor;

			/** Constant current injected into neurons */
			double constantCurrent;

			/** Peak current - sets maximum current into neurons. */
			double peakCurrent;

			//==============================  METHODS  =========================
			void updateProperties(map<string, Property>& properties);

	};

}

#endif /* JOINTINPUTCHANNEL_HPP_ */
