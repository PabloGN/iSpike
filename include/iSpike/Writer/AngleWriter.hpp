#ifndef ANGLEWRITER_HPP_
#define ANGLEWRITER_HPP_

#include <iSpike/Writer/Writer.hpp>

namespace ispike {

	/** Angle Writer, receives an angle as input and writes it to the designated output */
	class AngleWriter : public Writer{
		public:
			AngleWriter(){ angle = 0.0; }
			virtual ~AngleWriter(){}

			/** Sets the angle to be output */
			virtual void setAngle(double angle) = 0;

			/** Retrieves the angle  */
			double getAngle() { return angle; }

			/** Sets the degree of freedom */
			virtual void setDegreeOfFreedom(int dof) = 0;


		protected:
			/** The angle that is written by the writer */
			double angle;

			/** Degree of freedom of the joint */
			int degreeOfFreedom;
	};

}

#endif /* ANGLEWRITER_HPP_ */
