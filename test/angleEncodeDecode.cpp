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
 
 BOOST_AUTO_TEST_CASE(AngleEncodeDecode)
 {
	 int delay = 0;
	 std::vector< std::vector<double> > data;
     for(double currentAngle = -90; currentAngle < 90; currentAngle += 10)
     {
	 std::ofstream inputAngleStream;
	 inputAngleStream.open("anglesIn.txt", std::fstream::out);
	 inputAngleStream << currentAngle;
	 inputAngleStream.close();
	 for(int neuronCount = 1; neuronCount < 100; neuronCount += 10)
	 {
			 //Create the Reader
		 	 FileAngleReader* reader = new FileAngleReader();
		 	 std::map<std::string,Property*> inputProperties = reader->getReaderDescription().getReaderProperties();
		 	 Property* newFilename = new StringProperty("Filename", "anglesIn.txt", "whatever");
		 	inputProperties.at("Filename") = newFilename;
		 	 reader->initialise(inputProperties);
		 	 JointInputChannel* inputChannel = new JointInputChannel();
		 	std::map<std::string,Property*> inputChannelProperties = inputChannel->getChannelDescription().getChannelProperties();
		 	 Property* newNeuronCount = new IntegerProperty(
		 			 "Neuron Width",
		 			 neuronCount,
		 			 "description"
		 	 );
		 	inputChannelProperties.at("Neuron Width") = newNeuronCount;
		 	 inputChannel->initialise(reader, inputChannelProperties);
		 	 inputChannel->start();

		 	 FileAngleWriter* writer = new FileAngleWriter();
		 	std::map<std::string,Property*> outputProperties = writer->getWriterDescription().getWriterProperties();
		      writer->initialise();
		      JointOutputChannel* outputChannel = new JointOutputChannel();
			 	std::map<std::string,Property*> outputChannelProperties = outputChannel->getChannelDescription().getChannelProperties();
			 	outputChannelProperties.at("Neuron Width") = newNeuronCount;
		      outputChannel->initialise(writer, outputChannelProperties);
		      outputChannel->start();

		      for(delay = 0; delay < 1000; delay++)
		      {
		     	 std::vector< std::vector<int> > firings = inputChannel->getFiring();
		     	 if(!firings.empty())
		     	 {
		     		 std::vector<int> inputFirings = *(firings.begin());
		     		 outputChannel->setFiring(&inputFirings);
		     	 }
		     	 inputChannel->step();
		     	 outputChannel->step();
		     	 boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		     	 if(delay % 5 == 0)
		     	 {
		     		 std::vector<double> currentData(5);
		     		 currentData[0] = currentAngle;
		     		 currentData[1] = neuronCount;
		     		 currentData[2] = delay;
		     		 currentData[3] = inputChannel->getCurrentAngle();
		     		 currentData[4] = outputChannel->getCurrentAngle();
		     		 data.push_back(currentData);
		     		 std::cout << "Current Delay: " << delay << std::endl;
		     		 std::cout << "Input Angle: " << inputChannel->getCurrentAngle() << std::endl;
		     		 std::cout << "Output Angle: " << outputChannel->getCurrentAngle() << std::endl;
		     	 }
		      }
	 }
     }
     std::ofstream angleStream;
     angleStream.open("angles.txt", std::fstream::out);
     for(int i = 0; i < data.size(); i++)
     {
    	 for(int j = 0; j < data[i].size(); j++)
    	 {
    		 angleStream << data[i][j];
    		 if(j < (data[i].size() - 1))
    			 angleStream << ",";
    	 }
    	 angleStream << std::endl;
     }
     angleStream.close();
}
