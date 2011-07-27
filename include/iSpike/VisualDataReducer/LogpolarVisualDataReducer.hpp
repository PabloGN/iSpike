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
			LogPolarVisualDataReducer(int outputWidth, int outputHeight, int foveaSize);
			~LogPolarVisualDataReducer();
			Bitmap& getReducedImage();
			void setBitmap(Bitmap& bitmap);
			void setProperties(map<string, Property>& propertyMap);

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
			CoordMapType* cartesianToPolarMap;

			/** Radius of the foveated area */
			double foveaRadius;

			/** Records whether the maps for converting cartesian to polar coordinates have been initialized */
			bool mapsInitialized;


			//========================  METHODS  ==============================
			double getDistance(int x, int y, double center_x, double center_y);
			CoordMapType* initialisePolarToCartesianMap(Bitmap* image, int polarWidth, int polarHeight, int foveaRadius);
			CoordMapType* initialiseCartesianToPolarMap(Bitmap* image, int polarWidth, int polarHeight, int foveaRadius);
			Bitmap* logPolar(Bitmap* input, int polarWidth, int polarHeight, CoordMapType* polarToCartesianMap);
			Bitmap* logPolarToCartesian(Bitmap* logPolarImage, int outputWidth, int outputHeight, CoordMapType* cartesianToPolarMap);
			void calculateReducedImage(Bitmap& bitmap);

	};

}

#endif /* LOGPOLARVISUALDATAREDUCER_HPP_ */
