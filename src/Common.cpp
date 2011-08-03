//iSpike includes
#include <iSpike/Common.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/ISpikeException.hpp>
#include <boost/lexical_cast.hpp>
using namespace ispike;

//Other includes
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <ios>
#include <vector>
#include <algorithm>


/** Writes the supplied image to file */
void Common::savePPMImage(const char* filename, Bitmap* image){
  std::ofstream file_handle(filename, std::ios::binary);
  if (file_handle) {
    file_handle << "P6" << std::endl << image->getWidth() << ' ' << image->getHeight() << std::endl << 255 << std::endl;
    file_handle.write((char *)image->getContents(), image->getWidth() * image->getHeight() * image->getDepth());
    file_handle.close();
  }
}


/** Writes a spike pattern to file */
void Common::writePatternToFile(const char* fileName, std::vector<int> pattern, int numOfNeurons){
 std::ofstream fileStream;

 fileStream.open(fileName, std::fstream::out | std::fstream::app);

 if (!fileStream) {
   std::ostringstream messageStream;
   messageStream << "Can't write angles: " << fileName;
   std::string message(messageStream.str());
   throw ISpikeException(message);
 }

 //fileStream << boost::lexical_cast<std::string>(angle) << std::endl;
 for( int i = 0; i < numOfNeurons; i++ ) {
   if(std::find(pattern.begin(), pattern.end(), i) != pattern.end()) {
     fileStream << "1,";
   }
   else {
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
