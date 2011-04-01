/*
 * LogpolarVisualDataReducer.cpp
 *
 *  Created on: 5 Feb 2011
 *      Author: Edgars Lazdins
 */
#include <iSpike/VisualDataReducer/LogpolarVisualDataReducer.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/Common.hpp>
#include <iostream>
#include <boost/math/constants/constants.hpp>

LogPolarVisualDataReducer::LogPolarVisualDataReducer(VisualReader* reader, int queryInterval)
{
  this->reader = reader;
  this->queryInterval = queryInterval;
  this->reducedImage = new Bitmap(0, 0, 0, NULL);
  this->threadPointer = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&LogPolarVisualDataReducer::workerFunction, this)));
}

void LogPolarVisualDataReducer::workerFunction()
{
  int polarWidth = 320;
  int polarHeight = 240;
  while(true)
  {
    Bitmap rawImage = this->reader->getData();
    if (rawImage.getWidth() != 0)
    {
      CoordMapType* polarToCartesianMap = initialisePolarToCartesianMap(&rawImage, polarWidth, polarHeight, 60);
      CoordMapType* cartesianToPolarMap = initialiseCartesianToPolarMap(&rawImage, polarWidth, polarHeight, 60);
      Bitmap* logPolarImage = logPolar(&rawImage, polarWidth, polarHeight, polarToCartesianMap);
      Common::savePPMImage("logPolar.ppm", logPolarImage);
      if(logPolarImage != NULL)
      {
        boost::mutex::scoped_lock lock(this->mutex);
        delete this->reducedImage;
        this->reducedImage = logPolarImage;
      }
      /*Bitmap* reducedImage = logPolarToCartesian(logPolarImage, &rawImage, rawImage.getWidth(), rawImage.getHeight(), 60, cartesianToPolarMap);
      Common::savePPMImage("reducedImage.ppm", reducedImage);
      if(reducedImage != NULL)
      {
        boost::mutex::scoped_lock lock(this->mutex);
        delete this->reducedImage;
        this->reducedImage = reducedImage;
      }
      delete logPolarImage;*/
      delete polarToCartesianMap;
      delete cartesianToPolarMap;
    }
   // boost::this_thread::sleep(boost::posix_time::milliseconds(this->queryInterval));
  }
}

double LogPolarVisualDataReducer::getR(int x, int y, double center_x, double center_y)
{
  return sqrt(pow(x - center_x,2) + pow(y - center_y,2));
}

CoordMapType* LogPolarVisualDataReducer::initialisePolarToCartesianMap(Bitmap* image,
    int polarWidth, int polarHeight, int foveaRadius)
{
  CoordMapType* polarToCartesianMap = new CoordMapType();
  double center_x = image->getWidth() / 2;
  double center_y = image->getHeight() / 2;
  /*double topLeft = log(getR(0, image->getHeight(), center_x, center_y));
  double topRight = log(getR(image->getWidth(), image->getHeight(), center_x, center_y));
  double bottomLeft = log(getR(0, 0, center_x, center_y));
  double bottomRight = log(getR(image->getWidth(), 0, center_x, center_y));
  double max_r = std::max( std::max(topLeft, topRight), std::max(bottomLeft, bottomRight) );*/
  //double width_r = log(getR(0, center_y, center_x, center_y));
  //double height_r = log(getR(center_x, 0, center_x, center_y));
  double width_r = getR(0, center_y, center_x, center_y);
  double height_r = getR(center_x, 0, center_x, center_y);
  double max_r = (std::min(width_r, height_r));
  double log_max_r = std::min(log(width_r), log(height_r));
  for(int y = 0; y < polarHeight; y++)
    for(int x = 0; x < polarWidth; x++)
    {
      double theta = (2 * boost::math::constants::pi<double>() * y ) / polarHeight;
      double r = ( max_r * x ) / polarWidth;
      int cartesian_x;
      int cartesian_y;
      if(r > 0)
      {
        double log_r = ( log_max_r * x ) / polarWidth;
        cartesian_x = (int)floor(center_x + ( exp(log_r) * cos(theta) + 0.5));
        cartesian_y = (int)floor(center_y + ( exp(log_r) * sin(theta) + 0.5));
        polarToCartesianMap->insert(CoordMapType::value_type(std::make_pair(x,y), std::make_pair(cartesian_x,cartesian_y)));
      } else {
        cartesian_x = (int)floor(center_x + ( r * cos(theta)) + 0.5);
        cartesian_y = (int)floor(center_y + ( r * sin(theta)) + 0.5);
        polarToCartesianMap->insert(CoordMapType::value_type(std::make_pair(x,y), std::make_pair(cartesian_x,cartesian_y)));
      }
      //int cartesian_x = center_x + ( exp(r) * cos(theta) );
      //int cartesian_y = center_y + ( exp(r) * sin(theta) );
      //int cartesian_x = (int)floor(center_x + ( r * cos(theta) ) + 0.5);
      //int cartesian_y = (int)floor(center_y + ( r * sin(theta) ) + 0.5);
      //polarToCartesianMap->insert(CoordMapType::value_type(std::make_pair(x,y), std::make_pair(cartesian_x,cartesian_y)));
      if(x == 47 && y == 124)
      {
        //std::cout << "polartocartesian" << std::endl;
        //std::cout << "x: " << cartesian_x << " y: " << cartesian_y << " polar_x: " << x <<
        //    "polar_y: " << y << "theta: " << theta << " r: " << log_r << " max_r: " << max_r << std::endl;
      }
      //std::cout << "polar_x:" << x << "polar_y:" << y << std::endl;
      //std::cout << "cartesian_x:" << cartesian_x << "cartesian_y:" << cartesian_y << std::endl;
      //std::cout << "r:" << r << "theta:" << theta << std::endl;
      //std::cout << "topLeft:" << topLeft << "topRight:" << topRight;
      //std::cout << "bottomLeft:" << bottomLeft << "bottomRight:" << bottomRight;
      //std::cout << "max_r:" << max_r;
    }
  return polarToCartesianMap;
}

