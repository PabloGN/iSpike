#ifndef READERFACTORY_HPP_
#define READERFACTORY_HPP_

//iSpike includes
#include <iSpike/Reader/ReaderDescription.hpp>

#include <string>
#include <vector>
using namespace std;

namespace ispike {
	/** Lists all Readers available in the system and can produce a particular type of a Reader
	 */
	class ReaderFactory {
		public:
			ReaderFactory();
			ReaderFactory(string ip, string port);
			vector<ReaderDescription> getReadersOfType(std::string readerType);
			Reader* create(string readerName, map<string, Property>& readerProperties );


		private:
			/// A list of available readers
			vector<ReaderDescription> readerList;
			string ip;
			unsigned port;

	};

}

#endif /* READERFACTORY_HPP_ */
