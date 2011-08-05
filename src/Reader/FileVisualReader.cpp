//iSpike includes
#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;

//Other includes
#include <iostream>
#include <fstream>
using namespace std;


/** Constructor */
FileVisualReader::FileVisualReader(){
	// Define the properties of this reader
	addProperty(StringProperty("imageIn.txt", "File Name", "The file where the image will be read from", true));

	//Create description
	readerDescription = Description("File Visual Reader", "This is a file visual reader", "Visual Reader");

	//Initialize variables
	bitmap = NULL;
}


/** Destructor */
FileVisualReader::~FileVisualReader(){
	if(bitmap != NULL)
		delete bitmap;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Returns a reference to the visual data */
Bitmap& FileVisualReader::getBitmap(){
	return *bitmap;
}


//Inherited from Reader
void FileVisualReader::initialize(map<string, Property>& properties){
	setProperties(properties);
	setInitialized(true);
}


//Inherited from PropertyHolder
void FileVisualReader::setProperties(map<string, Property>& properties){
	string fileName = updatePropertyValue(dynamic_cast<StringProperty&>(properties["File Name"]));
	LOG(LOG_INFO) << "FileVisualReader: Reading image from: " << fileName;
	readPPMImage(fileName);
}


/*--------------------------------------------------------------------*/
/*---------                PRIVATE METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Reads a PPM image from the specified file */
void FileVisualReader::readPPMImage(string& fname){
	//Clean up old bitmap
	if(bitmap != NULL)
		delete bitmap;

	int N, M;
	char header [100], *ptr;
	ifstream ifp;

	ifp.open(fname.c_str(), ios::in | ios::binary);

	if (!ifp) {
		ostringstream messageStream;
		messageStream << "Can't read image: " << fname;
		string message(messageStream.str());
		throw ISpikeException(message);
	}

	// read header
	ifp.getline(header,100,'\n');

	if ( (header[0]!=80) ||    /* 'P' */
		 (header[1]!=54) ) {   /* '6' */
		ostringstream messageStream;
		messageStream << "Image " << fname << " is not PPM";
		string message(messageStream.str());
		throw ISpikeException(message);
	}

	ifp.getline(header,100,'\n');
	while(header[0]=='#')
		ifp.getline(header,100,'\n');

	M=strtol(header,&ptr,0);
	N=atoi(ptr);

	LOG(LOG_INFO) << "FileVisualReader: Retrieved image with Width: " << M << " Height: " << N;

	ifp.getline(header,100,'\n');

	//Create bitmap
	bitmap = new Bitmap(M, N, 3);
	unsigned char *charImage = bitmap->getContents();

	//Read image into bitmap's contents
	ifp.read((char*)charImage, 3*M*N);
	if (ifp.fail()) {
		ostringstream messageStream;
		messageStream << "Image " << fname << " has wrong size";
		string message(messageStream.str());
		throw ISpikeException(message);
	}

	//Clean up
	ifp.close();

	//Increment image id
	++imageID;
}

