/*
 * FileVisualReader.hpp
 *
 *  Created on: 1 Jan 2011
 *      Author: Edgars Lazdins
 */

#ifndef FILEVISUALREADER_HPP_
#define FILEVISUALREADER_HPP_

#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/YarpConnection.hpp>
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

  /*
   * The default constructor, only initialises the default parameters and the description
   */
  FileVisualReader()
  {
    /**
     * First define the properties of this reader
     */
    std::map<std::string,Property*> properties;
    properties["Filename"] = new StringProperty(
          "Filename",
          "image.ppm",
          "Path to a PPM format image",
          true
        );
    /**
     * Now let's create the description
     */
    this->readerDescription = new ReaderDescription(
          "File Visual Reader",
          "This is a file visual reader",
          "Visual Reader",
          properties
        );
  }

  /*~FileVisualReader()
  {
    delete this->buffer;
  }*/

  /**
   * Retrieves the image
   */
  Bitmap getData();

  void initialise(std::map<std::string,Property*> properties);

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
