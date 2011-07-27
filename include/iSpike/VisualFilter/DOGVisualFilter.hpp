#ifndef DOGVISUALFILTER_HPP_
#define DOGVISUALFILTER_HPP_

//iSpike includes
#include <iSpike/VisualFilter/VisualFilter.hpp>
#include <iSpike/VisualDataReducer/VisualDataReducer.hpp>
#include <iSpike/Bitmap.hpp>
#include "iSpike/iSpikeThread.hpp"

//Other includes
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

namespace ispike {

	/** This class represents a Difference Of Gaussians filter */
	class DOGVisualFilter : public VisualFilter, public PropertyHolder {
		public:
			DOGVisualFilter(VisualDataReducer* reducer, int queryInterval, double plusSigma, double minusSigma, double ratio1, double ratio2, int opponencyMap);
			~DOGVisualFilter();
			Bitmap& getOpponencyMap();
			void update();

		private:
			//==========================  VARIABLES  =======================
			VisualDataReducer* reducer;
			int queryInterval;
			boost::shared_ptr<boost::thread> threadPointer;
			boost::mutex mutex;
			double plusSigma;
			double minusSigma;
			double ratio1;
			double ratio2;
			int opponencyTypeID;

			/** Records when class has been initialized */
			bool initialized;

			/** Final output bitmap */
			Bitmap* opponencyBitmap;

			/** Red visual data */
			Bitmap* redBitmap;

			/** Green visual data */
			Bitmap* greenBitmap;

			/** Blue visual data */
			Bitmap* blueBitmap;

			/** Yellow visual data */
			Bitmap* yellowBitmap;

			/** Positive blurred bitmap */
			Bitmap* positiveBlurredBitmap;

			/** Negative blurred bitmap */
			Bitmap negativeBlurredBitmap;


			//==========================  METHODS  =========================
			unsigned char* extractRedChannel(Bitmap* image);
			unsigned char* extractGreenChannel(Bitmap* image);
			unsigned char* extractBlueChannel(Bitmap* image);
			unsigned char* extractYellowChannel(unsigned char* redChannel, unsigned char* greenChannel, int width, int height);
			unsigned char* gaussianBlur(unsigned char* image, double sigma, int width, int height);
			unsigned char* subtractImages(unsigned char* firstImage, unsigned char* secondImage, double ratio1, double ratio2, int width, int height);
	};

}

#endif /* DOGVISUALFILTER_HPP_ */
