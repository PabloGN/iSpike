/*
 * WriterDescription.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: cembo
 */

#ifndef WRITERDESCRIPTION_HPP_
#define WRITERDESCRIPTION_HPP_

#include <string>

class WriterDescription {

private:
    std::string writerName;
    std::string writerDescription;
    std::string writerType;

public:

    WriterDescription(std::string writerName, std::string writerDescription, std::string writerType)
    {
      this->writerName = writerName;
      this->writerDescription = writerDescription;
      this->writerType = writerType;
    }

    std::string getWriterDescription() const
    {
        return writerDescription;
    }

    std::string getWriterName() const
    {
        return writerName;
    }

    std::string getWriterType() const
    {
        return writerType;
    }

};

#endif /* WRITERDESCRIPTION_HPP_ */
