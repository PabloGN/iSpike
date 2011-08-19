#ifndef LOGPOLARVISUALDATAREDUCER_HPP_
#define LOGPOLARVISUALDATAREDUCER_HPP_

#include <iSpike/Bitmap.hpp>

#include <vector>
using namespace std;

#include <boost/scoped_ptr.hpp>

namespace ispike {

	/** Holds a mapping between polar and Cartesian coordinates */
	class PolarCartCoords {
		public:
			PolarCartCoords(int radius, int theta, int x, int y) :
				radius(radius),
				theta(theta),
				x(x),
				y(y)
			{
				;
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
			Bitmap& getReducedImage();
			bool isInitialized() { return initialized; }
			void setBitmap(Bitmap& bitmap);
			void setOutputHeight(int outputHeight);
			void setOutputWidth(int outputWidth);
			void setFoveaRadius(double foveaRadius);

		private:
			//========================  VARIABLES  ============================
			/** Log polar image */
			boost::scoped_ptr<Bitmap> reducedImage;

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

	};

}

#endif /* LOGPOLARVISUALDATAREDUCER_HPP_ */
