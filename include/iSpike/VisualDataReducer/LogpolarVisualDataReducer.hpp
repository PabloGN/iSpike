/*
 * LogpolarVisualDataReducer.hpp
 *
 *  Created on: 5 Feb 2011
 *      Author: cembo
 */

#ifndef LOGPOLARVISUALDATAREDUCER_HPP_
#define LOGPOLARVISUALDATAREDUCER_HPP_

#include <iSpike/VisualDataReducer/VisualDataReducer.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Reader/VisualReader.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

typedef std::map< std::pair< int,int >, std::pair< int,int > > CoordMapType;

/**
 * @class Log Polar Visual Data Reducer
 * @brief Log Polar Visual Data Reducer class
 *
 * This class represents a Log Polar Visual Data Reducer capable of transforming an image
 * from cartesian to logpolar coordinates
 *
 * @author Edgars Lazdins
 *
 */
class LogPolarVisualDataReducer : public VisualDataReducer {
private:

  Bitmap* reducedImage;
  VisualReader* reader;
  int queryInterval;

  /**
   * The main thread execution loop
   */
  void workerFunction();

  /**
   * Initialises a pixel map from polar to cartesian coordinates
   */
  CoordMapType* initialisePolarToCartesianMap(Bitmap* image,
      int polarWidth, int polarHeight, int foveaRadius);

  /**
   * Initialises a pixel map from cartesian to polar coordinates
   */
  CoordMapType* initialiseCartesianToPolarMap(Bitmap* image,
      int polarWidth, int polarHeight, int foveaRadius);

  /**
   * Performs the conversion of a cartesian to logPolar image
   */
  Bitmap* logPolar(Bitmap* input, int polarWidth, int polarHeight,
      CoordMapType* polarToCartesianMap);

  /**
   * Maps the logpolar image to cartesian space
   */
  Bitmap* logPolarToCartesian(Bitmap* logPolarImage, Bitmap* cartesianImage, int outputWidth, int outputHeight, int foveaDiameter,
      CoordMapType* cartesianToPolarMap);

  /**
   * Retrieves the R parameter - distance from center
   */
  double getR(int x, int y, double center_x, double center_y);
  boost::shared_ptr<boost::thread> threadPointer;
  boost::mutex mutex;

public:

  LogPolarVisualDataReducer(VisualReader* reader, int queryInterval);
  Bitmap getReducedImage()
  {
    boost::mutex::scoped_lock lock(this->mutex);
    return *(this->reducedImage);
  }

};

#endif /* LOGPOLARVISUALDATAREDUCER_HPP_ */
