//iSpike includes
#include <iSpike/VisualDataReducer/LogpolarVisualDataReducer.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/Common.hpp>
using namespace ispike;

//Other includes
#include <iostream>
#include <sstream>
#include <boost/math/constants/constants.hpp>


/** Ouputs debug images to file */
#define DEBUG_IMAGES

/** Constructor */
LogPolarVisualDataReducer::LogPolarVisualDataReducer(int outputWidth, int outputHeight, double foveaRadius){
	this->outputWidth = outputWidth;
	this->outputHeight = outputHeight;
	this->foveaRadius = foveaRadius;

	//Initialize variables
	mapsInitialized = false;
}


/** Destructor */
LogPolarVisualDataReducer::~LogPolarVisualDataReducer(){
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

	if(!mapsInitialized){
		inputWidth = bitmap.getWidth();
		inputHeight = bitmap.getHeight();
		initialiseCartesianToPolarMap(bitmap);
		reducedImage = new Bitmap(outputWidth, outputHeight, bitmap.getDepth());
		mapsInitialized = true;
	}
	calculateReducedImage(bitmap);
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

	//Work through Cartesian to polar map.
	for(CoordMapType::iterator iter = polarToCartesianMap.begin(); iter != polarToCartesianMap.end(); ++iter){
		for(int d=0; d<tmpDepth; ++d){
			//Copy pixel data into reduced image array
			reducedImageArray[outputWidth*iter->second.second + iter->second.first + d] = inputImageArray[inputWidth*iter->first.second + iter->first.first + d];
		}
	}

	//Output debug image if required
	#ifdef DEBUG_IMAGES
		Common::savePPMImage("logPolar.ppm", reducedImage);
	#endif//DEBUG_IMAGES
}


/** Returns the distance between two points */
double LogPolarVisualDataReducer::getDistance(int x1, int y1, double x2, double y2){
	return sqrt(pow(x1 - x2,2) + pow(y1 - y2,2));
}


/** Initialises a pixel map from cartesian to polar coordinates */
CoordMapType* LogPolarVisualDataReducer::initialiseCartesianToPolarMap(Bitmap& image){
	//Only process a circular area in input image
	double inputRadius = inputWidth/2.0;
	if(inputWidth>inputHeight)
		inputRadius = inputHeight/2.0;

	//Interval at which we sample the angle
	double thetaInterval = 360.0 / outputHeight;

	//Build a map that samples points in the input image and maps them to polar coordinates in the output image
	for(double theta=0.0; theta<360.0; theta += thetaInterval){//Work through all the angles
		for(double radius=0; radius<inputRadius; radius += 1.0){//Work along each radius
			//Sample all points within the fovea radius, otherwise log the radius
			tmpRadius = radius;
			if(radius > foveaRadius)
				tmpRadius = log10(radius);

			//Round and add to map - duplicates should be filtered by the map
			polarToCartesianMap->insert(getNearestInputCartesianCoordinate(tmpRadius, theta), getNearestOutputPolarCoordinate(tmpRadius, theta));
		}
	}
}


/** Gets the nearest Cartesian coordinate in the input image */
pair<int, int> LogPolarVisualDataReducer::getNearestInputCartesianCoordinate(double radius, double theta){
	int tmpY = nearbyint(radius * sin(theta));
	if(tmpY >= inputHeight)
		throw ISpikeException("LogPolarVisualDataReducer: Y out of range.");
	int tmpX = nearbyint(radius * cos(theta));
	if(tmpX >= inputWidth)
		throw ISpikeException("LogPolarVisualDataReducer: Y out of range.");
	return make_pair(tmpX, tmpY);
}


/** Gets the nearest Polar coordinate in the output image */
pair<int, int> LogPolarVisualDataReducer::getNearestOutputPolarCoordinate(double radius, double theta){
	int tmpRadius = nearbyint(radius);
	if(tmpRadius >= outputWidth)
		throw ISpikeException("LogPolarVisualDataReducer: radius out of range.");
	int tmpTheta = nearbyint(theta);
	if(tmpTheta >= outputHeight)
		throw ISpikeException("LogPolarVisualDataReducer: theta out of range.");
	return make_pair(tmpRadius, tmpTheta);
}

