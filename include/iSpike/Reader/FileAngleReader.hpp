#ifndef FILEANGLEREADER_HPP_
#define FILEANGLEREADER_HPP_

#include <iSpike/Reader/AngleReader.hpp>
#include <string>
#include <map>

using namespace std;

namespace ispike {

	/** Retrieves a joint angle from a given file and makes it available upon request */
	class FileAngleReader : public AngleReader {

		public:

			/** Create a new file angle reader which reads from a file named anglesIn.txt */
			FileAngleReader();

			~FileAngleReader();

			void initialize(map<string, Property>& properties);
			void setProperties(map<string, Property>& properties);

			void start() {}//No need for thread since file only needs to be read once

		private:
			void workerFunction() {}
			void readAngleFromFile(string& fileName);
	};

}

#endif /* FILEANGLEREADER_HPP_ */
