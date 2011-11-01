#include <iSpike/VisualFilter/DOGVisualFilter.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Common.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;

#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/round.hpp>


/** Maximum value of a pixel */
#define MAX_PIXEL_VALUE 255


/** Create a new difference-of-gaussians visual filter
 *
 * The filter object does /not/ take ownership of the reducer object
 */
DOGVisualFilter::DOGVisualFilter(LogPolarVisualDataReducer* reducer) :
	reducer(reducer),
	positiveSigma(-1.0),
	negativeSigma(-1.0),
	positiveFactor(-1.0),
	negativeFactor(-1.0),
	opponencyTypeID(-1),
	normalize(false),
	initialized(false),
	outputBitmap(NULL),
	redBitmap(NULL),
	greenBitmap(NULL),
	blueBitmap(NULL),
	yellowBitmap(NULL),
	greyBitmap(NULL),
	positiveBlurredBitmap(NULL),
	negativeBlurredBitmap(NULL)
{
	;
}



DOGVisualFilter::~DOGVisualFilter(){
	if(isInitialized()){
		delete redBitmap;
		delete greenBitmap;
		delete blueBitmap;
		delete yellowBitmap;
		delete greyBitmap;
		delete positiveBlurredBitmap;
		delete negativeBlurredBitmap;
		delete outputBitmap;
	}
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Returns a reference to the opponency bitmap */
Bitmap& DOGVisualFilter::getBitmap(){
	return *outputBitmap;
}


/** Sets the opponency type ID. This can only be done when the class is NOT initialized */
void DOGVisualFilter::setOpponencyTypeID(int opponencyTypeID){
	if(isInitialized())
		throw ISpikeException("DOGVisualFilter: Opponency type ID cannot be set after class has been initialized.");
	this->opponencyTypeID = opponencyTypeID;
}


/**  Updates the opponency map calculated by this filter
 *  * Retrives a reduced image
 *  * Decomposes it into individual colour channels
 *  * Blurs each of these channels
 *  * Subtracts the channels one from another
 *  * Normalises the resultant images
 *  * Stores each image in the appropriate buffer
 */
void DOGVisualFilter::update(){
	//Get log polar bitmap and check it is ok
	Bitmap& reducedImage = reducer->getReducedImage();
	if(reducedImage.isEmpty())	{
		LOG(LOG_DEBUG)<<"DOGVisualFilter: Empty bitmap";
		return;
	}

	//Create bitmaps if they have not been created already
	if(!isInitialized()){
		initialize(reducedImage.getWidth(), reducedImage.getHeight());
	}

	//Calculate opponency map
	if(opponencyTypeID == Common::redVsGreen){
		extractRedChannel(reducedImage);
		extractGreenChannel(reducedImage);
		calculateOpponency(*redBitmap, *greenBitmap);
	}
	else if(opponencyTypeID == Common::greenVsRed){
		extractRedChannel(reducedImage);
		extractGreenChannel(reducedImage);
		calculateOpponency(*greenBitmap, *redBitmap);
	}
	else if(opponencyTypeID == Common::blueVsYellow){
		extractRedChannel(reducedImage);
		extractGreenChannel(reducedImage);
		extractBlueChannel(reducedImage);
		extractYellowChannel();
		calculateOpponency(*blueBitmap, *yellowBitmap);
	}
	else if(opponencyTypeID == Common::greyVsGrey){
		extractGreyChannel(reducedImage);
		calculateOpponency(*greyBitmap, *greyBitmap);
	}
	else if(opponencyTypeID == Common::motionSensitive){
		extractGreyChannel(reducedImage);
		calculateLogDifference(*greyBitmap, *greyBitmap);//FIXME!!
	}
	else
		throw ISpikeException("Opponency type ID not recognized: ", opponencyTypeID);
}


/*--------------------------------------------------------------------*/
/*---------                 PRIVATE METHODS                    -------*/
/*--------------------------------------------------------------------*/

/*! Calculates the log of the difference between the two bitmaps */
void DOGVisualFilter::calculateLogDifference(Bitmap& bitmap1, Bitmap& bitmap2){

}


/** Calculate opponency image */
void DOGVisualFilter::calculateOpponency(Bitmap& bitmap1, Bitmap& bitmap2){
	//Blur the positive image
	gaussianBlur(bitmap1, *positiveBlurredBitmap, positiveSigma);
	#ifdef DEBUG_IMAGES
		Common::savePPMImage("positiveBlur.ppm", positiveBlurredBitmap);
	#endif//DEBUG_IMAGES

	//Blur the negative image
	gaussianBlur(bitmap2, *negativeBlurredBitmap, negativeSigma);
	#ifdef DEBUG_IMAGES
		Common::savePPMImage("negativeBlur.ppm", negativeBlurredBitmap);
	#endif//DEBUG_IMAGES

	//Subtract the negative from the positive image to get the opponency map
	subtractImages(*positiveBlurredBitmap, *positiveBlurredBitmap, *outputBitmap);

	//Normalize opponency map if required
	if(normalize)
		normalizeImage(*outputBitmap);

	//Output debug image if required
	#ifdef DEBUG_IMAGES
		if(opponencyTypeID == Common::redVsGreen)
			Common::savePPMImage("Red+Green-.ppm", outputBitmap);
		else if(opponencyTypeID == Common::greenVsRed)
			Common::savePPMImage("Green+Red-.ppm", outputBitmap);
		if(opponencyTypeID == Common::blueVsYellow)
			Common::savePPMImage("Blue+Yellow-.ppm", outputBitmap);
	#endif//DEBUG_IMAGES
}


/** Gaussian blurs an image */
void DOGVisualFilter::gaussianBlur(Bitmap& inputBitmap, Bitmap& resultBitmap, double sigma){
	if(inputBitmap.getWidth() != resultBitmap.getWidth() || inputBitmap.getHeight() != resultBitmap.getHeight())
		throw ISpikeException("DOGVisualFilter: Mismatch between input and results dimensions.");
	int width = inputBitmap.getWidth();
	int height = inputBitmap.getHeight();

	//Pointers to arrays in bitmaps
	unsigned char* inputContents = inputBitmap.getContents();
	unsigned char* resultContents = resultBitmap.getContents();

	//Create kernel
	double* kernel = new double[(int)ceil(6 * sigma) + 1];
	for(int i = 0; i < ceil( 6 * sigma ) + 1; i++){
		int x = i - int(ceil( 3 * sigma ));
		double k = exp( - ( (x * x) / ( 2 * sigma * sigma ) ) );
		kernel[i] = k / sqrt( 2 * boost::math::constants::pi<double>() * sigma * sigma );
	}

	//Iterate horizontally
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			double colour_value = 0;
			unsigned char current_colour, *sourcePtr;

			//iterate over the kernel
			for(int i = 0; i < ceil( 6 * sigma ) + 1; i++){
				int kernel_x = i - int(ceil( 3 * sigma ));
				if(x + kernel_x < 0 )
					continue;
				else if(x + kernel_x > width)
					continue;
				sourcePtr = inputContents + ( y * width ) + x;
				current_colour = (unsigned char) *(sourcePtr + kernel_x);
				colour_value += current_colour * kernel[i];
			}
			resultContents[y * width + x] = (int)floor( colour_value + 0.5 );
		}
	}

	//Iterate vertically
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			double colour_value = 0;
			unsigned char current_colour, *sourcePtr;

			//iterate over the kernel
			for(int i = 0; i < ceil( 6 * sigma ) + 1; i++){
				int kernel_y = i - int(ceil( 3 * sigma ));
				if(y + kernel_y <= 0 )
					continue;
				else if(y + kernel_y >= height)
					continue;
				sourcePtr = inputContents + ( y * width ) + x;
				current_colour = (unsigned char) *(sourcePtr + (kernel_y * width));
				colour_value += current_colour * kernel[i];
			}
			resultContents[y * width + x] += (int)floor( colour_value + 0.5 );
		}
	}

	//Clean up kernel memory
	free(kernel);
}


