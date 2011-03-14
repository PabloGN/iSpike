/*
 * ReaderFactory.hpp
 *
 *  Created on: 13 Mar 2011
 *      Author: cembo
 */

#ifndef READERFACTORY_HPP_
#define READERFACTORY_HPP_

#include <vector>
#include <iSpike/Reader/Reader.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Reader/ReaderDescription.hpp>

class ReaderFactory {

private:
  static std::vector<ReaderDescription> readerList;
  static std::vector<ReaderDescription> initialiseReaderList()
  {
    std::vector<ReaderDescription> readerList;

    readerList.push_back(FileAngleReader::readerDescription);
    return readerList;
  }

public:

  /*
   * Returns all readers of a particular type
   */
  static std::vector<ReaderDescription> getReadersOfType(std::string readerType)
  {
    std::vector<ReaderDescription> result;
    for(int i = 0; i < readerList.size(); i++)
    {
      if(readerList[i].getReaderType() == readerType)
        result.push_back(readerList[i]);
    }
    return result;
  }

  /*
   * Creates a particular reader
   */
  static Reader* create(std::string readerName)
  {
    //if(channelName == "File Angle Reader")
    //  return new FileAngleReader((AngleReader*)reader);
  }
};


#endif /* READERFACTORY_HPP_ */
