#ifndef YARPANGLEWRITER_HPP_
#define YARPANGLEWRITER_HPP_

#include <queue>
#include <string>
#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/Writer/WriterDescription.hpp>
#include <map>
#include <iSpike/YarpPortDetails.hpp>

namespace ispike {
	class YarpAngleWriter : public AngleWriter {
		public:
			YarpAngleWriter(string nameserverIP, unsigned nameserverPort);
			virtual ~YarpAngleWriter();
			void setAngle(double angle);
			void initialise(map<string,Property> properties);
			void start();

		private:
			//===========================  VARIABLES  =========================
			double angle;
			string portName;
			int degreeOfFreedom;
			YarpConnection* yarpConnection;
			map<string, YarpPortDetails> portMap;

			/*! Controls whether the angle should be output */
			bool angleChanged;

			//===========================  METHODS  ===========================
			void workerFunction();
	};

}

#endif /* YARPANGLEWRITER_HPP_ */
