#ifndef WRITERFACTORY_HPP_
#define WRITERFACTORY_HPP_

//iSpike includes
#include "iSpike/Writer/Writer.hpp"
#include <iSpike/Writer/WriterDescription.hpp>
#include <iSpike/Property.hpp>

//Other includes
#include <vector>
#include <string>
using namespace std;

namespace ispike {

	/** List all Writers available in the system and creates a particular type of a Writer */
	class WriterFactory {
		private:
			/** A list of available writers */
			vector<WriterDescription> writerList;
			string ip;
			unsigned port;

		public:
			WriterFactory();
			WriterFactory(std::string ip, unsigned port);
			vector<WriterDescription> getWritersOfType(string writerType);
			Writer* create(string writerName, map<string, Property>& writerProperties );

	};
}


#endif /* WRITERFACTORY_HPP_ */
