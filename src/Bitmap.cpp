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
Bitmap::Bitmap (const Bitmap& copy_from_me){
	//Clean up old bitmap
	if(!isEmpty()){
		delete [] contents;
	}

	//Store new values
	width = copy_from_me.width;
	height = copy_from_me.height;
	depth = copy_from_me.depth;

	//Create new bitmap
	int tmpSize = size();//Avoid repeated calls to size
	contents = new unsigned char[tmpSize];

	//Copy contents across into this bitmap
	for(int i=; i<size(); ++i)
		contents[i] = copy_from_me[i];
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

	//Copy contents across into this bitmap
	for(int i=; i<size(); ++i)
		contents[i] = rhs[i];

	return *this;
}


/** Returns value of corresponding pixel in the image */
unsigned char getPixel(unsigned x, unsigned y) {
	if( (x >= 0 && x < width) && (y >= 0 && y < height))
		return *(contents + (width * y) + x);
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
