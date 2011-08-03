#ifndef FILEANGLEWRITER_HPP_
#define FILEANGLEWRITER_HPP_

#include <iSpike/Reader/AngleWriter.hpp>
#include <string>
#include <vector>

namespace ispike {

	/** Writes a joint angle to a given file  */
	class FileAngleWriter : public AngleWriter {
		public:
			FileAngleWriter();
			virtual FileAngleWriter(){}
			void initialise(map<string, Property>& properties);
			void setProperties(map<string, Property>& properties);
			void start() {}

		private:
			//===========================  METHODS  ==========================
			void writeAngleToFile(string& fileName);
			void workerFunction() {}


	};

}
#endif /* FILEANGLEWRITER_HPP_ */
