/*
 * Reader.hpp
 *
 *  Created on: 1 Jan 2011
 *      Author: Edgars Lazdins
 */

#ifndef READER_HPP_
#define READER_HPP_

#include <iSpike/Reader/ReaderDescription.hpp>
#include <iSpike/Log/Log.hpp>

/**
 * @class Reader
 * @brief Reader class
 *
 * This class represents a Reader, capable of retrieving analogue information
 * from a predefined source and serving it upon request
 *
 * @author Edgars Lazdins
 *
 */
class Reader {

protected:
  ReaderDescription* readerDescription;

public:
  ReaderDescription getReaderDescription() const
  {
      return *(readerDescription);
  }

  void initialise()
  {
    initialise(readerDescription->getReaderProperties());
  }

  virtual void initialise(property_map properties) = 0;

  virtual ~Reader()
  {
    delete this->readerDescription;
  }

};

#endif /* READER_HPP_ */
