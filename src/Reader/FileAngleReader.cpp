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
#include <iSpike/Property.hpp>
#include <iSpike/ISpikeException.hpp>

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
   std::ostringstream messageStream;
   messageStream << "Can't read angles: " << fileName;
   std::string message(messageStream.str());
   throw ISpikeException(message);
 }
 fileStream >> contents;

 std::cout << contents;

 if (fileStream.fail()) {
   std::ostringstream messageStream;
   messageStream << "Can't read angles: " << fileName;
   std::string message(messageStream.str());
   throw ISpikeException(message);
 }

 fileStream.close();

 std::vector<double> *angles = new std::vector<double>();
 boost::regex split_string(" ");
 std::list<std::string> lines;
 boost::regex_split(std::back_inserter(lines), contents, split_string);
 while(lines.size() > 0)
 {
   std::string current_string = *(lines.begin());
   lines.pop_front();
   try
   {
     double angle = boost::lexical_cast<double>(current_string);
     angles->push_back(angle);
   }
   catch(boost::bad_lexical_cast &)
   {
     std::cout << "could not convert " << current_string << " to double" << std::endl;
     break;
   }
 }

 return angles;

}

void FileAngleReader::start()
{
  if(!initialised)
  {
    initialised = true;
    std::cout << "Reading angles from: " << fileName << std::endl;
    this->buffer = readAngleFromFile(fileName.c_str());
  }
}

void FileAngleReader::initialise(std::map<std::string,Property*> properties)
{
	this->buffer = new std::vector<double>();
	this->fileName = ((StringProperty*)(properties["Filename"]))->getValue();
}
