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
			Reader* create(Description& desc, map<string, Property>& readerProperties);
			map<string, Property> getDefaultProperties(Description& desc);
			vector<Description> getReadersOfType(string readerType);


		private:
			//==============================  VARIABLES  =============================
			/// A list of available readers
			vector<Description> readerList;
			string ip;
			unsigned port;

			//=============================  METHODS  ================================
			void printReaders();

	};

}

#endif /* READERFACTORY_HPP_ */
