#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/Bitmap.hpp>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/mpl/vector.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/ISpikeException.hpp>

Bitmap FileVisualReader::getData()
{
  return *(this->buffer);
}

void FileVisualReader::start()
{
  if(!initialised)
  {
    initialised = true;
  }
}

Bitmap* readPPMImage(const char* fname)
{
 int N, M;
 char header [100], *ptr;
 std::ifstream ifp;

 ifp.open(fname, std::ios::in | std::ios::binary);

 if (!ifp) {
   std::ostringstream messageStream;
   messageStream << "Can't read image: " << fname;
   std::string message(messageStream.str());
   throw ISpikeException(message);
 }

 // read header

 ifp.getline(header,100,'\n');

 if ( (header[0]!=80) ||    /* 'P' */
      (header[1]!=54) ) {   /* '6' */
   std::ostringstream messageStream;
   messageStream << "Image " << fname << " is not PPM";
   std::string message(messageStream.str());
   throw ISpikeException(message);
 }

 ifp.getline(header,100,'\n');
 while(header[0]=='#')
   ifp.getline(header,100,'\n');

 M=strtol(header,&ptr,0);
 N=atoi(ptr);

 std::cout << "Width: " << M << " Height: " << N << std::endl;

 ifp.getline(header,100,'\n');

 unsigned char *charImage = new unsigned char [3*M*N];

 ifp.read((char*)charImage, 3*M*N);


 if (ifp.fail()) {
   std::ostringstream messageStream;
   messageStream << "Image " << fname << " has wrong size";
   std::string message(messageStream.str());
   throw ISpikeException(message);
 }

 ifp.close();

 return new Bitmap(M, N, 3, charImage);

}

void FileVisualReader::initialise(std::map<std::string,Property*> properties)
{
  std::string filename = ((StringProperty*)(properties["Filename"]))->getValue();
	this->buffer = readPPMImage(filename.c_str());
}
