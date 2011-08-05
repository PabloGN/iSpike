#ifndef WRITERFACTORY_HPP_
#define WRITERFACTORY_HPP_

//iSpike includes
#include "iSpike/Writer/Writer.hpp"
#include <iSpike/Description.hpp>

//Other includes
#include <vector>
#include <string>
using namespace std;

namespace ispike {

	/** List all Writers available in the system and creates a particular type of a Writer */
	class WriterFactory {
		private:
			/** A list of available writers */
			vector<Description> writerList;
			string ip;
			unsigned port;

			//=========================  METHODS  ===========================
			void printWriters();

		public:
			WriterFactory();
			WriterFactory(std::string ip, unsigned port);
			Writer* create(Description& desc, map<string, Property>& writerProperties );
			map<string, Property> getDefaultProperties(Description& desc);
			vector<Description> getWritersOfType(string writerType);

	};
}


#endif /* WRITERFACTORY_HPP_ */
