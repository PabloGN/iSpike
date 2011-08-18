/**
 * This test creates a joint input and output channel
 * Associates file readers/writers with the channels
 * And executes them for varying periods of time, with varying inputs, with varying numbers of neurons
 * While recording the results in a file
 */

#include <boost/test/unit_test.hpp>
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Writer/FileAngleWriter.hpp>
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>
#include <iostream>
#include <vector>
#include <iSpike/Log/Log.hpp>

BOOST_AUTO_TEST_CASE(AngleEncodeDecode)
{
	Log::ReportingLevel() = LOG_ERROR;
	srand ( time(NULL) );
	int delay = 0;
	std::vector< std::vector<double> > data;
	std::ofstream angleStream;
	angleStream.open("angles.txt", std::fstream::out | std::fstream::app);
	for (int i = 1; i < 10; i++) {
		double currentAngle = (rand() % 180) - 90;
		LOG(LOG_NOTICE) << "Angle: " << currentAngle;
		std::ofstream inputAngleStream;
		inputAngleStream.open("anglesIn.txt", std::fstream::out);
		inputAngleStream << currentAngle;
		inputAngleStream.close();
		for (int neuronCount = 1; neuronCount < 10; neuronCount++) {
			try {
				LOG(LOG_DEBUG) << "Neuron Count: " << neuronCount;
				//Create the Reader
				FileAngleReader* reader = new FileAngleReader("anglesIn.txt");
				JointInputChannel* inputChannel = new JointInputChannel();
				std::map<std::string, Property*> inputChannelProperties = inputChannel->getChannelDescription().getChannelProperties();
				Property* newNeuronCount = new IntegerProperty("Neuron Width", neuronCount, "description", true);
				inputChannelProperties.at("Neuron Width") = newNeuronCount;
				inputChannel->initialise(reader, inputChannelProperties);
				inputChannel->start();

				FileAngleWriter* writer = new FileAngleWriter();
				std::map<std::string, Property*> outputProperties = writer->getWriterDescription().getWriterProperties();
				writer->initialise();
				LOG(LOG_DEBUG) << "Filename: " << ((StringProperty*)(writer->getWriterDescription().getWriterProperties()["Filename"]))->getValue();
				JointOutputChannel* outputChannel = new JointOutputChannel();
				std::map<std::string, Property*> outputChannelProperties = outputChannel->getChannelDescription().getChannelProperties();
				Property* outputNeuronCount = new IntegerProperty("Neuron Width", neuronCount, "description", true);
				outputChannelProperties.at("Neuron Width") = outputNeuronCount;
				outputChannel->initialise(writer, outputChannelProperties);
				LOG(LOG_DEBUG) << "I: " << i << "Neuron Height: " << ((IntegerProperty*)(outputChannel->getChannelDescription().getChannelProperties()["Neuron Height"]))->getValue();
				outputChannel->start();

				for (delay = 0; delay < 100; delay++) {
					std::vector<std::vector<int> > firings = inputChannel->getFiring();
					if (!firings.empty()) {
						std::vector<int> inputFirings = *(firings.begin());
						outputChannel->setFiring(&inputFirings);
						/*std::ostringstream currentText;
						  currentText << "Neurons: "  << neuronCount << "Spikes: ";
						  for(unsigned int n = 0; n < inputFirings.size(); n++)
						  {
						  currentText << inputFirings[n] << ", ";
						  }
						  LOG(LOG_NOTICE) << currentText.str();*/
					}
					inputChannel->step();
					outputChannel->step();
					//boost::this_thread::sleep(boost::posix_time::milliseconds(1));
					if (delay % 5 == 0) {
						std::vector<double> currentData(5);
						currentData[0] = currentAngle;
						currentData[1] = neuronCount;
						currentData[2] = delay;
						currentData[3] = inputChannel->getCurrentAngle();
						currentData[4] = outputChannel->getCurrentAngle();
						data.push_back(currentData);
						//LOG(LOG_DEBUG) << "Current Delay: " << delay;
						//LOG(LOG_DEBUG) << "Input Angle: " << inputChannel->getCurrentAngle();
						//LOG(LOG_DEBUG) << "Output Angle: " << outputChannel->getCurrentAngle();
					}
				}
				LOG(LOG_ERROR) << "I: " << i << " Neurons: " << neuronCount << " Input Angle: " << inputChannel->getCurrentAngle() << " Output Angle: " << outputChannel->getCurrentAngle();
				//LOG(LOG_DEBUG) << inputChannel->getWidth() << " " << outputChannel->getWidth();
				delete inputChannel;
				delete reader;
				delete outputChannel;
				delete writer;
			} catch(std::exception& e) {
				LOG(LOG_ERROR) << "EXCEPTION CAUGHT!!!!!";
				LOG(LOG_ERROR) << e.what();
			}
		}
		for (int i = 0; i < data.size(); i++) {
			//if((data.size() / i) % 10 == 0)
			//  LOG(LOG_INFO) << (data.size() / i) << "% completed";
			for (int j = 0; j < data[i].size(); j++)
			{
				angleStream << data[i][j];
				if (j < (data[i].size() - 1))
					angleStream << ",";
			}
			angleStream << std::endl;
		}
		angleStream.flush();
		data.clear();
	}
	LOG(LOG_NOTICE) << "Dumping data to angles.txt";
	angleStream.close();
}
