#ifndef YARPANGLEREADER_HPP_
#define YARPANGLEREADER_HPP_

//iSpike includes
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include "iSpike/iSpikeThread.hpp"

//Other includes

#include <vector>
#include <map>
using namespace std;

namespace ispike {

	/** Retrieves a vector of joint angles from a given yarp port and makes it available upon request */
	class YarpAngleReader : public AngleReader, public iSpikeThread {
		public:
			YarpAngleReader(string nameserverIP, unsigned nameserverPort);
			virtual ~YarpAngleReader();
			void initialise(map<string, Property>& properties);
			void start();

		private:
			//=========================  VARIABLES  ===========================
			YarpConnection* yarpConnection;

			/** Map of the ports that are availabe in YARP */
			map<string, YarpPortDetails> portMap;

			/** Degree of freedom controlling which angle is stored and returned */
			unsigned degreeOfFreedom;


			//============================  METHODS  ===========================
			void updateProperties(map<string, Property>& properties, bool updateReadOnly);
			void workerFunction();

	};

}
#endif /* YARPANGLEREADER_HPP_ */
