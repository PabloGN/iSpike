/*
 * DOGVisualFilter.cpp
 *
 *  Created on: 5 Feb 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/VisualFilter/DOGVisualFilter.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Common.hpp>

DOGVisualFilter::DOGVisualFilter(VisualDataReducer* reducer, int queryInterval)
{
  this->reducer = reducer;
  this->queryInterval = queryInterval;
  this->rPlusGMinus = new Bitmap(0, 0, 0, NULL);
  this->gPlusRMinus = new Bitmap(0, 0, 0, NULL);
  this->bPlusYMinus = new Bitmap(0, 0, 0, NULL);
  this->threadPointer = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&DOGVisualFilter::workerFunction, this)));
}

void DOGVisualFilter::workerFunction()
{
  int plusSigma = 3;
  int minusSigma = 2;
  while(true)
  {
    Bitmap reducedImage = this->reducer->getReducedImage();
    if(reducedImage.getWidth() > 0)
    {
      unsigned char* red = extractRedChannel(&reducedImage);
      unsigned char* green = extractGreenChannel(&reducedImage);
      unsigned char* blue = extractBlueChannel(&reducedImage);
      unsigned char* yellow = extractYellowChannel(red, green, reducedImage.getWidth(), reducedImage.getHeight());
      unsigned char* redPlusGaussian = gaussianBlur(red, plusSigma, reducedImage.getWidth(), reducedImage.getHeight());
      unsigned char* redMinusGaussian = gaussianBlur(red, minusSigma, reducedImage.getWidth(), reducedImage.getHeight());
      unsigned char* greenPlusGaussian = gaussianBlur(green, plusSigma, reducedImage.getWidth(), reducedImage.getHeight());
      unsigned char* greenMinusGaussian = gaussianBlur(green, minusSigma, reducedImage.getWidth(), reducedImage.getHeight());
      unsigned char* bluePlusGaussian = gaussianBlur(blue, plusSigma, reducedImage.getWidth(), reducedImage.getHeight());
      unsigned char* yellowMinusGaussian = gaussianBlur(yellow, minusSigma, reducedImage.getWidth(), reducedImage.getHeight());
      unsigned char* rPlusGMinus = subtractImages(redPlusGaussian, greenMinusGaussian, 1, 1, reducedImage.getWidth(), reducedImage.getHeight());
      unsigned char* gPlusRMinus = subtractImages(greenPlusGaussian, redMinusGaussian, 1, 1, reducedImage.getWidth(), reducedImage.getHeight());
      unsigned char* bPlusYMinus = subtractImages(bluePlusGaussian, yellowMinusGaussian, 1, 1, reducedImage.getWidth(), reducedImage.getHeight());
      unsigned char* rPlusGMinusNormalised = Common::normaliseImage(rPlusGMinus, reducedImage.getWidth() * reducedImage.getHeight());
      unsigned char* gPlusRMinusNormalised = Common::normaliseImage(gPlusRMinus, reducedImage.getWidth() * reducedImage.getHeight());
      unsigned char* bPlusYMinusNormalised = Common::normaliseImage(bPlusYMinus, reducedImage.getWidth() * reducedImage.getHeight());
      Bitmap* image = Common::produceGrayscale(rPlusGMinus, reducedImage.getWidth(), reducedImage.getHeight());
      Bitmap* normalisedImage = Common::produceGrayscale(rPlusGMinusNormalised, reducedImage.getWidth(), reducedImage.getHeight());
      Common::savePPMImage("rPlusGMinus.ppm", image);
      Common::savePPMImage("normalised.ppm", normalisedImage);
      delete image;
      delete normalisedImage;
      if(rPlusGMinus != NULL)
      {
        boost::mutex::scoped_lock lock(this->mutex);
        delete this->rPlusGMinus;
        this->rPlusGMinus = new Bitmap(reducedImage.getWidth(), reducedImage.getHeight(), 1, rPlusGMinusNormalised);
      }
      if(gPlusRMinus != NULL)
      {
        boost::mutex::scoped_lock lock(this->mutex);
        delete this->gPlusRMinus;
        this->gPlusRMinus = new Bitmap(reducedImage.getWidth(), reducedImage.getHeight(), 1, gPlusRMinusNormalised);
      }
      if(bPlusYMinus != NULL)
      {
        boost::mutex::scoped_lock lock(this->mutex);
        delete this->bPlusYMinus;
        this->bPlusYMinus = new Bitmap(reducedImage.getWidth(), reducedImage.getHeight(), 1, bPlusYMinusNormalised);
      }
      free(red);
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
      free(bPlusYMinus);
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
    kernel[i] = k / sqrt( 2 * M_PI * sigma * sigma );
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
