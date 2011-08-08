//iSpike includes
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;

//Other includes
#include <boost/regex.hpp>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


/** Constructor */
FileAngleReader::FileAngleReader() {
	// Define the properties of this reader
	addProperty(Property("anglesIn.txt", "File Name", "The file where the angles will be read from", true));

	//Create description
	readerDescription = Description("File Angle Reader", "This is a file angle reader", "Angle Reader");
}


/** Destructor */
FileAngleReader::~FileAngleReader(){
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

//Inherited from Reader
void FileAngleReader::initialize(map<string, Property>& properties){
	setProperties(properties);
	setInitialized(true);
}


//Inherited from PropertyHolder
void FileAngleReader::setProperties(map<string, Property>& properties){
	string fileName = updateStringProperty(properties["File Name"]);
	LOG(LOG_INFO) << "FileAngleReader: Reading angles from: " << fileName;
	readAngleFromFile(fileName);
}


/*--------------------------------------------------------------------*/
/*---------                PRIVATE METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Reads angles from a file */
void FileAngleReader::readAngleFromFile(string& fileName) {
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

	vector<double> buffer;
	boost::regex split_string(" ");
	list<string> lines;
	boost::regex_split(std::back_inserter(lines), contents, split_string);
	while(lines.size() > 0)	{
		std::string current_string = *(lines.begin());
		lines.pop_front();
		double angle = boost::lexical_cast<double>(current_string);
		buffer.push_back(angle);
	}

	if(!buffer.empty())
		setAngle(buffer[0]);
	else
		throw ISpikeException("FileAngleReader: No angles found in file.");
}
