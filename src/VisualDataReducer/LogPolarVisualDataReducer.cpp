//iSpike includes
#include <iSpike/VisualDataReducer/LogPolarVisualDataReducer.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/Common.hpp>
#include "iSpike/ISpikeException.hpp"
using namespace ispike;

//Other includes
#include <iostream>
#include <sstream>
using namespace std;


/** Ouputs debug images to file */
#define DEBUG_IMAGES

/** Constructor */
LogPolarVisualDataReducer::LogPolarVisualDataReducer(){
	//Initialize variables
	reducedImage = NULL;
	setInitialized(false);
}


/** Destructor */
LogPolarVisualDataReducer::~LogPolarVisualDataReducer(){
	if(isInitialized())
		delete reducedImage;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Returns a reference to the reduced image */
Bitmap& LogPolarVisualDataReducer::getReducedImage(){
	return *reducedImage;
}


/** Sets the bitmap and recalculates the new reduced image */
void LogPolarVisualDataReducer::setBitmap(Bitmap& bitmap){
	if(bitmap.isEmpty()){
		LOG(LOG_DEBUG)<<"LogPolarVisualDataReducer: Empty bitmap";
		return;
	}

	//Prepare for first use if necessary
	if(!isInitialized()){
		initialize(bitmap);
	}

	//Check bitmap is correct size
	if(bitmap.getWidth() != inputWidth || bitmap.getHeight() != inputHeight)
		throw ISpikeException("LogPolarVisualDataReducer: Incoming image size has changed.");

	//Calculate the log polar foveated image
	calculateReducedImage(bitmap);
}


/** Sets the input width, throws exception if class has been initialized. */
void LogPolarVisualDataReducer::setOutputHeight(int outputHeight){
	if(isInitialized())
		throw ISpikeException("LogPolarVisualDataReducer: Output height cannot be set after class has been initialized.");
	this->outputHeight = outputHeight;
}


/** Sets the output width, throws exception if class has been initialized */
void LogPolarVisualDataReducer::setOutputWidth(int outputWidth){
	if(isInitialized())
		throw ISpikeException("LogPolarVisualDataReducer: Output width cannot be set after class has been initialized.");
	this->outputWidth = outputWidth;
}


/** Sets fovea radius, throws exception if class has been initialized */
void LogPolarVisualDataReducer::setFoveaRadius(double foveaRadius){
	if(isInitialized())
		throw ISpikeException("LogPolarVisualDataReducer: Fovea radius cannot be set after class has been initialized.");
	this->foveaRadius = foveaRadius;
}


/*--------------------------------------------------------------------*/
/*---------                 PRIVATE METHODS                    -------*/
/*--------------------------------------------------------------------*/

/** Calculates the log polar reduced image from the bitmap */
void LogPolarVisualDataReducer::calculateReducedImage(Bitmap& bitmap){
	if(bitmap.getDepth() != reducedImage->getDepth())
		throw ISpikeException("Depth in input image and reduced image do not match");
	int tmpDepth = bitmap.getDepth();

	//References to arrays inside both images
	unsigned char* inputImageArray = bitmap.getContents();
	unsigned char* reducedImageArray = reducedImage->getContents();

	//Work through coordinates in log polar output map
	for(int r=0; r<outputWidth; ++r){
		for(int theta=0; theta<outputHeight; ++theta){
		}
	}

	//Copy the pixels across from the input to the output maps
	for(vector<PolarCartCoords>::iterator iter = coordinatesVector.begin(); iter != coordinatesVector.end(); ++iter){
		for(int d=0; d<tmpDepth; ++d){
			//Copy pixel data into reduced image array
			//First coordinate is the polar(r, theta) in the output bitmap; second coordinate is the Cartesian(x,y) in the input bitmap
			if(tmpDepth == 0){
				reducedImageArray[ outputWidth * (iter->theta) + (iter->radius)] =
					inputImageArray[ inputWidth * (iter->y) + (iter->x) ];
			}
			else{
				reducedImageArray[ outputWidth * (iter->theta) * tmpDepth + (iter->radius) * tmpDepth + d ] =
					inputImageArray[ inputWidth * (iter->y) * tmpDepth + (iter->x) * tmpDepth + d ];
			}
		}
	}

	//Output debug image if required
	#ifdef DEBUG_IMAGES
		Common::savePPMImage("logPolar.ppm", reducedImage);
	#endif//DEBUG_IMAGES
}


/** Checks that parameters are sensible and initializes the class */
void LogPolarVisualDataReducer::initialize(Bitmap& bitmap){
	if(outputWidth == 0 || outputHeight == 0)
		throw ISpikeException("LogPolarVisualDataReducer: Cannot initialize with zero width and/or height.");

	if(foveaRadius > outputWidth || foveaRadius > outputHeight)
		throw ISpikeException("LogPolarVisualDataReducer: Fovea radius must be less than or equal to output width and height.");

	if(bitmap.getWidth() <= outputWidth || bitmap.getHeight() <= outputHeight)
		throw ISpikeException("LogPolarVisualDataReducer: Incoming image must be greater in size than output.");

	inputWidth = bitmap.getWidth();
	inputHeight = bitmap.getHeight();
	initialisePolarToCartesianVector();
	reducedImage = new Bitmap(outputWidth, outputHeight, bitmap.getDepth());
	setInitialized(true);
}


/** Initialises a pixel map from polar to Cartesian coordinates.
	This will enable us to move through points in the output image and take samples
	at appropriate points in the input image. */
void LogPolarVisualDataReducer::initialisePolarToCartesianVector(){
	coordinatesVector.clear();

	//Map between a location on the log polar map and an angle on input map
	double angleResolution = 360.0 / outputHeight;

	//Get appropriate base for logarithm
	double inputRadius = inputWidth/2.0;
	if(inputWidth>inputHeight)
		inputRadius = inputHeight/2.0;//Minimum - can only sample a circular pattern in input

	//Radius lengths outside foveated area
	double outputLogRadius = outputWidth - foveaRadius;
	double inputLogRadius = inputRadius - foveaRadius;

	//Find the base such that the maximum value will lie inside the inputRadius
	double logBase = pow(inputLogRadius, outputLogRadius);

	for(int r=0; r<outputWidth; ++r){
		for(int theta=0; theta<outputHeight; ++theta){
			if(r <= foveaRadius){
				pair<int, int> tmpPair = getInputCartesianCoordinate(r, theta*angleResolution);
				coordinatesVector.push_back(PolarCartCoords(r, theta, tmpPair.first, tmpPair.second));
			}
			else{
				pair<int, int> tmpPair = getInputCartesianCoordinate(foveaRadius + pow(logBase, r-foveaRadius), theta*angleResolution);
				coordinatesVector.push_back(PolarCartCoords(r, theta, tmpPair.first, tmpPair.second));
			}
		}
	}
}


/** Gets the nearest Cartesian coordinate in the input image */
pair<int, int> LogPolarVisualDataReducer::getInputCartesianCoordinate(double radius, double theta){
	int tmpY = nearbyint(radius * sin(theta));
	if(tmpY >= inputHeight)
		throw ISpikeException("LogPolarVisualDataReducer: Y out of range.");
	int tmpX = nearbyint(radius * cos(theta));
	if(tmpX >= inputWidth)
		throw ISpikeException("LogPolarVisualDataReducer: X out of range.");
	return make_pair(tmpX, tmpY);
}


