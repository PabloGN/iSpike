/*
 * This file includes the magic necessary in order to get your unit tests
 * that you create with UnitTest++ to automatically run. There should
 * never be a reason to modify this file. For an example unit test,
 * see "sanity_check.cpp". For a reference on creating tests, see "test.h".
 *
 */
#include "test.h"
#include <string>
#include <iostream>
#include <vector>
#include <iSpike/ChannelController.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Reader/YarpTextReader.hpp>
#include <iSpike/Reader/YarpVisualReader.hpp>
#include <iSpike/Channel/VisualInputChannel.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Common.hpp>
#include <boost/thread/thread.hpp>
#include <pthread.h>
#include <boost/date_time/posix_time/posix_time_types.hpp>

int main(int argc, char* argv[])
{
  //YarpInterface *interface = new YarpInterface(boost::asio::ip::hostname(), "10001");
      //Neuron Network Size
      int width = 320;
      int height = 240;

      ChannelController* controller = new ChannelController();
      std::map<int, std::string>::iterator i;
      std::map<int,std::string>* inputChannels = controller->getInputChannels();
      std::cout << "Input Channels:" << std::endl;
      for (i = inputChannels->begin(); i != inputChannels->end(); i++)
        std::cout << i->first << "," << i->second << std::endl;
      std::map<int,std::string>* outputChannels = controller->getOutputChannels();
      std::cout << "Output Channels:" << std::endl;
      for (i = outputChannels->begin(); i != outputChannels->end(); i++)
        std::cout << i->first << "," << i->second << std::endl;

      char* inputValues[] = {"0", "2", "-90", "90", "20"};
      std::vector<std::string> inputArguments(inputValues, inputValues + sizeof(inputValues) / sizeof(char*) );
      controller->inputChannelSubscribe(1, inputArguments);
      char* outputValues[] = {"-90", "90", "0.1", "20"};
      std::vector<std::string> outputArguments(outputValues, outputValues + sizeof(outputValues) / sizeof(char*) );
      controller->outputChannelSubscribe(1, outputArguments);
      std::vector< std::vector<int> > spikes;

      while(true)
      {
        //int angle = 45;
        //int neuronId = floor(angle / 10 + 0.5);

        //get fired spikes
        controller->stepInputChannel(1);
        spikes = controller->getFiring(1);
        //spikes = std::vector< std::vector<int> >();
        //spikes.push_back(std::vector<int>(1,1));
        std::cout << spikes.size() << std::endl;

        if(spikes.size() > 0)
        {
          for(int i = 0; i < spikes.size(); i++)
          {
            controller->setFiring(1, &(spikes.at(i)));
            controller->stepOutputChannel(1);
          }
          std::cout << "[";
          for(int i = 0; i < spikes.front().size(); i++)
          {
            std::cout << spikes.front().at(i) << ",";
          }
          std::cout << "]" << std::endl;
        }

        /*//if any spikes have been fired
        if(spikes.size() > 0 )
        {
          //create a grayscale image buffer
          unsigned char* buffer = new unsigned char[width * height];
          for(int i = 0; i < width * height; i++)
            buffer[i] = 0;

          //for each spike frame
          for(int i = 0; i < spikes.size(); i++)
          {
            //for each spike
            for(int j = 0; j < spikes.at(i).size(); j++)
            {
              if(buffer[spikes.at(i).at(j)] == 0)
              {
                double ratio = ((double)i / spikes.size());
                unsigned char pixelValue = (int) floor(255 * (1 - ratio) + 0.5);
                buffer[spikes.at(i).at(j)] = pixelValue;
              }
            }
          }
          Bitmap* image = Common::produceGrayscale(buffer, width, height);
          Common::savePPMImage("resultSpikes.ppm", image);
          delete buffer;
          delete image;
        } else {
          std::cout << "length is zero" << std::endl;
        }*/

        boost::this_thread::sleep(boost::posix_time::milliseconds(200));
      }
  return 1;
  //return UnitTest::RunAllTests();
}
