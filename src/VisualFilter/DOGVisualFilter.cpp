/*
 * DOGVisualFilter.cpp
 *
 *  Created on: 5 Feb 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/VisualFilter/DOGVisualFilter.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Common.hpp>
#include <boost/math/constants/constants.hpp>

/**
 * Default constructor
 * @param reducer The Visual Data reducer where the foveated/logpolar image is retrieved from
 * @param queryInterval How often the thread queries new information
 * @param plusSigma The intensity of the Positive blur
 * @param minusSigma The intensity of the Negative blur
 */
DOGVisualFilter::DOGVisualFilter(VisualDataReducer* reducer, int queryInterval, double plusSigma, double minusSigma, int opponencyMap)
{
  this->reducer = reducer;
  this->queryInterval = queryInterval;
  this->buffer = new Bitmap(0, 0, 0, NULL);
  this->plusSigma = plusSigma;
  this->minusSigma = minusSigma;
  this->opponencyMap = opponencyMap;
  this->threadPointer = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&DOGVisualFilter::workerFunction, this)));
}

/**
 * Worker function, on each iteration :
 *  * Retrives a reduced image
 *  * Decomposes it into individual colour channels
 *  * Blurs each of these channels
 *  * Subtracts the channels one from another
 *  * Normalises the resultant images
 *  * Stores each image in the appropriate buffer
 */
