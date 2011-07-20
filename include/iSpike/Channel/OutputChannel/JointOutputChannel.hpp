#ifndef JOINTOUTPUTCHANNEL_HPP_
#define JOINTOUTPUTCHANNEL_HPP_

#include <iSpike/Channel/OutputChannel/OutputChannel.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <queue>
#include <string>
#include <vector>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Log/Log.hpp>

namespace ispike {

	/** Encodes joint angles into spikes */
	class JointOutputChannel : public OutputChannel {
		public:
			JointOutputChannel();
			~JointOutputChannel();
			void initialise(AngleWriter* writer, map<string,Property>& properties);
			void setFiring(std::vector<int>& buffer);
			void setProperties(map<string, Property>& properties);
			void step();


		protected:
			void updateProperties(std::map<std::string,Property*> properties, bool updateReadOnly);


		private:
			AngleWriter* writer;
			double minAngle;
			double maxAngle;
			double rateOfDecay;
			double currentAngle;

			/** Array holding the current variables */
			double* variables;

			/*! Number of current variables */
			int numVariables;

			/** The amount by which a current variable is incremented with each spike */
			double currentIncrement;

			/** Map holding new properties, for updating when thread has finished processing the current time step*/
			map<string, Property> newPropertyMap;

			/** Flag to indicate that properties should be updated */
			bool copyProperties;



	};

}

#endif /* JOINTOUTPUTCHANNEL_HPP_ */