/** Extracts the red channel from a given image.
 Extracts the red information from each pixel in the incoming image, whose dimensions must match. */
void DOGVisualFilter::extractRedChannel(Bitmap& image){
	//Check image dimensions match
	if(image.getWidth() != redBitmap->getWidth() || image.getHeight() != redBitmap->getHeight())
		throw ISpikeException("DOGVisualFilter: Red image and incoming reduced image have different dimensions");

	//Check incoming image has depth 3
	if(image.getDepth() !=3)
		throw ISpikeException ("DOGVisualFilter: expecting full colour image for red extraction.");

	//Avoid multiple function calls
	int imageSize = redBitmap->size();
	unsigned char* redContents = redBitmap->getContents();
	unsigned char* imageContents = image.getContents();

	//Copy the red pixels
	for(int pixel = 0; pixel < imageSize; ++pixel) {
		redContents[pixel] = imageContents[pixel * 3];
	}

	//Output debug image if required
	#ifdef DEBUG_IMAGES
		Common::savePPMImage("red.ppm", redBitmap);
	#endif//DEBUG_IMAGES
}


/** Extracts the green channel from a given image.
 Extracts the green information from each pixel in the incoming image, whose dimensions must match. */
void DOGVisualFilter::extractGreenChannel(Bitmap& image){
	//Check image dimensions match
	if(image.getWidth() != greenBitmap->getWidth() || image.getHeight() != greenBitmap->getHeight())
		throw ISpikeException("DOGVisualFilter: Green image and incoming reduced image have different dimensions");

	//Avoid multiple function calls
	int imageSize = greenBitmap->size();
	unsigned char* greenContents = greenBitmap->getContents();
	unsigned char* imageContents = image.getContents();

	//Copy the green pixels
	for(int pixel = 0; pixel < imageSize; ++pixel) {
		greenContents[pixel] = imageContents[pixel * 3 + 1];
	}

	//Output debug image if required
	#ifdef DEBUG_IMAGES
		Common::savePPMImage("green.ppm", greenBitmap);
	#endif//DEBUG_IMAGES
}


