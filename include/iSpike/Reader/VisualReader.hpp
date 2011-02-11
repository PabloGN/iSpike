/*
 * FileVisualReader.hpp
 *
 *  Created on: 1 Jan 2011
 *      Author: cembo
 */

#ifndef VISUALREADER_HPP_
#define VISUALREADER_HPP_

#include <iSpike/Reader/Reader.hpp>
#include <iSpike/YarpInterface.hpp>
#include <iSpike/Bitmap.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/gil/gil_all.hpp>

/**
 * @class VisualReader
 * @brief VisualReader class
 *
 * VisualReader continuously fetches and retrieves an image upon request
 *
 * @author Edgars Lazdins
 *
 */
class VisualReader : public Reader {

public:

  /**
   * Retrieves the image
   */
  virtual Bitmap getData() = 0;

  /**
   * Initialises the reader and starts the main thread
   */
  virtual void start() = 0;

  virtual bool getInitialised() const = 0;

  virtual void setInitialised(bool initialised) = 0;

};

#endif /* VISUALREADER_HPP_ */
