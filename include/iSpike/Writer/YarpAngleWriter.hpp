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
			void initialize(map<string,Property>& properties);
			void start();

		private:
			//===========================  VARIABLES  =========================
			YarpConnection* yarpConnection;

			/** Holds the output ports that are available */
			map<string, YarpPortDetails> portMap;

			/*! Controls whether the angle should be output */
			bool angleChanged;

			/** The port to use */
			string portName;

			/** Amount to sleep in between writing commands */
			unsigned sleepDuration_ms;


			//===========================  METHODS  ===========================
			void updateProperties(map<string, Property>& properties);
			void workerFunction();
	};

}

#endif /* YARPANGLEWRITER_HPP_ */
