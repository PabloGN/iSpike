//iSpike includes
#include "iSpike/Bitmap.hpp"
using namespace ispike;

/** Default constructor */
Bitmap::Bitmap(){
	width = 0;
	height = 0;
	depth = 0;
	contents = NULL;
}


/** Constructor */
Bitmap::Bitmap(unsigned width, unsigned height, unsigned depth) {
	this->width = width;
	this->height = height;
	this->depth = depth;
	contents = new unsigned char[width * height * depth];
}


/** Copy constructor */
Bitmap::Bitmap (const Bitmap& bmp){
	//Clean up old bitmap
	if(!isEmpty()){
		delete [] contents;
	}

	//Store new values
	width = bmp.width;
	height = bmp.height;
	depth = bmp.depth;

	//Create new bitmap
	int tmpSize = size();//Avoid repeated calls to size
	contents = new unsigned char[tmpSize];
	unsigned char* bmpContents = bmp.getContents();

	//Copy contents across into this bitmap
	for(int i=0; i<tmpSize; ++i)
		contents[i] = bmpContents[i];
}


/** Destructor */
Bitmap::~Bitmap(){
	if(!isEmpty())
		delete contents;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Assignment operator */
Bitmap& Bitmap::operator=(const Bitmap& rhs){
	if(this == &rhs)
		return *this;

	//Clean up old bitmap
	if(!isEmpty()){
		delete [] contents;
	}

	//Store new values
	width = rhs.width;
	height = rhs.height;
	depth = rhs.depth;

	//Create new bitmap
	int tmpSize = size();//Avoid repeated calls to size
	contents = new unsigned char[tmpSize];
	unsigned char* rhsContents = rhs.getContents();

	//Copy contents across into this bitmap
	for(int i=0; i<tmpSize; ++i)
		contents[i] = rhsContents[i];

	return *this;
}


/** Returns value of corresponding pixel in the image */
unsigned char Bitmap::getPixel(unsigned x, unsigned y, unsigned d) {
	if( (x >= 0 && x < width) && (y >= 0 && y < height) && (d >=0 && d < depth)){
		if(depth ==0)
			return *(contents + (width * y) + x);
		return *(contents + (width * depth * y) + x * depth + d);
	}
}


/** Reconstructs bitmap using the new parameters */
void Bitmap::reset(unsigned width, unsigned height, unsigned depth){
	//Clean up old bitmap
	if(!isEmpty()){
		delete [] contents;
	}

	this->width = width;
	this->height = height;
	this->depth = depth;

	contents = new unsigned char[size()];
}
