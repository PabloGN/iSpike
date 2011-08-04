#ifndef FILEANGLEWRITER_HPP_
#define FILEANGLEWRITER_HPP_

#include <iSpike/Writer/AngleWriter.hpp>

namespace ispike {

	/** Writes a joint angle to a given file  */
	class FileAngleWriter : public AngleWriter {
		public:
			FileAngleWriter();
			virtual ~FileAngleWriter(){}
			void initialize(map<string, Property>& properties);
			void setAngle(double angle);
			void setProperties(map<string, Property>& properties);
			void start() {}

		private:
			//==========================  VARIABLES  =========================
			/** Used to establish if angle has changed */
			double currentAngle;

			/** Name of the file to write the angle */
			string fileName;

			//===========================  METHODS  ==========================
			void writeAngleToFile();
			void workerFunction() {}

	};

}
#endif /* FILEANGLEWRITER_HPP_ */
