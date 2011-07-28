#ifndef LOGPOLARVISUALDATAREDUCER_HPP_
#define LOGPOLARVISUALDATAREDUCER_HPP_

//iSpike includes
#include <iSpike/VisualDataReducer/VisualDataReducer.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Reader/VisualReader.hpp>

//Other includes
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <map>
using namespace std;

namespace ispike {

	/** Maps between a pair of coordinates in one coordinate system to a pair of coordinates in another coordinate system */
	typedef map< pair<int, int>, pair<int, int> > CoordMapType;

	/** Represents a Log Polar Visual Data Reducer capable of transforming an image
	 * from cartesian to logpolar coordinates */
	class LogPolarVisualDataReducer : public VisualDataReducer{
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
			CoordMapType* polarToCartesianMap;

			/** Radius of the foveated area */
			double foveaRadius;

			/** Records whether the maps for converting cartesian to polar coordinates have been initialized */
			bool initialized;


			//========================  METHODS  ==============================
			void calculateReducedImage(Bitmap& bitmap);
			pair<int, int> getInputCartesianCoordinate(double radius, double theta);
			void initialize(Bitmap& bitmap);
			void initialisePolarToCartesianMap();
	};

}

#endif /* LOGPOLARVISUALDATAREDUCER_HPP_ */
