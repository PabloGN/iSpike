//iSpike includes
#include <iSpike/Writer/FileAngleWriter.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;

//Other includes
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>


/** Constructor */
FileAngleWriter::FileAngleWriter() {
	// Define the properties of this writer
	fileName = "anglesOut.txt";
	addProperty(StringProperty("anglesOut.txt", "File Name", "The file where the angles will be written to", true));

	//Create description
	writerDescription = WriterDescription("File Angle Writer", "This is a file angle writer", "Angle Writer");

	//Initialize variables
	currentAngle = -1000000000.0;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

//Inherited from Writer
void FileAngleWriter::initialize(map<string, Property> &properties){
	setProperties(properties);
	setInitialized(true);
}

//Inherited from AngleWriter
void FileAngleWriter::setAngle(double angle){
	if(angle != currentAngle){
		writeAngleToFile();
		currentAngle = angle;
	}
}


//Inherited from PropertyHolder
void FileAngleWriter::setProperties(map<string, Property>& properties){
	fileName = updatePropertyValue(dynamic_cast<StringProperty&>(properties["File Name"]));
}


/*--------------------------------------------------------------------*/
/*---------                PRIVATE METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Writes the angle held in this class to the specified file */
void FileAngleWriter::writeAngleToFile(){
	ofstream fileStream;
	fileStream.open(fileName.c_str(), fstream::out);
	if (!fileStream) {
		ostringstream errorStream;
		errorStream << "FileAngleWriter: Could not write angles to " << fileName;
		throw ISpikeException(errorStream.str());
	}

	fileStream << boost::lexical_cast<string>(getAngle()) << endl;

	if (fileStream.fail()) {
		ostringstream errorStream;
		errorStream << "FileAngleWriter: Could not write angles to " << fileName;
		throw ISpikeException(errorStream.str());
	}

	fileStream.close();

}

