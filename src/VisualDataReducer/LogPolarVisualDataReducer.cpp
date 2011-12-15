#include <iSpike/VisualDataReducer/LogPolarVisualDataReducer.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/Common.hpp>
#include <iSpike/ISpikeException.hpp>
using namespace ispike;

#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

#include <boost/math/special_functions/round.hpp>

//Pi
#define PI 3.141592653589793238

/** Ouputs debug images and coordinates to file */
//#define DEBUG_IMAGES
//#define DEBUG_COORDINATES


/** Constructor */
LogPolarVisualDataReducer::LogPolarVisualDataReducer() :
	inputWidth(0),
	inputHeight(0),
	outputWidth(0),
	outputHeight(0),
	foveaRadius(0.0),
	initialized(false)
{
	;
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
	if(bitmap.isEmpty()) {
		LOG(LOG_DEBUG)<<"LogPolarVisualDataReducer: Empty bitmap";
		return;
	}

	//Prepare for first use if necessary
	if(!isInitialized()){
		initialize(bitmap);
	}

	//Check bitmap is correct size
	if(bitmap.getWidth() != inputWidth || bitmap.getHeight() != inputHeight) {
		throw ISpikeException("LogPolarVisualDataReducer: Incoming image size has changed.");
	}

	//Calculate the log polar foveated image
	calculateReducedImage(bitmap);
}


/** Sets the input width, throws exception if class has been initialized. */
void LogPolarVisualDataReducer::setOutputHeight(unsigned outputHeight){
	if(isInitialized()) {
		throw ISpikeException("LogPolarVisualDataReducer: Output height cannot be set after class has been initialized.");
	}
	this->outputHeight = outputHeight;
}


/** Sets the output width, throws exception if class has been initialized */
void LogPolarVisualDataReducer::setOutputWidth(unsigned outputWidth){
	if(isInitialized()) {
		throw ISpikeException("LogPolarVisualDataReducer: Output width cannot be set after class has been initialized.");
	}
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
	if(bitmap.getDepth() != reducedImage->getDepth()) {
		throw ISpikeException("Depth in input image and reduced image do not match");
	}
	int tmpDepth = bitmap.getDepth();

	//References to arrays inside both images
	unsigned char* inputImageArray = bitmap.getContents();
	unsigned char* reducedImageArray = reducedImage->getContents();

	LOG(LOG_DEBUG)<<"Input image size: "<<bitmap.size()<<"; reduced image size: "<<reducedImage->size();

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
				//LOG(LOG_DEBUG)<<"Input image index: "<<(inputWidth * (iter->y) * tmpDepth + (iter->x) * tmpDepth + d)<<"; reducedImageIndex: "<<( outputWidth * (iter->theta) * tmpDepth + (iter->radius) * tmpDepth + d );
				reducedImageArray[ outputWidth * (iter->theta) * tmpDepth + (iter->radius) * tmpDepth + d ] =
					inputImageArray[ inputWidth * (iter->y) * tmpDepth + (iter->x) * tmpDepth + d ];
			}
		}
	}

	//Output reduced image if required
	#ifdef DEBUG_IMAGES
		Common::savePPMImage("inputImage.ppm", &bitmap);
		Common::savePPMImage("logPolar.ppm", reducedImage.get());
	#endif//DEBUG_IMAGES
}


/** Checks that parameters are sensible and initializes the class */
void LogPolarVisualDataReducer::initialize(Bitmap& bitmap){
	if(outputWidth == 0 || outputHeight == 0) {
		throw ISpikeException("LogPolarVisualDataReducer: Cannot initialize with zero width and/or height.");
	}

	if(foveaRadius > outputWidth) {
		throw ISpikeException("LogPolarVisualDataReducer: Fovea radius must be less than or equal to output width.");
	}

	if(bitmap.getWidth() < outputWidth || bitmap.getHeight() < outputWidth) {
		throw ISpikeException("LogPolarVisualDataReducer: Incoming image must be greater in size than output.");
	}

	//Store input width and height
	inputWidth = bitmap.getWidth();
	inputHeight = bitmap.getHeight();
	if(foveaRadius > inputWidth/2 || foveaRadius > inputHeight/2) {
		throw ISpikeException("LogPolarVisualDataReducer: Fovea radius must be less than or equal to half of input width or height.");
	}

	//Initialize map for converting between polar and Cartesian
	initialisePolarToCartesianVector();

	//Create reduced image
	reducedImage.reset(new Bitmap(outputWidth, outputHeight, bitmap.getDepth()));

	initialized = true;
}


