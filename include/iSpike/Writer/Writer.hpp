/*
 * Writer.hpp
 *
 *  Created on: 23 Feb 2011
 *      Author: Edgars Lazdins
 */

#ifndef WRITER_HPP_
#define WRITER_HPP_

#include <iSpike/Writer/WriterDescription.hpp>

/**
 * @class Writer
 * @brief Writer class
 *
 * This class represents a Writer
 *
 * @author Edgars Lazdins
 *
 */
class Writer {

protected:
  WriterDescription* writerDescription;

public:
  WriterDescription getWriterDescription() const
    {
        return *(writerDescription);
    }

    void initialise()
    {
      initialise(writerDescription->getWriterProperties());
    }

    virtual void initialise(std::map<std::string,Property*> properties) = 0;
};

#endif /* WRITER_HPP_ */