/** Extracts the blue channel from a given image.
 Extracts the blue information from each pixel in the incoming image, whose dimensions must match. */
void DOGVisualFilter::extractBlueChannel(Bitmap& image){
	//Check image dimensions match
	if(image.getWidth() != blueBitmap->getWidth() || image.getHeight() != blueBitmap->getHeight())
		throw ISpikeException("DOGVisualFilter: Blue image and incoming reduced image have different dimensions");

	//Avoid multiple function calls
	int imageSize = blueBitmap->size();
	unsigned char* blueContents = blueBitmap->getContents();
	unsigned char* imageContents = image.getContents();

	//Copy the blue pixels
	for(int pixel = 0; pixel < imageSize; ++pixel) {
		blueContents[pixel] = imageContents[pixel * 3 + 2];
	}

	//Output debug image if required
	#ifdef DEBUG_IMAGES
		Common::savePPMImage("blue.ppm", blueBitmap);
	#endif//DEBUG_IMAGES
}


/** Constructs the yellow channel from red and green channels */
void DOGVisualFilter::extractYellowChannel(){
	//Avoid multiple function calls
	int imageSize = yellowBitmap->size();
	unsigned char* redContents = redBitmap->getContents();
	unsigned char* greenContents = greenBitmap->getContents();
	unsigned char* yellowContents = yellowBitmap->getContents();
	int tmpNum;

	//Add green and red to create the yellow
	for(int pixel=0; pixel < imageSize; ++pixel){
		tmpNum = redContents[pixel] + greenContents[pixel];
		yellowContents[pixel] = (unsigned char)(tmpNum/2);
	}

	//Output debug image if required
	#ifdef DEBUG_IMAGES
		Common::savePPMImage("yellow.ppm", yellowBitmap);
	#endif//DEBUG_IMAGES
}