/** Initialises a pixel map from polar to Cartesian coordinates.
	This will enable us to move through points in the output image and take samples
	at appropriate points in the input image. */
void LogPolarVisualDataReducer::initialisePolarToCartesianVector(){
	coordinatesVector.clear();
	#ifdef DEBUG_COORDINATES
		ofstream fileStream;
		fileStream.open("LogPolarVisualDataReducer.log", fstream::out);
	#endif//DEBUG_COORDINATES

	//Map between a location on the log polar map and an angle on input map
	double angleResolution = 360.0 / outputHeight;

	//Get appropriate base for logarithm
	double inputRadius = inputWidth/2.0;
	if(inputWidth>inputHeight) {
		inputRadius = inputHeight/2.0;//Minimum - can only sample a circular pattern in input
	}

	//Radius lengths outside foveated area
	double outputLogRadius = outputWidth - foveaRadius;
	double inputLogRadius = inputRadius - foveaRadius;

	//Find the base such that the maximum value will lie inside the inputRadius
	double expBase = pow(10.0, log10(inputLogRadius) / outputLogRadius );

	LOG(LOG_DEBUG)<<"OutputLogRadius: "<<outputLogRadius<<"; InputLogRadius: "<<inputLogRadius<<"; ExpBase: "<<expBase;

	for(unsigned r=0; r<outputWidth; ++r){
		for(unsigned theta=0; theta<outputHeight; ++theta){
			if(r <= foveaRadius){
				pair<int, int> tmpPair = getInputCartesianCoordinate(r, theta*angleResolution);
				coordinatesVector.push_back(PolarCartCoords(r, theta, tmpPair.first, tmpPair.second));
				#ifdef DEBUG_COORDINATES
					fileStream<<"Polar("<<r<<", "<<theta<<") -> Cart("<<tmpPair.first<<", "<<tmpPair.second<<")"<<endl;
				#endif//DEBUG_COORDINATES
			} else {
				pair<int, int> tmpPair = getInputCartesianCoordinate(foveaRadius + pow(expBase, r-foveaRadius), theta*angleResolution);
				coordinatesVector.push_back(PolarCartCoords(r, theta, tmpPair.first, tmpPair.second));
				#ifdef DEBUG_COORDINATES
					fileStream<<"Polar("<<r<<", "<<theta<<") -> Cart("<<tmpPair.first<<", "<<tmpPair.second<<")"<<endl;
				#endif//DEBUG_COORDINATES
			}
		}
	}
	#ifdef DEBUG_COORDINATES
		fileStream.close();
	#endif//DEBUG_COORDINATES
}


/** Gets the nearest Cartesian coordinate in the input image.
	X and Y coordinates range from 0 to 2*radius
	theta is in degrees */
pair<int, int> LogPolarVisualDataReducer::getInputCartesianCoordinate(double radius, double theta_deg){
	double theta_rads = (theta_deg / 360.0) * 2.0 * PI;
	int tmpY = boost::math::iround(double(inputHeight/2) + radius * sin(theta_rads));
	if(tmpY >= int(inputHeight)) {
		throw ISpikeException("LogPolarVisualDataReducer: Y out of range: ", tmpY);
	}
	int tmpX = boost::math::iround(double(inputWidth/2) + radius * cos(theta_rads));
	if(tmpX >= int(inputWidth)) {
		throw ISpikeException("LogPolarVisualDataReducer: X out of range: ", tmpX);
	}
	return make_pair(tmpX, tmpY);
}


