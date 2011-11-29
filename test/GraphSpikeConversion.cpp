//iSpike includes
#include <iSpike/Property.hpp>
#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>
#include <iSpike/Log/Log.hpp>

//Boost includes
#include <boost/test/unit_test.hpp>
#include <boost/unordered_map.hpp>

//Other includes
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using namespace ispike;

BOOST_AUTO_TEST_CASE(GraphSpikeConversion){
	//Reporting level
	Log::ReportingLevel() = LOG_DEBUG;
	LOG(LOG_DEBUG)<<"GraphSpikeConversion Test";

	//Number of time steps to record
	int numTestTimeSteps = 10000;

	//Create reader
	FileVisualReader* visualReader = new FileVisualReader();
	std::map<std::string, Property> fileVisualReaderProperties = visualReader->getProperties();
	fileVisualReaderProperties.at("File Name") = Property("spikeTest400x400.ppm", "File Name", "description", true);
	visualReader->initialize(fileVisualReaderProperties);

	//Number of pixels in the image
	int neuronWidth = 200;
	int neuronHeight = 200;
	int imageSize = neuronWidth*neuronHeight;

	//Create visual channel
	boost::scoped_ptr<VisualInputChannel> visualChannel(new VisualInputChannel());
	std::map<std::string, Property> visualChannelProperties = visualChannel->getProperties();
	visualChannelProperties.at("Neuron Width") = Property(Property::Integer, neuronWidth, "Neuron Width", "description", true);
	visualChannelProperties.at("Neuron Height") = Property(Property::Integer, neuronHeight, "Neuron Height", "description", true);
	visualChannelProperties.at("Fovea Radius") = Property(Property::Double, 50, "Fovea Radius", "description", true);
	visualChannelProperties.at("Current Factor") = Property(Property::Double, 7.5, "Current Factor", "description", true);
	visualChannel->initialize(visualReader, visualChannelProperties);


	//Map for time to first spike
	boost::unordered_map<unsigned, int> spikeMap;

	//Map for rate
	boost::unordered_map<unsigned, int> spikeCountMap;
	for(int i=0; i<imageSize; ++i)
		spikeCountMap[i] = 0;

	//Run test numTestTimeSteps time steps
	LOG(LOG_DEBUG)<<"Running "<<neuronWidth<<"x"<<neuronHeight<<" for "<<numTestTimeSteps<<" time steps.";
	for(int i=0; i<numTestTimeSteps; ++i){
		visualChannel->step();
		const vector<unsigned>& spikeVect = visualChannel->getFiring();
		LOG(LOG_DEBUG)<<"NUMBER OF FIRING NEURONS: "<<spikeVect.size()<<" spike map size: "<<spikeMap.size();

		for(int j=0; j<spikeVect.size(); ++j){
			//Add spikes to map if they have not been done already
			if(!spikeMap.count(spikeVect.at(j))){
				spikeMap[spikeVect.at(j)] = i;
			}

			//Count the number of times that each neuron fires
			if(spikeCountMap.count(spikeVect.at(j)) == 0)
				LOG(LOG_ERROR)<<"Neuron ID missing fro spike count map: "<<spikeVect.at(j);
			++spikeCountMap[spikeVect.at(j)];
		}
	}

	//Set any remaining neurons to the maximum duration
	for(int i=0; i<imageSize; ++i){
		if(!spikeMap.count(i)){
			spikeMap[i] = numTestTimeSteps;
		}
	}

	//Output time to fire results
	ofstream resFileStr;
	resFileStr.open("SpikeResults_TimeToFire.txt", fstream::out);
	for(int yPos=0; yPos<neuronHeight; ++yPos){
		for(int xPos=0; xPos<neuronWidth; ++xPos){
			resFileStr<<spikeMap[yPos*neuronWidth + xPos]<<" ";
		}
		resFileStr<<endl;
	}
	resFileStr<<endl;
	resFileStr.close();

	//Output rate results
	ofstream rateFileStr;
	rateFileStr.open("SpikeResults_Rate.txt", fstream::out);
	for(int yPos=0; yPos<neuronHeight; ++yPos){
		for(int xPos=0; xPos<neuronWidth; ++xPos){
			rateFileStr<<( (1000.0 * (double)spikeCountMap[yPos*neuronWidth + xPos]) / (double)numTestTimeSteps)<<" ";
		}
		rateFileStr<<endl;
	}
	rateFileStr<<endl;
	rateFileStr.close();

	//Print averaged pixel values
	ofstream imgFileStr;
	imgFileStr.open("ImagePixelValues.txt", fstream::out);

	Bitmap& inputImage = visualReader->getBitmap();
	unsigned char* inputImageContents = inputImage.getContents();
	LOG(LOG_DEBUG)<<"Writing pixel values. Width: "<<inputImage.getWidth()<<"; height: "<<inputImage.getHeight()<<"; depth: "<<inputImage.getDepth();

	for(int yPos=0; yPos<inputImage.getHeight(); ++yPos){
		for(int xPos=0; xPos<inputImage.getWidth(); ++xPos){
			//Take the average value for this position
			double avgVal = inputImage.getPixel(xPos,yPos,0) + inputImage.getPixel(xPos,yPos,1) + inputImage.getPixel(xPos,yPos,2);
			imgFileStr<<(avgVal/3.0)<<" ";
		}
		imgFileStr<<endl;
	}

	imgFileStr<<endl;
	imgFileStr.close();
}
