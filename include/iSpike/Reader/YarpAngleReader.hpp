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
	class YarpAngleReader : public AngleReader {
		public:
			YarpAngleReader(string nameserverIP, unsigned nameserverPort);
			virtual ~YarpAngleReader();
			std::vector<double> getData();
			void initialise(map<string, Property>& properties);
			std::string getPortName(){ return this->portName; }
			void setPortName(std::string portName){ this->portName = portName;}
			void start();

		private:
			//=========================  VARIABLES  ===========================
			vector<double> buffer;
			string portName;
			YarpConnection* yarpConnection;
			map<string, YarpPortDetails> portMap;

			//============================  METHODS  ===========================
			void workerFunction();

	};

}
#endif /* YARPANGLEREADER_HPP_ */
