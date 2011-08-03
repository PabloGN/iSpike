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
	addProperty(StringProperty("anglesOut.txt", "File Name", "The file where the angles will be written to", true));

	//Create description
	writerDescription = WriterDescription("File Angle Writer", "This is a file angle writer", "Angle Writer");
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

//Inherited from Writer
void FileAngleWriter::initialise(map<string, Property> &properties){
	setProperties(properties);
	setInitialized(true);
}


//Inherited from PropertyHolder
void FileAngleWriter::setProperties(map<string, Property>& properties){
	string fileName = getPropertyValue((StringProperty)properties["File Name"]);
	LOG(LOG_INFO) << "FileAngleReader: Reading angles from: " << fileName;
	writeAngleToFile(string& fileName);
}


/*--------------------------------------------------------------------*/
/*---------                PRIVATE METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Writes the angle held in this class to the specified file */
void FileAngleWriter::writeAngleToFile(const char* fileName){
	std::ofstream fileStream;

	fileStream.open(fileName, std::fstream::out);

	if (!fileStream) {
		std::ostringstream errorStream;
		errorStream << "FileAngleWriter: Could not write angles to " << fileName;
		throw ISpikeException(errorStream.str());
	}

	fileStream << boost::lexical_cast<std::string>(getAngle()) << std::endl;

	if (fileStream.fail()) {
		std::ostringstream errorStream;
		errorStream << "FileAngleWriter: Could not write angles to " << fileName;
		throw ISpikeException(errorStream.str());
	}

	fileStream.close();

}

