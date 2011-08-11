#ifndef ANGLEREADER_HPP_
#define ANGLEREADER_HPP_

//iSpike includes
#include <iSpike/Reader/Reader.hpp>

namespace ispike {
	/** AngleReader continuously fetches and retrieves joint angles upon request */
	class AngleReader : public Reader {
		public:
			AngleReader() { angle = 0.0; }
			virtual ~AngleReader(){}

			/** Retrieves the angle  */
			double getAngle() { return angle; }

			/** Sets the degree of freedom */
			void setDegreeOfFreedom(int dof){ degreeOfFreedom = dof; }

			/** Sets the angle */
			void setAngle(double angle) { this->angle = angle; }


		protected:
			/** The angle that is read by the reader */
			double angle;

			/** Degree of freedom of the joint */
			int degreeOfFreedom;

	};

}

#endif /* ANGLEREADER_HPP_ */
