#ifndef ANGLEREADER_HPP_
#define ANGLEREADER_HPP_

#include <iSpike/Reader/Reader.hpp>
#include <vector>
#include <string>
using namespace std;

namespace ispike {
	/** AngleReader continuously fetches and retrieves joint angles upon request */
	class AngleReader : public Reader {
		public:
			virtual ~AngleReader() { angle = 0.0; }

			/** Retrieves the angle  */
			double getAngle() { return angle; }

			/** Sets the angle */
			void setAngle(double angle) { this->angle = angle; }

		private:
			/** The angle that is read by the reader */
			double angle;

	};

}

#endif /* ANGLEREADER_HPP_ */
