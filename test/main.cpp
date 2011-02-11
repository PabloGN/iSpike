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
#include <SpikeAdapter/ChannelController.hpp>
#include <SpikeAdapter/YarpInterface.hpp>
#include <SpikeAdapter/Reader/YarpTextReader.hpp>
#include <SpikeAdapter/Reader/YarpVisualReader.hpp>
#include <SpikeAdapter/Channel/DummyInputChannel.hpp>
#include <SpikeAdapter/Bitmap.hpp>
#include <SpikeAdapter/Common.hpp>
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
      controller->inputChannelSubscribe(1);
      std::vector< std::vector<int> > spikes;

      while(true)
      {
        //get fired spikes
        spikes = controller->getFiring(1);
        std::cout << spikes.size() << std::endl;

        //if any spikes have been fired
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
        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(200));
      }
  return 1;
  //return UnitTest::RunAllTests();
}
