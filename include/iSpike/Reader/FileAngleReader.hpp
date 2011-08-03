#ifndef FILEANGLEREADER_HPP_
#define FILEANGLEREADER_HPP_

#include <iSpike/Reader/AngleReader.hpp>
#include <string>
#include <vector>

namespace ispike {

	/** Retrieves a joint angle from a given file and makes it available upon request */
	class FileAngleReader : public AngleReader {
		public:
			FileAngleReader();
			void initialise(map<string, Property>& properties);
			void setProperties(map<string, Property>& properties);
			void start() {}

		private:
			//===========================  METHODS  ==========================
			void readAngleFromFile(string& fileName);
			void workerFunction() {}


	};

}

#endif /* FILEANGLEREADER_HPP_ */
