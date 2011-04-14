/*
 * MagnocellularFilter.hpp
 *
 *  Created on: 14 Apr 2011
 *      Author: Edgars Lazdins
 */

#ifndef MOVEMENTFILTER_HPP_
#define MOVEMENTFILTER_HPP_

#include <iSpike/VisualFilter/VisualFilter.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Common.hpp>

class MovementFilter {
private:

  Bitmap* movementMap;
  Bitmap* previousImage;
  VisualFilter* foveator;
  int queryInterval;
  boost::shared_ptr<boost::thread> threadPointer;
  boost::mutex mutex;

  /**
   * Main thread execution loop
   */
  void workerFunction();

  /**
   * Produces a movement map between two images
   */
  Bitmap* produceMap(Bitmap* firstImage, Bitmap* secondImage);

public:

  MovementFilter(VisualFilter* foveator, int queryInterval);

  /**
   * Returns the Movement Map
   */
  Bitmap getMovementMap()
  {
    boost::mutex::scoped_lock lock(this->mutex);
    return *(this->movementMap);
  }

};

#endif /* MOVEMENTFILTER_HPP_ */
