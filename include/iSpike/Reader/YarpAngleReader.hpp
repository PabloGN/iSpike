#ifndef YARPANGLEREADER_HPP_
#define YARPANGLEREADER_HPP_

#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/YarpConnection.hpp>

#include <vector>
#include <map>
using namespace std;

namespace ispike {

	/** Retrieves a vector of joint angles from a given yarp port and makes it available upon request */
	class YarpAngleReader : public AngleReader {
		public:
			YarpAngleReader(string nameserverIP, unsigned nameserverPort);
			~YarpAngleReader();
			void initialize(map<string, Property>& properties);
			void setProperties(map<string, Property>& properties);
			void start();

		private:
			//=========================  VARIABLES  ===========================
			/** Class handling connection to YARP */
			YarpConnection* yarpConnection;

			/** Name of port that we connect to */
			string portName;

			/** Sleep duration in between reads */
			int sleepDuration_ms;


			//============================  METHODS  ===========================
			void updateProperties(map<string, Property>& properties);
			void workerFunction();

	};

}
#endif /* YARPANGLEREADER_HPP_ */