/** Extracts the grey channel from a given image, whose dimensions must match.
	Takes the average of the red, green and blue channels. */
void DOGVisualFilter::extractGreyChannel(Bitmap& image){
	//Check image dimensions match
	if(image.getWidth() != greyBitmap->getWidth() || image.getHeight() != greyBitmap->getHeight())
		throw ISpikeException("DOGVisualFilter: Grey image and incoming reduced image have different dimensions");

	//Avoid multiple function calls
	int imageSize = greyBitmap->size();
	unsigned char* greyContents = greyBitmap->getContents();
	unsigned char* imageContents = image.getContents();

	//Take the average of the red, green and blue pixels
	double tmpSum;//Avoid overrun of unsigned char
	for(int pixel = 0; pixel < imageSize; ++pixel) {
		tmpSum =  imageContents[pixel * 3];
		tmpSum += imageContents[pixel * 3 + 1];
		tmpSum += imageContents[pixel * 3 + 2];
		greyContents[pixel] = (unsigned char)rint(tmpSum/3.0);
	}

	//Output debug image if required
	#ifdef DEBUG_IMAGES
		Common::savePPMImage("grey.ppm", greyBitmap);
	#endif//DEBUG_IMAGES
}


/** Initializes the bitmaps used by the class - this way the memory only has to be allocated once */
void DOGVisualFilter::initialize(int width, int height){
	//Check variables have been set
	if(positiveSigma<0 || negativeSigma<0 || positiveFactor <0 || negativeFactor<0 || opponencyTypeID<0)
		throw ISpikeException("DOGVisualFilter: Some or all of the parameters have not been set.");

	redBitmap = new Bitmap(width, height, 1);
	greenBitmap = new Bitmap(width, height, 1);
	blueBitmap = new Bitmap(width, height, 1);
	yellowBitmap = new Bitmap(width, height, 1);
	greyBitmap = new Bitmap(width, height, 1);
	positiveBlurredBitmap = new Bitmap(width, height, 1);
	negativeBlurredBitmap = new Bitmap(width, height, 1);
	outputBitmap = new Bitmap(width, height, 1);
	initialized = true;
}


/** Normalizes the values in the bitmap to the range 0-255 */
void DOGVisualFilter::normalizeImage(Bitmap& image){
	//References to buffer etc.
	int imageSize = image.size();
	unsigned char* imageContents = image.getContents();

	//Find the maximum value in the bitmap
	unsigned char max = 0;
	for(int i=0; i<imageSize; ++i){
		if(imageContents[i] > max)
			max = imageContents[i];
	}

	//Do nothing if the image is already normalized
	if(max == MAX_PIXEL_VALUE)
		return;

	//Normalize the image
	double factor = MAX_PIXEL_VALUE / (double)max;
	for(int i=0; i<imageSize; ++i)
		imageContents[i] = (unsigned char)floor(imageContents[i] * factor);
}


/** Subtracts one image from another of the same dimensions. */
void DOGVisualFilter::subtractImages(Bitmap& firstImage, Bitmap& secondImage, Bitmap& result){
	if(firstImage.size() != secondImage.size() || firstImage.size() != result.size())
		throw ISpikeException("Subtraction only works between images of the same size. The result must be the same size as well.");

	//Get references to buffers etc.
	int imageSize = firstImage.size();
	unsigned char* firstImageContents = firstImage.getContents();
	unsigned char* secondImageContents = secondImage.getContents();
	unsigned char* resultContents = result.getContents();

	for(int i=0; i<imageSize; ++i){
		double val = double(firstImageContents[i])*positiveFactor - double(secondImageContents[i])*negativeFactor;
		resultContents[i] = boost::math::round<unsigned char>(val);
	}
}
