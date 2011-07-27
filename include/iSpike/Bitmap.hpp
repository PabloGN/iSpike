#ifndef BITMAP_HPP_
#define BITMAP_HPP_

//Other includes
#include <string>
using namespace std;

namespace ispike {

	/** Represents a bitmap representation of an image or just
	 * any sequence of bytes, has a height, width and depth  */
	class Bitmap {
		public:
			Bitmap(unsigned width, unsigned height, unsigned depth) ;
			Bitmap (const Bitmap& copy_from_me);
			~Bitmap();
			Bitmap& operator=(const Bitmap& rhs);
			unsigned char* getContents() const  { return contents;  }
			unsigned char getPixel(unsigned x, unsigned y) ;
			unsigned getDepth() const { return depth; }
			unsigned getHeight() const { return height; }
			unsigned getWidth() const { return width; }
			bool isEmpty() { if (size() > 0) return true; return false; }
			void reset(unsigned width, unsigned height, unsigned depth);

		private:
			//========================  VARIABLES  ========================
			unsigned width;
			unsigned height;
			unsigned depth;
			unsigned char* contents;

			//======================  METHODS  ============================
			int size() { return width*height*depth; }

	};

}

#endif /* BITMAP_HPP_ */
