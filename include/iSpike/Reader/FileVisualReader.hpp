/*
 * FileVisualReader.hpp
 *
 *  Created on: 1 Jan 2011
 *      Author: cembo
 */

#ifndef FILEVISUALREADER_HPP_
#define FILEVISUALREADER_HPP_

#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/YarpInterface.hpp>
#include <iSpike/Bitmap.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/gil/gil_all.hpp>

/**
 * @class FileVisualReader
 * @brief FileVisualReader class
 *
 * This class represents a reader capable of retrieving an image from the hard drive
 * and continuously outputting it upon request
 *
 * @author Edgars Lazdins
 *
 */
class FileVisualReader : public VisualReader {

private:
  Bitmap* buffer;
  bool initialised;

public:

  /**
   * Retrieves the image
   */
  Bitmap getData();

  /**
   * Constructor
   * @param filename The path to the image
   */
  FileVisualReader(const char* filename);

  /**
   * Initialises the reader and starts the execution of the main thread
   */
  void start();

  /**
   * Returns true if the reader has been initialised
   */
  bool getInitialised() const
  {
      return initialised;
  }

  void setInitialised(bool initialised)
  {
      this->initialised = initialised;
  }

};

#endif /* FILEVISUALREADER_HPP_ */
