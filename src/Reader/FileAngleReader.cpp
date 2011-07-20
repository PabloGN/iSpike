//iSpike includes
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;

//Other includes
#include <boost/asio.hpp>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

/** Constructor */
FileAngleReader() {
	// Define the properties of this reader
	properties["Filename"] = StringProperty("anglesIn.txt", "Filename", "The file where the angles will be read from", true);

	//Create description
	readerDescription = ReaderDescription("File Angle Reader", "This is a file angle reader", "Angle Reader");
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

//Inherited from Reader
void FileAngleReader::initialise(map<string, Property> &properties){
	buffer.clear();

	//Check property exists
	if(propertyMap.count("Filename") == 0)
		throw iSpikeException("Filename property does not exist.");

	fileName = ((StringProperty)properties["FileName"]).getValue();

	setInitialized(true);
}


// Inherited from AngleReader
void FileAngleReader::start() {
	if(!isInitialized()){
		LOG(LOG_INFO) << "FileAngleReader: Reading angles from: " << fileName;
		readAngleFromFile();
		setInitialized(true);
	}
}


/*--------------------------------------------------------------------*/
/*---------                PRIVATE METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Reads angles from a file */
void FileAngleReader::readAngleFromFile() {
	ifstream fileStream;
	string contents;

	fileStream.open(fileName.c_str(), std::ios::in | std::ios::binary);

	if (!fileStream) {
		std::ostringstream messageStream;
		messageStream << "Can't read angles: " << fileName;
		std::string message(messageStream.str());
		throw ISpikeException(message);
	}
	fileStream >> contents;

	LOG(LOG_INFO) << "FileAngleReader: Read the following angles: " << contents;

	if (fileStream.fail()) {
		std::ostringstream messageStream;
		messageStream << "Can't read angles: " << fileName;
		std::string message(messageStream.str());
		throw ISpikeException(message);
	}

	fileStream.close();

	buffer.clear();
	boost::regex split_string(" ");
	std::list<std::string> lines;
	boost::regex_split(std::back_inserter(lines), contents, split_string);
	while(lines.size() > 0)	{
		std::string current_string = *(lines.begin());
		lines.pop_front();
		double angle = boost::lexical_cast<double>(current_string);
		buffer.push_back(angle);
	}
}
