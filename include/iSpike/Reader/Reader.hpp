/*
 * Reader.hpp
 *
 *  Created on: 1 Jan 2011
 *      Author: Edgars Lazdins
 */

#ifndef READER_HPP_
#define READER_HPP_

#include <boost/scoped_ptr.hpp>
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
	boost::scoped_ptr<ReaderDescription> readerDescription;

public:

  ReaderDescription getReaderDescription() const
  {
      return *readerDescription;
  }

  virtual void initialise(property_map properties) = 0;

  void initialise()
  {
    initialise(readerDescription->getReaderProperties());
  }

};

#endif /* READER_HPP_ */
