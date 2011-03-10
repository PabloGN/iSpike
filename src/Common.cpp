/*
 * Common.cpp
 *
 *  Created on: 7 Feb 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/Common.hpp>
#include <iSpike/Bitmap.hpp>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <ios>

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
  for(int i = 0; i < size; i++)
    result[i] = image[i] * 255 / max;
  return result;
}
