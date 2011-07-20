#ifndef ANGLEWRITER_HPP_
#define ANGLEWRITER_HPP_

#include <iSpike/Writer/Writer.hpp>

namespace ispike {

	/** Angle Writer, receives an angle as input and writes it to the designated output */
	class AngleWriter : public Writer {
		public:
			/** Sets the angle to be output */
			virtual void setAngle(double angle) = 0;

			/** Initialises the writer and starts the main thread  */
			virtual void start() = 0;

	};

}

#endif /* ANGLEWRITER_HPP_ */
