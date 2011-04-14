/*
 * MovementFilter.cpp
 *
 *  Created on: 5 Feb 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/VisualFilter/MovementFilter.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Common.hpp>
#include <boost/math/constants/constants.hpp>

/**
 * Default constructor
 * @param foveator The visual filter the image is retrieved from
 * @param queryInterval How often the thread queries new information
 */
MovementFilter::MovementFilter(VisualFilter* foveator, int queryInterval)
{
  this->foveator = foveator;
  this->queryInterval = queryInterval;
  this->threadPointer = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&MovementFilter::workerFunction, this)));
  this->previousImage = new Bitmap(0, 0, 0, NULL);
  this->movementMap = new Bitmap(0, 0, 0, NULL);
}

Bitmap* MovementFilter::produceMap(Bitmap* firstImage, Bitmap* secondImage)
{
  Bitmap* result = new Bitmap(firstImage->getWidth(), firstImage->getHeight(), firstImage->getDepth());
  for(int pixel = 0; pixel < (result->getHeight() * result->getWidth()); pixel++)
  {
    unsigned char* firstPtr = firstImage->getContents() + pixel * firstImage->getDepth();
    unsigned char* secondPtr = secondImage->getContents() + pixel * secondImage->getDepth();
    unsigned char* targetPtr = result->getContents() + pixel;
    for(int i = 0; i < result->getDepth(); i++)
      *(targetPtr + i) = abs(*(firstPtr + i) - *(secondPtr + i));
  }
  return result;
}

void MovementFilter::workerFunction()
{
  while(true)
  {
    Bitmap* currentImage = &(this->foveator->getRPlusGMinus());
    Bitmap* movementMap = produceMap(this->previousImage, currentImage);
    {
      boost::mutex::scoped_lock lock(this->mutex);
      delete this->previousImage;
      this->previousImage = currentImage;
      this->movementMap = movementMap;
    }
  }
}