void DOGVisualFilter::workerFunction()
{
  while(true)
  {
    Bitmap reducedImage = this->reducer->getReducedImage();
    //Common::savePPMImage("logPolar.ppm", &reducedImage);
    if(reducedImage.getWidth() > 0)
    {
      LOG(LOG_DEBUG) << "Got a reduced image, width: " << reducedImage.getWidth();
        //unsigned char* red = extractRedChannel(&reducedImage);
        Bitmap red(reducedImage.getWidth(), reducedImage.getHeight(), 1, extractRedChannel(&reducedImage));
        Common::savePPMImage("red.ppm", Common::produceGrayscale(red.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Bitmap green(reducedImage.getWidth(), reducedImage.getHeight(), 1, extractGreenChannel(&reducedImage));
        Common::savePPMImage("green.ppm", Common::produceGrayscale(green.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Bitmap blue(reducedImage.getWidth(), reducedImage.getHeight(), 1, extractBlueChannel(&reducedImage));
        Common::savePPMImage("blue.ppm", Common::produceGrayscale(blue.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Bitmap yellow(reducedImage.getWidth(), reducedImage.getHeight(), 1,
            extractYellowChannel(red.getContents(), green.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
      if(this->opponencyMap == Common::redVsGreen)
      {
        //Common::savePPMImage("yellow.ppm", Common::produceGrayscale(yellow.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Bitmap redPlusGaussian(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          gaussianBlur(red.getContents(), this->plusSigma, reducedImage.getWidth(), reducedImage.getHeight())
        );
        Common::savePPMImage("redPlusGaussian.ppm", Common::produceGrayscale(redPlusGaussian.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        //Common::savePPMImage("greenPlusGaussian.ppm", Common::produceGrayscale(greenPlusGaussian.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Bitmap greenMinusGaussian(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          gaussianBlur(green.getContents(), this->minusSigma, reducedImage.getWidth(), reducedImage.getHeight())
        );
        Common::savePPMImage("greenMinusGaussian.ppm", Common::produceGrayscale(greenMinusGaussian.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        //Common::savePPMImage("yellowMinusGaussian.ppm", Common::produceGrayscale(yellowMinusGaussian.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Bitmap rPlusGMinus(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          subtractImages(redPlusGaussian.getContents(), greenMinusGaussian.getContents(), 1, 1, reducedImage.getWidth(), reducedImage.getHeight())
        );
        //Common::savePPMImage("bPlusYMinus.ppm", Common::produceGrayscale(bPlusYMinus.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Common::savePPMImage("rPlusGMinus.ppm", Common::produceGrayscale(rPlusGMinus.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Bitmap* rPlusGMinusNormalised = new Bitmap(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          Common::normaliseImage(rPlusGMinus.getContents(), reducedImage.getWidth() * reducedImage.getHeight())
        );
        Common::savePPMImage("rPlusGMinusNormalised.ppm", Common::produceGrayscale(rPlusGMinusNormalised->getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        boost::mutex::scoped_lock lock(this->mutex);
        delete this->buffer;
        this->buffer = rPlusGMinusNormalised;
      } else if(this->opponencyMap == Common::greenVsRed)
      {
        Bitmap redMinusGaussian(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          gaussianBlur(red.getContents(), this->minusSigma, reducedImage.getWidth(), reducedImage.getHeight())
        );
        //Common::savePPMImage("redMinusGaussian.ppm", Common::produceGrayscale(redMinusGaussian.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Bitmap greenPlusGaussian(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          gaussianBlur(green.getContents(), this->plusSigma, reducedImage.getWidth(), reducedImage.getHeight())
        );
        Bitmap gPlusRMinus(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          subtractImages(greenPlusGaussian.getContents(), redMinusGaussian.getContents(), 1, 1, reducedImage.getWidth(), reducedImage.getHeight())
        );
        Bitmap* gPlusRMinusNormalised = new Bitmap(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          Common::normaliseImage(gPlusRMinus.getContents(), reducedImage.getWidth() * reducedImage.getHeight())
        );
        boost::mutex::scoped_lock lock(this->mutex);
        delete this->buffer;
        this->buffer = gPlusRMinusNormalised;
      } else if(this->opponencyMap == Common::blueVsYellow)
      {
        Bitmap bluePlusGaussian(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          gaussianBlur(blue.getContents(), this->plusSigma, reducedImage.getWidth(), reducedImage.getHeight())
        );
        //Common::savePPMImage("bluePlusGaussian.ppm", Common::produceGrayscale(bluePlusGaussian.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Bitmap yellowMinusGaussian(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          gaussianBlur(yellow.getContents(), this->minusSigma, reducedImage.getWidth(), reducedImage.getHeight())
        );
        //Common::savePPMImage("gPlusRMinus.ppm", Common::produceGrayscale(gPlusRMinus.getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Bitmap bPlusYMinus(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          subtractImages(bluePlusGaussian.getContents(), yellowMinusGaussian.getContents(), 1, 1, reducedImage.getWidth(), reducedImage.getHeight())
        );
        //Common::savePPMImage("gPlusRMinusNormalised.ppm", Common::produceGrayscale(gPlusRMinusNormalised->getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
        Bitmap* bPlusYMinusNormalised = new Bitmap(
          reducedImage.getWidth(), reducedImage.getHeight(), 1,
          Common::normaliseImage(bPlusYMinus.getContents(), reducedImage.getWidth() * reducedImage.getHeight())
        );
        boost::mutex::scoped_lock lock(this->mutex);
        delete this->buffer;
        this->buffer = bPlusYMinusNormalised;
      }
      //Common::savePPMImage("bPlusYMinusNormalised.ppm", Common::produceGrayscale(bPlusYMinusNormalised->getContents(), reducedImage.getWidth(), reducedImage.getHeight()));
      //Bitmap* image = Common::produceGrayscale(rPlusGMinus.getContents(), reducedImage.getWidth(), reducedImage.getHeight());
      //Bitmap* normalisedImage = Common::produceGrayscale(rPlusGMinusNormalised->getContents(), reducedImage.getWidth(), reducedImage.getHeight());
      //Common::savePPMImage("rPlusGMinus.ppm", image);
      //Common::savePPMImage("normalised.ppm", normalisedImage);
      //delete image;
      //delete normalisedImage;
      /*free(red);
      free(green);
      free(blue);
      free(yellow);
      free(redPlusGaussian);
      free(redMinusGaussian);
      free(greenPlusGaussian);
      free(greenMinusGaussian);
      free(bluePlusGaussian);
      free(yellowMinusGaussian);
      free(rPlusGMinus);
      free(gPlusRMinus);
      free(bPlusYMinus);*/
    }
    //boost::this_thread::sleep(boost::posix_time::milliseconds(this->queryInterval));
  }
}

unsigned char* DOGVisualFilter::gaussianBlur(unsigned char* image, double sigma, int width, int height)
{
  unsigned char* tempBuffer = (unsigned char*) calloc(width, height);
  unsigned char* resultBuffer = (unsigned char*) calloc(width, height);
  double* kernel = new double[(int)ceil(6 * sigma) + 1];
  for(int i = 0; i < ceil( 6 * sigma ) + 1; i++)
  {
    int x = i - ceil( 3 * sigma );
    double k = exp( - ( (x * x) / ( 2 * sigma * sigma ) ) );
    kernel[i] = k / sqrt( 2 * boost::math::constants::pi<double>() * sigma * sigma );
  }

  //iterate horizontally
  for(int y = 0; y < height; y++)
    for(int x = 0; x < width; x++)
    {
      unsigned char* targetPtr = tempBuffer + ( y * width ) + x;
      double colour_value = 0;
      //iterate over the kernel
      for(int i = 0; i < ceil( 6 * sigma ) + 1; i++)
      {
        int kernel_x = i - ceil( 3 * sigma );
        if(x + kernel_x < 0 )
          continue;
        else if(x + kernel_x > width)
          continue;
        unsigned char* sourcePtr = image + ( y * width ) + x;
        unsigned char current_colour = (unsigned char) *(sourcePtr + kernel_x);
        double current_kernel = kernel[i];
        colour_value += current_colour * current_kernel;
      }
      *targetPtr = (int)floor( colour_value + 0.5 );
    }

  //iterate vertically
  for(int x = 0; x < width; x++)
    for(int y = 0; y < height; y++)
    {
      unsigned char* targetPtr = resultBuffer + ( y * width ) + x;
      double colour_value = 0;
      //iterate over the kernel
      for(int i = 0; i < ceil( 6 * sigma ) + 1; i++)
      {
        int kernel_y = i - ceil( 3 * sigma );
        if(y + kernel_y <= 0 )
          continue;
        else if(y + kernel_y >= height)
          continue;
        unsigned char* sourcePtr = tempBuffer + ( y * width ) + x;
        unsigned char current_colour = (unsigned char) *(sourcePtr + (kernel_y * width));
        double current_kernel = kernel[i];
        colour_value += current_colour * current_kernel;
      }
      *targetPtr += (int)floor( colour_value + 0.5 );
    }
  free(tempBuffer);
  return resultBuffer;
}

unsigned char* DOGVisualFilter::extractRedChannel(Bitmap* image)
{
  unsigned char* redBuffer = (unsigned char*) calloc(image->getWidth(), image->getHeight());
  for(int pixel = 0; pixel < (image->getHeight() * image->getWidth()); pixel++)
  {
    unsigned char* sourcePtr = image->getContents() + pixel * image->getDepth();
    unsigned char* targetPtr = redBuffer + pixel;
    *targetPtr = *sourcePtr;
  }
  return redBuffer;
}

unsigned char* DOGVisualFilter::extractGreenChannel(Bitmap* image)
{
  unsigned char* greenBuffer = (unsigned char*) calloc(image->getWidth(), image->getHeight());
  for(int pixel = 0; pixel < (image->getHeight() * image->getWidth()); pixel++)
  {
    unsigned char* sourcePtr = image->getContents() + pixel*image->getDepth() + 1;
    unsigned char* targetPtr = greenBuffer + pixel;
    *targetPtr = *sourcePtr;
  }
  return greenBuffer;
}

unsigned char* DOGVisualFilter::extractBlueChannel(Bitmap* image)
{
  unsigned char* blueBuffer = (unsigned char*) calloc(image->getWidth(), image->getHeight());
  for(int pixel = 0; pixel < (image->getHeight() * image->getWidth()); pixel++)
  {
    unsigned char* sourcePtr = image->getContents() + pixel*image->getDepth() + 2;
    unsigned char* targetPtr = blueBuffer + pixel;
    *targetPtr = *sourcePtr;
  }
  return blueBuffer;
}

unsigned char* DOGVisualFilter::extractYellowChannel(unsigned char* redChannel, unsigned char* greenChannel, int width, int height)
{
  unsigned char* resultBuffer = (unsigned char*) calloc(width, height);
  for(int i = 0; i < width * height; i++)
  {
    unsigned char redPixel = (unsigned char) *(redChannel + i);
    unsigned char greenPixel = (unsigned char) *(greenChannel + i);
    unsigned char yellowValue = (unsigned int) floor( ( ( (int) redPixel + (int) greenPixel ) / 2 ) + 0.5 );
    *(resultBuffer + i) = yellowValue;
  }
  return resultBuffer;
}

unsigned char* DOGVisualFilter::subtractImages(unsigned char* firstImage, unsigned char* secondImage, double ratio1, double ratio2, int width, int height)
{
  unsigned char* resultBuffer = (unsigned char*) calloc(width, height);
  for(int i = 0; i < width * height; i++)
  {
    unsigned char firstPixel = (unsigned char) *(firstImage + i);
    unsigned char secondPixel = (unsigned char) *(secondImage + i);
    int difference = ratio1 * firstPixel - ratio2 * secondPixel;
    if(difference < 0)
      difference = 0;
    unsigned char resultPixel = difference;
    *(resultBuffer + i) = resultPixel;
  }
  return resultBuffer;
}
