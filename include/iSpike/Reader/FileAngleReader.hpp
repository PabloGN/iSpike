#ifndef FILEANGLEREADER_HPP_
#define FILEANGLEREADER_HPP_

#include <iSpike/Reader/AngleReader.hpp>
#include <string>
#include <vector>

namespace ispike {

	/** Retrieves a vector of joint angles from a given file and makes it available upon request */
	class FileAngleReader : public AngleReader {

		private:
			//==========================  VARIABLES  =========================
			vector<double> buffer;
			bool initialised;
			string fileName;

			//===========================  METHODS  ==========================
			void FileAngleReader::readAngleFromFile(const char* fileName);


		public:
			FileAngleReader();
			std::vector<double> getData() { return buffer; }
			void initialise(map<string, Property>& properties);
			void start();
	};

}

#endif /* FILEANGLEREADER_HPP_ */
