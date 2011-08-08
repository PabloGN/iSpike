#ifndef JOINTOUTPUTCHANNEL_HPP_
#define JOINTOUTPUTCHANNEL_HPP_

//iSpike includes
#include <iSpike/Channel/OutputChannel/OutputChannel.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Log/Log.hpp>

//Other includes
#include <string>
#include <vector>
using namespace std;

namespace ispike {

	/** Encodes joint angles into spikes */
	class JointOutputChannel : public OutputChannel {
		public:
			JointOutputChannel();
			virtual ~JointOutputChannel();
			void initialize(Writer* writer, map<string,Property>& properties);
			void setFiring(vector<int>& buffer);
			void setProperties(map<string, Property>& properties);
			void step();


		protected:
			void updateProperties(map<string, Property>& properties);


		private:
			/** Outputs angle to YARP, file, etc. */
			AngleWriter* writer;

			/** The minimum value of the angle */
			double minAngle;

			/** The maximum value of the angle */
			double maxAngle;

			/** The rate at which the current variables decay */
			double rateOfDecay;

			/** Vector holding the current variables */
			vector<double> currentVariables;

			/** Angle associated with each current variable */
			vector<double> currentVariableAngles;

			/** The amount by which a current variable is incremented with each spike */
			double currentIncrement;
	};

}

#endif /* JOINTOUTPUTCHANNEL_HPP_ */
