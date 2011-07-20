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
			virtual ~AngleReader() {}

			/** Retrieves the angles  */
			virtual std::vector<double> getData() = 0;

			/** Initialises the reader and starts the main thread  */
			virtual void start() = 0;

	};

}

#endif /* ANGLEREADER_HPP_ */
