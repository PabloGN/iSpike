//iSpike includes
#include <iSpike/Property.hpp>
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/Writer/FileAngleWriter.hpp>
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>
#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>
#include <iSpike/Log/Log.hpp>
#include "MockAngleWriter.h"

//Boost includes
#include <boost/test/unit_test.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

//Other includes
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using namespace ispike;

BOOST_AUTO_TEST_CASE(Performance){
	//Reporting level
	Log::ReportingLevel() = LOG_ERROR;

	//Results file
	ofstream resultsFileStream;
	resultsFileStream.open("results.txt", fstream::out);

	//Number of time steps to run each test
	int numTestTimeSteps = 100000;

	// First test: encode angle into spikes using different resolutions
	LOG(LOG_DEBUG)<<"Testing joint angle encoding";
	for(int numNeur=10; numNeur<=100; numNeur += 10){//Experiment with different numbers of neurons to encode angles
		resultsFileStream<<"Joint angle Encoding "<<numNeur<<" Neurons"<<endl;
		for(int testNum=1; testNum<=10; ++testNum){//Ten test runs
			//Create reader
			FileAngleReader* reader = new FileAngleReader();//Will read from anglesIn.txt in the run directory by default

			// Create input channel
			boost::scoped_ptr<JointInputChannel> inputChannel(new JointInputChannel());
			std::map<std::string, Property> inputChannelProperties = inputChannel->getProperties();
			inputChannelProperties.at("Neuron Width") =	Property(Property::Integer, numNeur, "Neuron Width", "description", true);
			inputChannel->initialize(reader, inputChannelProperties);

			//Time 1000 time steps
			boost::posix_time::ptime startTime(boost::posix_time::microsec_clock::universal_time());
			for(int i=0; i<numTestTimeSteps; ++i){
				inputChannel->step();
				inputChannel->getFiring();
			}
			boost::posix_time::ptime endTime(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration timeDur = endTime - startTime;
			resultsFileStream<<( (double)timeDur.total_microseconds() / (double)numTestTimeSteps )<<endl;
			LOG(LOG_DEBUG)<<"Test "<<testNum<<"; 1000 time steps took "<<timeDur.total_microseconds()<<" microseconds";
		}
		resultsFileStream<<endl;
		resultsFileStream.flush();
	}


	//Second test: Decode angle from spikes using different resolutions
	LOG(LOG_DEBUG)<<endl<<"Testing joint angle decoding";
	for(int numNeur=10; numNeur<=100; numNeur += 10){//Experiment with different numbers of neurons to decode angles
		resultsFileStream<<"Joint angle Decoding "<<numNeur<<" Neurons"<<endl;
		for(int testNum=1; testNum<=10; ++testNum){//Ten test runs
			//Create array of firing neurons
			std::vector<unsigned> firingNeurons;
			firingNeurons.push_back(numNeur/2);

			//Create mock writer
			MockAngleWriter* writer = new MockAngleWriter();//Accepts the angle without doing anything with it.

			//Create output channel
			boost::scoped_ptr<JointOutputChannel> outputChannel(new JointOutputChannel());
			std::map<std::string, Property> outputChannelProperties = outputChannel->getProperties();
			outputChannelProperties.at("Neuron Width") = Property(Property::Integer, numNeur, "Neuron Width", "description", true);
			outputChannel->initialize(writer, outputChannelProperties);

			//Time 1000 time steps
			boost::posix_time::ptime startTime(boost::posix_time::microsec_clock::universal_time());
			for(int i=0; i<numTestTimeSteps; ++i){
				outputChannel->setFiring(firingNeurons);
				outputChannel->step();
			}
			boost::posix_time::ptime endTime(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration timeDur = endTime - startTime;
			resultsFileStream<<( (double)timeDur.total_microseconds() / (double)numTestTimeSteps )<<endl;
			LOG(LOG_DEBUG)<<"Test "<<testNum<<"; "<<numTestTimeSteps<<" time steps took "<<timeDur.total_microseconds()<<" microseconds";
		}
		resultsFileStream<<endl;
		resultsFileStream.flush();
	}


	//Third test: encode vision
	LOG(LOG_DEBUG)<<endl<<"Testing vision encoding";
	//Array of update rates
	int updateRateArray[] = {25, 50, 100, 200, 300, 100000};
	int updateRateArrayLength = 6;

	//Run image stuff for less time steps
	numTestTimeSteps = 1000;

	//Test image with 640x480
	for(int imgRateCtr = 0; imgRateCtr < updateRateArrayLength; ++ imgRateCtr){
		resultsFileStream<<"Vision encoding 640x480; Update rate: "<<updateRateArray[imgRateCtr]<<endl;

		for(int testNum=1; testNum<=10; ++testNum){//Ten test runs
			//Create reader
			FileVisualReader* visualReader = new FileVisualReader();
			std::map<std::string, Property> fileVisualReaderProperties = visualReader->getProperties();
			fileVisualReaderProperties.at("File Name") = Property("image640x480.ppm", "File Name", "description", true);
			visualReader->initialize(fileVisualReaderProperties);

			//Create visual channel
			boost::scoped_ptr<VisualInputChannel> visualChannel(new VisualInputChannel());
			std::map<std::string, Property> visualChannelProperties = visualChannel->getProperties();
			visualChannelProperties.at("Neuron Width") = Property(Property::Integer, 200, "Neuron Width", "description", true);
			visualChannelProperties.at("Neuron Height") = Property(Property::Integer, 200, "Neuron Height", "description", true);
			visualChannelProperties.at("Fovea Radius") = Property(Property::Double, 25, "Fovea Radius", "description", true);
			visualChannel->initialize(visualReader, visualChannelProperties);

			//Run test numTestTimeSteps time steps
			boost::posix_time::ptime startTime(boost::posix_time::microsec_clock::universal_time());
			for(int i=0; i<numTestTimeSteps; ++i){
				visualChannel->step();
				visualChannel->getFiring();

				//Change the image ID so that it is reloaded by the channel
				if(i % updateRateArray[imgRateCtr] == 0)
					visualReader->setImageID(visualReader->getImageID() + 1);
			}
			boost::posix_time::ptime endTime(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration timeDur = endTime - startTime;
			resultsFileStream<<( (double)timeDur.total_microseconds() / (double)numTestTimeSteps )<<endl;
			LOG(LOG_DEBUG)<<"Test "<<testNum<<"; "<<numTestTimeSteps<<" time steps took "<<timeDur.total_microseconds()<<" microseconds";
		}
		resultsFileStream<<endl;
		resultsFileStream.flush();
	}

	//Test image with 200x200
	for(int imgRateCtr = 0; imgRateCtr < updateRateArrayLength; ++ imgRateCtr){
		resultsFileStream<<"Vision encoding 200x200; Update rate: "<<updateRateArray[imgRateCtr]<<endl;

		for(int testNum=1; testNum<=10; ++testNum){//Ten test runs
			//Create reader
			FileVisualReader* visualReader = new FileVisualReader();
			std::map<std::string, Property> fileVisualReaderProperties = visualReader->getProperties();
			fileVisualReaderProperties.at("File Name") = Property("image200x200.ppm", "File Name", "description", true);
			visualReader->initialize(fileVisualReaderProperties);

			//Create visual channel
			boost::scoped_ptr<VisualInputChannel> visualChannel(new VisualInputChannel());
			std::map<std::string, Property> visualChannelProperties = visualChannel->getProperties();
			visualChannelProperties.at("Neuron Width") = Property(Property::Integer, 50, "Neuron Width", "description", true);
			visualChannelProperties.at("Neuron Height") = Property(Property::Integer, 50, "Neuron Height", "description", true);
			visualChannelProperties.at("Fovea Radius") = Property(Property::Double, 10, "Fovea Radius", "description", true);
			visualChannel->initialize(visualReader, visualChannelProperties);

			//Run test numTestTimeSteps time steps
			boost::posix_time::ptime startTime(boost::posix_time::microsec_clock::universal_time());
			for(int i=0; i<numTestTimeSteps; ++i){
				visualChannel->step();
				visualChannel->getFiring();

				//Change the image ID so that it is reloaded by the channel
				if(i % updateRateArray[imgRateCtr] == 0)
					visualReader->setImageID(visualReader->getImageID() + 1);
			}
			boost::posix_time::ptime endTime(boost::posix_time::microsec_clock::universal_time());
			boost::posix_time::time_duration timeDur = endTime - startTime;
			resultsFileStream<<( (double)timeDur.total_microseconds() / (double)numTestTimeSteps )<<endl;
			LOG(LOG_DEBUG)<<"Test "<<testNum<<"; "<<numTestTimeSteps<<" time steps took "<<timeDur.total_microseconds()<<" microseconds";
		}
		resultsFileStream<<endl;
	}

	//Close results file
	resultsFileStream.close();
}
