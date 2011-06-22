/*
 * DOGVisualFilter.hpp
 *
 *  Created on: 5 Feb 2011
 *      Author: Edgars Lazdins
 */

#ifndef DOGVISUALFILTER_HPP_
#define DOGVISUALFILTER_HPP_

#include <iSpike/VisualFilter/VisualFilter.hpp>
#include <iSpike/VisualDataReducer/VisualDataReducer.hpp>
#include <iSpike/Bitmap.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

/**
 * @class DOGVisualFilter
 * @brief DOGVisualFilter class
 *
 * This class represents a Difference Of Gaussians filter
 *
 * @author Edgars Lazdins
 *
 */
class DOGVisualFilter : public VisualFilter {
private:

  Bitmap* buffer;
  VisualDataReducer* reducer;
  int queryInterval;
  boost::shared_ptr<boost::thread> threadPointer;
  boost::mutex mutex;
  double plusSigma;
  double minusSigma;
  int opponencyMap;

  /**
   * Main thread execution loop
   */
  void workerFunction();

  /**
   * Gaussian blurs an image
   */
  unsigned char* gaussianBlur(unsigned char* image, double sigma, int width, int height);

  /**
   * Extracts the red channel from a given image
   */
  unsigned char* extractRedChannel(Bitmap* image);

  /**
   * Extracts the green channel from a given image
   */
  unsigned char* extractGreenChannel(Bitmap* image);

  /**
   * Extracts the blue channel from a given image
   */
  unsigned char* extractBlueChannel(Bitmap* image);

  /**
   * Constructs the yellow channel from red and green channels
   */
  unsigned char* extractYellowChannel(unsigned char* redChannel, unsigned char* greenChannel, int width, int height);

  /**
   * Subtracts one image from another of the same dimensions, ratios can be applied to each
   */
  unsigned char* subtractImages(unsigned char* firstImage, unsigned char* secondImage, double ratio1, double ratio2, int width, int height);

public:

  DOGVisualFilter(
		  VisualDataReducer* reducer,
		  int queryInterval,
		  double plusSigma,
		  double minusSigma,
		  int opponencyMap
	  );

  /**
   * Returns the opponency map
   */
  Bitmap getOpponencyMap()
  {
    boost::mutex::scoped_lock lock(this->mutex);
    return *(this->buffer);
  }

};

#endif /* DOGVISUALFILTER_HPP_ */
