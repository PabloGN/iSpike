/*
 * Common.cpp
 *
 *  Created on: 7 Feb 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/Common.hpp>
#include <iSpike/Bitmap.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <ios>
#include <vector>
#include <algorithm>
#include <iSpike/ISpikeException.hpp>

void Common::savePPMImage(char* filename, Bitmap* image)
{
  std::ofstream file_handle(filename, std::ios::binary);
  if (file_handle)
  {
    file_handle << "P6" << std::endl << image->getWidth() << ' ' << image->getHeight() << std::endl << 255 << std::endl;
    file_handle.write((char *)image->getContents(), image->getWidth() * image->getHeight() * image->getDepth());
    file_handle.close();
  }
}

Bitmap* Common::produceGrayscale(unsigned char* image, int width, int height)
{
  unsigned char* resultBuffer = new unsigned char[width * height * 3];
  Bitmap* result = new Bitmap(width, height, 3, resultBuffer);
  for(int i = 0; i < width * height; i++)
  {
    unsigned char resultPixel[3] = {*(image + i), *(image + i), *(image + i)};
    //*(result->getContents() + (i * 3)) = resultPixel;
    memcpy(result->getContents() + (i * 3), resultPixel, 3);
  }
  return result;
}

unsigned char* Common::normaliseImage(unsigned char* image, int size)
{
  unsigned char max = 0;
  unsigned char* result = new unsigned char[size];
  for(int i = 0; i < size; i++)
  {
    if(image[i] > max)
      max = image[i];
    if(max == 255)
      break;
  }
  if(max == 0)
	  return image;
  for(int i = 0; i < size; i++)
    result[i] = image[i] * 255 / max;
  return result;
}

std::map<std::string, Property*> Common::getProperties(std::map<std::string,Property*> defaultProperties)
{
  std::map<std::string, Property*> resultProperties;
  for(std::map<std::string,Property*>::iterator iter = defaultProperties.begin(); iter != defaultProperties.end(); ++iter)
  {
    std::cout << iter->second->getName();
    if(iter->second->getType() == Property::Double)
    {
      double defaultValue = ((DoubleProperty*)(iter->second))->getValue();
      std::cout << "(" << defaultValue << "):" << std::endl;
      std::string readValue;
      double parameterValue;
      std::getline(std::cin,readValue);
      if(readValue == "")
      {
        parameterValue = defaultValue;
      } else {
        parameterValue = boost::lexical_cast<double>(readValue);
      }
      resultProperties[iter->second->getName()] = new DoubleProperty(
          iter->second->getName(),
          parameterValue,
          iter->second->getDescription()
        );
    } else if(iter->second->getType() == Property::Integer)
    {
      int defaultValue = ((IntegerProperty*)(iter->second))->getValue();
      std::cout << "(" << defaultValue << "):" << std::endl;
      std::string readValue;
      int parameterValue;
      std::getline(std::cin,readValue);
      if(readValue == "")
      {
        parameterValue = defaultValue;
      } else {
        parameterValue = boost::lexical_cast<int>(readValue);
      }
      resultProperties[iter->second->getName()] = new IntegerProperty(
          iter->second->getName(),
          parameterValue,
          iter->second->getDescription()
        );
    } else if(iter->second->getType() == Property::String)
    {
      std::string defaultValue = ((StringProperty*)(iter->second))->getValue();
      std::cout << "(" << defaultValue << "):" << std::endl;
      std::string readValue;
      std::string parameterValue;
      std::getline(std::cin,readValue);
      if(readValue == "")
      {
        parameterValue = defaultValue;
      } else {
        parameterValue = readValue;
      }
      resultProperties[iter->second->getName()] = new StringProperty(
          iter->second->getName(),
          parameterValue,
          iter->second->getDescription()
        );
    }
  }
  return resultProperties;
}

void Common::writePatternToFile(const char* fileName, std::vector<int> pattern, int numOfNeurons)
{
 std::ofstream fileStream;

 fileStream.open(fileName, std::fstream::out | std::fstream::app);

 if (!fileStream) {
   std::ostringstream messageStream;
   messageStream << "Can't write angles: " << fileName;
   std::string message(messageStream.str());
   throw ISpikeException(message);
 }

 //fileStream << boost::lexical_cast<std::string>(angle) << std::endl;
 for( int i = 0; i < numOfNeurons; i++ )
 {
   if(std::find(pattern.begin(), pattern.end(), i) != pattern.end())
   {
     fileStream << "1,";
   } else {
     fileStream << "0,";
   }
 }
 fileStream << std::endl;

 if (fileStream.fail()) {
   std::ostringstream messageStream;
   messageStream << "Can't write angles: " << fileName;
   std::string message(messageStream.str());
   throw ISpikeException(message);
 }

 fileStream.close();

}
