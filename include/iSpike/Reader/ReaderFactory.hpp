#ifndef READERFACTORY_HPP_
#define READERFACTORY_HPP_

//iSpike includes
#include "iSpike/Reader/Reader.hpp"
#include <iSpike/Description.hpp>

//Other includes
#include <string>
#include <vector>
using namespace std;

namespace ispike {

	/** Lists all Readers available in the system and can produce a particular type of a Reader 	 */
	class ReaderFactory {
		public:
			ReaderFactory();
			ReaderFactory(string ip, unsigned port);
			vector<Description> getReadersOfType(string readerType);
			Reader* create(string readerName, map<string, Property>& readerProperties);


		private:
			/// A list of available readers
			vector<Description> readerList;
			string ip;
			unsigned port;

	};

}

#endif /* READERFACTORY_HPP_ */