CoordMapType* LogPolarVisualDataReducer::initialiseCartesianToPolarMap(Bitmap* image,
    int polarWidth, int polarHeight, int foveaRadius)
{
  CoordMapType* polarToCartesianMap = new CoordMapType();
  double center_x = image->getWidth() / 2;
  double center_y = image->getHeight() / 2;
  //double width_r = log(getR(0, center_y, center_x, center_y));
  //double height_r = log(getR(center_x, 0, center_x, center_y));
  double width_r = getR(0, center_y, center_x, center_y);
  double height_r = getR(center_x, 0, center_x, center_y);
  double max_r = (std::min(width_r, height_r));
  //double log_max_r = (std::min(log(width_r), log(height_r)));
  for(int y = 0; y < image->getHeight(); y++)
    for(int x = 0; x < image->getWidth(); x++)
    {
      //double r = log(getR(x, y, center_x, center_y));
      double r = getR(x, y, center_x, center_y);
      //if(r > foveaRadius)
      //  r = log(r);
      double theta = atan2(y - center_y, x - center_x);
      if(theta < 0)
        theta += 2 * boost::math::constants::pi<double>();
      int polar_x;
      //if(r <= foveaRadius)
        polar_x = (int)floor( ( r * polarWidth) / max_r + 0.5);
      //else
        //polar_x = (int)floor( ( r * polarWidth) / log_max_r + 0.5);
      int polar_y = (int)floor( ( theta * polarHeight ) / ( 2 * boost::math::constants::pi<double>() ) + 0.5);
      polarToCartesianMap->insert(CoordMapType::value_type(std::make_pair(x,y), std::make_pair(polar_x,polar_y)));
      if(x == center_x + 10 && y == center_y + 10)
      {
        //std::cout << "cartesiantopolar" << std::endl;
        //std::cout << "x: " << x << " y: " << y << " polar_x: " << polar_x << "polar_y: " << polar_y << "theta: " << theta << " r: " << r << std::endl;
      }
    }
  return polarToCartesianMap;
}

Bitmap* LogPolarVisualDataReducer::logPolar(Bitmap* input, int polarWidth, int polarHeight,
    CoordMapType* polarToCartesianMap)
{
  unsigned char* polar_buffer = new unsigned char[polarWidth * polarHeight * input->getDepth()];
  CoordMapType::const_iterator iterator;
  for(iterator = polarToCartesianMap->begin(); iterator != polarToCartesianMap->end(); ++iterator){
    int r = (*iterator).first.first;
    int theta = (*iterator).first.second;
    int x = (*iterator).second.first;
    int y = (*iterator).second.second;
    int destination = input->getDepth() * ( ( polarWidth * theta ) + r );
    int source = ( ( ( input->getWidth() * y  ) + x ) * input->getDepth() );
    unsigned char* destination_ptr = polar_buffer + destination;
    unsigned char* source_ptr;
    if(x >= 0 && y >= 0 && x < input->getWidth() && y < input->getHeight())
    {
      source_ptr = input->getContents() + source;
      memmove(destination_ptr, source_ptr, input->getDepth());
    }
    else
    {
      source_ptr = new unsigned char[input->getDepth()];
      memset(source_ptr, 0, input->getDepth());
      memmove(destination_ptr, source_ptr, input->getDepth());
      delete[] source_ptr;
    }
  }
  return new Bitmap(polarWidth, polarHeight, input->getDepth(), polar_buffer);
}

Bitmap* LogPolarVisualDataReducer::logPolarToCartesian(Bitmap* logPolarImage, Bitmap* cartesianImage, int outputWidth, int outputHeight, int foveaDiameter,
    CoordMapType* cartesianToPolarMap)
{
  unsigned char* cartesian_buffer = (unsigned char*) malloc( outputWidth * outputHeight * logPolarImage->getDepth() );
  CoordMapType::const_iterator iterator;
  for(iterator = cartesianToPolarMap->begin(); iterator != cartesianToPolarMap->end(); ++iterator){
    int x = (*iterator).first.first;
    int y = (*iterator).first.second;
    int r = (*iterator).second.first;
    int theta = (*iterator).second.second;
    int destination = logPolarImage->getDepth() * ( ( outputWidth * y ) + x );
    unsigned char* source_ptr;
    unsigned char* destination_ptr = cartesian_buffer + destination;
    //std::cout << "x:" << x << "y:" << y << std::endl;
    //std::cout << "r:" << r << "theta:" << theta << std::endl;
    /*if(r <= foveaDiameter)
    {
      source_ptr = cartesianImage->getContents() + destination;
      memmove(destination_ptr, source_ptr, logPolarImage->getDepth());
    } else */if(r >= 0 && r < logPolarImage->getWidth() && theta >= 0 && theta < logPolarImage->getHeight()) {
      int source = logPolarImage->getDepth() * ( ( logPolarImage->getWidth() * theta ) + r );
      source_ptr = logPolarImage->getContents() + source;
      memmove(destination_ptr, source_ptr, logPolarImage->getDepth());
    } else {
      source_ptr = new unsigned char[logPolarImage->getDepth()];
      memset(source_ptr, 0, logPolarImage->getDepth());
      memmove(destination_ptr, source_ptr, logPolarImage->getDepth());
      delete[] source_ptr;
    }
  }
  return new Bitmap(outputWidth, outputHeight, logPolarImage->getDepth(), cartesian_buffer);
}



