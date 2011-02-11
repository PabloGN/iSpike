/*
 * Common.hpp
 *
 *  Created on: 7 Feb 2011
 *      Author: cembo
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <SpikeAdapter/Bitmap.hpp>

class Common {

public:

  static void savePPMImage(char* filename, Bitmap* image);
  static Bitmap* produceGrayscale(unsigned char* image, int width, int height);
  static unsigned char* normaliseImage(unsigned char* image, int size);

};

#endif /* COMMON_HPP_ */
