#ifndef LOGPOLARVISUALDATAREDUCER_HPP_
#define LOGPOLARVISUALDATAREDUCER_HPP_

//iSpike includes
#include <iSpike/Bitmap.hpp>

//Other includes
#include <vector>
using namespace std;

namespace ispike {

	/** Holds a mapping between polar and Cartesian coordinates */
	class PolarCartCoords {
		public:
			PolarCartCoords(int radius, int theta, int x, int y){
				this->radius = radius;
				this->theta = theta;
				this->x = x;
				this->y = y;
			}
			int radius;
			int theta;
			int x;
			int y;
	};

	/** Represents a Log Polar Visual Data Reducer capable of transforming an image
	 * from cartesian to logpolar coordinates */
	class LogPolarVisualDataReducer {
		public:
			LogPolarVisualDataReducer();
			~LogPolarVisualDataReducer();
			Bitmap& getReducedImage();
			bool isInitialized() { return initialized; }
			void setBitmap(Bitmap& bitmap);
			void setOutputHeight(int outputHeight);
			void setOutputWidth(int outputWidth);
			void setFoveaRadius(double foveaRadius);


		private:
			//========================  VARIABLES  ============================
			/** Log polar image */
			Bitmap* reducedImage;

			/** Width of the incoming image */
			int inputWidth;

			/** Height of the incoming image */
			int inputHeight;

			/** Width of the reduced image to be output */
			int outputWidth;

			/** Height of the reduced image to be output */
			int outputHeight;

			/** Map linking a set of Cartesian coordinates in the input image to a set of polar coordinates in the output image */
			//CoordMapType polarToCartesianMap;
			vector<PolarCartCoords> coordinatesVector;


			/** Radius of the foveated area */
			double foveaRadius;

			/** Records whether the maps for converting cartesian to polar coordinates have been initialized */
			bool initialized;


			//========================  METHODS  ==============================
			void calculateReducedImage(Bitmap& bitmap);
			pair<int, int> getInputCartesianCoordinate(double radius, double theta);
			void initialize(Bitmap& bitmap);
			void initialisePolarToCartesianVector();
			void setInitialized(bool initialized){ this->initialized = initialized; }

	};

}

#endif /* LOGPOLARVISUALDATAREDUCER_HPP_ */
