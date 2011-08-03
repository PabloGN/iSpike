#ifndef JOINTOUTPUTCHANNEL_HPP_
#define JOINTOUTPUTCHANNEL_HPP_

//iSpike includes
#include <iSpike/Channel/OutputChannel/OutputChannel.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Log/Log.hpp>

//Other includes
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <queue>
#include <string>
#include <vector>

namespace ispike {

	/** Encodes joint angles into spikes */
	class JointOutputChannel : public OutputChannel {
		public:
			JointOutputChannel();
			virtual ~JointOutputChannel();
			void initialise(AngleWriter* writer, map<string,Property>& properties);
			void setFiring(vector<int>& buffer);
			void setProperties(map<string, Property>& properties);
			void step();


		protected:
			void updateProperties(map<string,Property*> properties, bool updateReadOnly);


		private:
			/** Outputs angle to YARP, file, etc. */
			AngleWriter* writer;

			/** The minimum value of the angle */
			double minAngle;

			/** The maximum value of the angle */
			double maxAngle;

			/** The rate at which the current variables decay */
			double rateOfDecay;

			/** Array holding the current variables */
			double* currentVariables;

			/** The amount by which a current variable is incremented with each spike */
			double currentIncrement;
	};

}

#endif /* JOINTOUTPUTCHANNEL_HPP_ */
