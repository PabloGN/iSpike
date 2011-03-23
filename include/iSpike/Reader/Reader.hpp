/*
 * Reader.hpp
 *
 *  Created on: 1 Jan 2011
 *      Author: Edgars Lazdins
 */

#ifndef READER_HPP_
#define READER_HPP_

#include <iSpike/Reader/ReaderDescription.hpp>

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

  virtual void initialise(std::map<std::string,Property*> properties) = 0;

};

#endif /* READER_HPP_ */
