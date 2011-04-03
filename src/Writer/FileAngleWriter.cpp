/*
 * FileAngleWriter.cpp
 *
 *  Created on: 9 Mar 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/Writer/FileAngleWriter.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <iSpike/ISpikeException.hpp>
#include <sstream>

void FileAngleWriter::initialise(std::map<std::string,Property*> properties)
{
  this->angleList = new std::queue<double>();
  this->fileName = ((StringProperty*)(properties["Filename"]))->getValue();
}

void FileAngleWriter::addAngle(double angle)
{
  boost::mutex::scoped_lock lock(this->mutex);
  this->angleList->push(angle);
}

void FileAngleWriter::start()
{
  if(!initialised)
  {
    this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&FileAngleWriter::workerFunction, this))));
    initialised = true;
  }
}

void writeAngleToFile(const char* fileName, double angle)
{
 std::ofstream fileStream;

 fileStream.open(fileName, std::fstream::out);

 if (!fileStream) {
   std::ostringstream errorStream;
   errorStream << "FileAngleWriter: Could not write angles to " << fileName;
   throw ISpikeException(errorStream.str());
 }

 fileStream << boost::lexical_cast<std::string>(angle) << std::endl;

 if (fileStream.fail()) {
   std::ostringstream errorStream;
   errorStream << "FileAngleWriter: Could not write angles to " << fileName;
   throw ISpikeException(errorStream.str());
 }

 fileStream.close();

}

void FileAngleWriter::workerFunction()
{
  int sleepAmount = 1;
  while(true)
  {
    if(this->angleList->size() > 0)
    {
      double angle = this->angleList->front();
      {
        boost::mutex::scoped_lock lock(this->mutex);
        this->angleList->pop();
      }
      writeAngleToFile(this->getFileName().c_str(), angle);
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(sleepAmount));
  }
}
