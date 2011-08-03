#ifndef READERDESCRIPTION_HPP_
#define READERDESCRIPTION_HPP_

//iSpike includes
#include <iSpike/Property.hpp>
#include <iSpike/Log/Log.hpp>

//Other includes
#include <string>
#include <map>
using namespace std;

namespace ispike {

	/** Describes a Reader */
	class ReaderDescription {
		public:
			ReaderDescription();
			ReaderDescription(string readerName, string readerDescription,	std::string readerType);
			ReaderDescription(const ReaderDescription& desc);
			ReaderDescription& operator=(const ReaderDescription& rhs);

			/** Returns the Reader's description */
			std::string getDescription() const { return description;}

			/** Returns the name of the Reader */
			std::string getName() const {	return name; }

			/** Returns the Reader's type */
			std::string getType() const {	return type; }


		private:
			///Name of the Reader
			string name;

			///Description of the Reader
			string description;

			/// Type of the Reader
			string type;

	};

}


#endif /* READERDESCRIPTION_HPP_ */
