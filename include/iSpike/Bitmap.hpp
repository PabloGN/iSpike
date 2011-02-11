/*
 * Bitmap.hpp
 *
 *  Created on: 6 Jan 2011
 *      Author: cembo
 */

#ifndef BITMAP_HPP_
#define BITMAP_HPP_

#include <string>

class Bitmap
{
  private:
    int width;
    int height;
    int depth;
    unsigned char* contents;

  public:
    Bitmap(int width, int height, int depth, unsigned char* contents)
    {
      this->width = width;
      this->height = height;
      this->depth = depth;
      this->contents = contents;
    }

    Bitmap (const Bitmap& copy_from_me)
    {
      this->width = copy_from_me.width;
      this->height = copy_from_me.height;
      this->depth = copy_from_me.depth;
      int content_size = copy_from_me.width * copy_from_me.height * copy_from_me.depth;
      this->contents = new unsigned char[content_size];
      std::copy(copy_from_me.contents, copy_from_me.contents + content_size, this->contents);
    }

    ~Bitmap(){
      delete this->contents;
    }

    unsigned char *getContents() const
    {
        return contents;
    }

    int getDepth() const
    {
        return depth;
    }

    int getHeight() const
    {
        return height;
    }

    int getWidth() const
    {
        return width;
    }

    void setContents(unsigned char *contents)
    {
        this->contents = contents;
    }

    void setDepth(int depth)
    {
        this->depth = depth;
    }

    void setHeight(int height)
    {
        this->height = height;
    }

    void setWidth(int width)
    {
        this->width = width;
    }

};


#endif /* BITMAP_HPP_ */