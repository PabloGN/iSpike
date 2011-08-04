#ifndef WRITERDESCRIPTION_HPP_
#define WRITERDESCRIPTION_HPP_

//iSpike includes
#include <iSpike/Property.hpp>
#include <iSpike/Log/Log.hpp>

//Other includes
#include <string>
#include <map>
using namespace std;

namespace ispike {

	/** Describes a Writer */
	class WriterDescription {
		public:
			WriterDescription();
			WriterDescription(string name, string description, string type);
			WriterDescription(const WriterDescription& desc);
			WriterDescription& operator=(const WriterDescription& rhs);

			/** Returns the Writer's description */
			string getDescription() const { return description;}

			/** Returns the name of the Writer */
			string getName() const { return name; }

			/** Returns the Writer's type */
			string getType() const { return type; }


		private:
			// Name of the Writer
			string name;

			// Description of the Writer
			string description;

			// Type of the Writer
			string type;

	};

}


#endif /* WRITERDESCRIPTION_HPP_ */
