/*
 * This test creates a joint input and output channel
 * Associates file readers/writers with the channels
 * And executes them for varying periods of time, with varying inputs, with varying numbers of neurons
 * While recording the results in a file
 */

#include <boost/test/unit_test.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Writer/FileAngleWriter.hpp>
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <iSpike/Log/Log.hpp>

BOOST_AUTO_TEST_CASE(AngleEncodeDecode)
{
	using namespace ispike;

	//Log::ReportingLevel() = LOG_ERROR;
	LOG(LOG_DEBUG)<<"AngleEncodeDecode Test";
	return;

	srand ( time(NULL) );
	int delay = 0;
	// std::vector< std::vector<double> > data;
	// std::ofstream angleStream;
	// angleStream.open("angles.txt", std::fstream::out | std::fstream::app);
	for (int i = 1; i < 10; i++) {
		double currentAngle = (rand() % 180) - 90;
		LOG(LOG_NOTICE) << "Angle: " << currentAngle;
#if 0
		std::ofstream inputAngleStream;
		inputAngleStream.open("anglesIn.txt", std::fstream::out);
		inputAngleStream << currentAngle;
		inputAngleStream.close();
#endif


		for (int neuronCount = 2; neuronCount < 10; neuronCount++) {

			LOG(LOG_DEBUG) << "Neuron Count: " << neuronCount;
			FileAngleReader* reader = new FileAngleReader();

			/* Create input channel */
			boost::scoped_ptr<JointInputChannel> inputChannel(new JointInputChannel());
			std::map<std::string, Property> inputChannelProperties = inputChannel->getProperties();
			inputChannelProperties.at("Neuron Width") =
				Property(Property::Integer, neuronCount,
						"Neuron Width", "description", true);
			inputChannel->initialize(reader, inputChannelProperties);

			/* Create output channel */
			FileAngleWriter* writer = new FileAngleWriter();
			boost::scoped_ptr<JointOutputChannel> outputChannel(new JointOutputChannel());
			std::map<std::string, Property> outputChannelProperties = outputChannel->getProperties();
			outputChannelProperties.at("Neuron Width") =
				Property(Property::Integer, neuronCount,
						"Neuron Width", "description", true);
			outputChannel->initialize(writer, outputChannelProperties);

			// LOG(LOG_DEBUG) << "I: " << i << "Neuron Height: " << ((IntegerProperty*)(outputChannel->getChannelDescription().getChannelProperties()["Neuron Height"]))->getValue();
			// outputChannel->start();

			for (delay = 0; delay < 100; delay++) {
				const vector<unsigned>& firings = inputChannel->getFiring();
				if (!firings.empty()) {
					outputChannel->setFiring(firings);
				}
				inputChannel->step();
				outputChannel->step();
				// now compare the input and output angles
				//boost::this_thread::sleep(boost::posix_time::milliseconds(1));
#if 0
				if (delay % 5 == 0) {
					std::vector<double> currentData(5);
					currentData[0] = currentAngle;
					currentData[1] = neuronCount;
					currentData[2] = delay;
					// HERE: how do we get the angles?
					currentData[3] = inputChannel->getCurrentAngle();
					currentData[4] = outputChannel->getCurrentAngle();
					// data.push_back(currentData);
					//LOG(LOG_DEBUG) << "Current Delay: " << delay;
					//LOG(LOG_DEBUG) << "Input Angle: " << inputChannel->getCurrentAngle();
					//LOG(LOG_DEBUG) << "Output Angle: " << outputChannel->getCurrentAngle();
				}
#endif
			}
			// LOG(LOG_ERROR) << "I: " << i << " Neurons: " << neuronCount << " Input Angle: " << inputChannel->getCurrentAngle() << " Output Angle: " << outputChannel->getCurrentAngle();
			//LOG(LOG_DEBUG) << inputChannel->getWidth() << " " << outputChannel->getWidth();
		}
#if 0
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
#endif
		// data.clear();
	}
	LOG(LOG_NOTICE) << "Dumping data to angles.txt";
	// angleStream.close();
}
