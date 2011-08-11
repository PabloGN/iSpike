//iSpike includes
#include <iSpike/Writer/FileAngleWriter.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;

//Other includes
#include <string>
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>
using namespace std;


/** Constructor */
FileAngleWriter::FileAngleWriter() {
	// Define the properties of this writer
	fileName = "anglesOut.txt";
	addProperty(Property("anglesOut.txt", "File Name", "The file where the angles will be written to", true));

	//Create description
	writerDescription = Description("File Angle Writer", "This is a file angle writer", "Angle Writer");
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
void FileAngleWriter::setAngle(double newAngle){
	if(this->angle != newAngle){
		this->angle = newAngle;
		writeAngleToFile();
	}
}


//Inherited from AngleWriter
void FileAngleWriter::setDegreeOfFreedom(int dof){
	if(this->degreeOfFreedom != dof){
		this->degreeOfFreedom = dof;
		writeAngleToFile();
	}
}


//Inherited from PropertyHolder
void FileAngleWriter::setProperties(map<string, Property>& properties){
	fileName = updateStringProperty(properties["File Name"]);
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

