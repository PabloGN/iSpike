/*
 * FileAngleReader.cpp
 *
 *  Created on: 9 Mar 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/Reader/FileAngleReader.hpp>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

std::vector<double> FileAngleReader::getData()
{
  return *(this->buffer);
}

std::vector<double>* readAngleFromFile(const char* fileName)
{
 std::ifstream fileStream;
 std::string contents;

 fileStream.open(fileName, std::ios::in | std::ios::binary);

 if (!fileStream) {
   std::cout << "Can't read angles: " << fileName << std::endl;
   exit(1);
 }
 fileStream >> contents;

 std::cout << contents;

 if (fileStream.fail()) {
   std::cout << "Can't read angles: " << fileName << std::endl;
   exit(1);
 }

 fileStream.close();

 return new std::vector<double>();

}

void FileAngleReader::start()
{
  if(!initialised)
  {
    initialised = true;
    this->buffer = readAngleFromFile(fileName.c_str());
  }
}

FileAngleReader::FileAngleReader(std::string fileName)
{
	this->buffer = new std::vector<double>();
}
