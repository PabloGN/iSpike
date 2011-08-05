#ifndef COMMON_HPP_
#define COMMON_HPP_

//iSpike includes
#include <iSpike/Bitmap.hpp>

//Other includes
#include <vector>
using namespace std;

namespace ispike {

	/** Static common functionality */
	class Common {
		public:
			//========================  METHODS  =========================
			static void savePPMImage(const char* filename, Bitmap* image);
			static void writePatternToFile(const char* fileName, vector<int> pattern, int numOfNeurons);


			//========================  VARIABLES  =======================
			static const int redVsGreen = 0;
			static const int greenVsRed = 1;
			static const int blueVsYellow = 2;
	};

}

#endif /* COMMON_HPP_ */
