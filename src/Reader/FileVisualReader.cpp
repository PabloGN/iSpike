#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/Bitmap.hpp>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/mpl/vector.hpp>
#include <iSpike/YarpConnection.hpp>

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
 int N, M, Q;
 char header [100], *ptr;
 std::ifstream ifp;

 ifp.open(fname, std::ios::in | std::ios::binary);

 if (!ifp) {
   std::cout << "Can't read image: " << fname << std::endl;
   exit(1);
 }

 // read header

 ifp.getline(header,100,'\n');

 if ( (header[0]!=80) ||    /* 'P' */
      (header[1]!=54) ) {   /* '6' */
      std::cout << "Image " << fname << " is not PPM" << std::endl;
      exit(1);
 }

 ifp.getline(header,100,'\n');
 while(header[0]=='#')
   ifp.getline(header,100,'\n');

 M=strtol(header,&ptr,0);
 N=atoi(ptr);

 std::cout << "Width: " << M << " Height: " << N << std::endl;

 ifp.getline(header,100,'\n');
 Q=strtol(header,&ptr,0);

 unsigned char *charImage = new unsigned char [3*M*N];

 ifp.read((char*)charImage, 3*M*N);


 if (ifp.fail()) {
   std::cout << "Image " << fname << " has wrong size" << std::endl;
   exit(1);
 }

 ifp.close();

 return new Bitmap(M, N, 3, charImage);

}

void FileVisualReader::initialise(std::map<std::string,Property*> properties)
{
  std::string filename = ((StringProperty*)(properties["Filename"]))->getValue();
	this->buffer = readPPMImage(filename.c_str());
}
