#ifndef MOCKANGLEWRITER_HPP_
#define MOCKANGLEWRITER_HPP_

#include <iSpike/Writer/AngleWriter.hpp>

namespace ispike {

	/** Class for testing that receives an angle but does nothing with it  */
	class MockAngleWriter : public AngleWriter {
		public:
			MockAngleWriter(){}
			virtual ~MockAngleWriter(){}
			void initialize(map<string, Property>& properties){}
			void setAngle(double angle){}
			void setDegreeOfFreedom(int dof){}
			void setProperties(map<string, Property>& properties){}
			void start() {}
			void workerFunction(){}
	};

}
#endif /* MOCKANGLEWRITER_HPP_ */
