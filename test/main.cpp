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
#include <map>
#include <iSpike/Reader/ReaderFactory.hpp>
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Reader/ReaderDescription.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/Channel/InputChannel/InputChannelFactory.hpp>
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <boost/lexical_cast.hpp>
//#include <iSpike/ChannelController.hpp>
//#include <iSpike/YarpConnection.hpp>
//#include <iSpike/Reader/YarpTextReader.hpp>
//#include <iSpike/Reader/YarpVisualReader.hpp>
//#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>
//#include <iSpike/Bitmap.hpp>
//#include <iSpike/Common.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/date_time/posix_time/posix_time_types.hpp>


int main(int argc, char* argv[])
{
      ReaderFactory readerFactory;
      InputChannelFactory channelFactory;
      //get all input channels and output them to the console
      std::vector<InputChannelDescription> inputChannelDescriptions = channelFactory.getAllChannels();
      std::cout << "Available Input Channels:" << std::endl;
      for(int i = 0; i < inputChannelDescriptions.size(); i++)
      {
        std::cout << i << ": " << inputChannelDescriptions[i].getChannelName() << ": " << inputChannelDescriptions[i].getChannelDescription() << std::endl;
      }

      //let the user pick a channel
      std::cout << "Please select a channel:" << std::endl;
      int selectedChannel;
      std::cin >> selectedChannel;
      std::cin.ignore();

      //get the properties for that channel and let the user provide values
      std::map<std::string,Property*> channelProperties = inputChannelDescriptions[selectedChannel].getChannelProperties();
      std::map<std::string, Property*> constructedChannelProperties;
      for(std::map<std::string,Property*>::iterator iter = channelProperties.begin(); iter != channelProperties.end(); ++iter)
      {
        std::cout << iter->second->getName();
        if(iter->second->getType() == Property::Double)
        {
            double defaultValue = ((DoubleProperty*)(iter->second))->getValue();
            std::cout << "(" << defaultValue << "):" << std::endl;
            std::string readValue;
            double parameterValue;
            std::getline(std::cin,readValue);
            if(readValue == "")
            {
              parameterValue = defaultValue;
            } else {
              parameterValue = boost::lexical_cast<double>(readValue);
            }
            constructedChannelProperties[iter->second->getName()] = new DoubleProperty(
                iter->second->getName(),
                parameterValue,
                iter->second->getDescription()
              );
        } else if(iter->second->getType() == Property::Integer)
        {
          int defaultValue = ((IntegerProperty*)(iter->second))->getValue();
          std::cout << "(" << defaultValue << "):" << std::endl;
          std::string readValue;
          int parameterValue;
          std::getline(std::cin,readValue);
          if(readValue == "")
          {
            parameterValue = defaultValue;
          } else {
            parameterValue = boost::lexical_cast<int>(readValue);
          }
          constructedChannelProperties[iter->second->getName()] = new IntegerProperty(
              iter->second->getName(),
              parameterValue,
              iter->second->getDescription()
            );
        } else if(iter->second->getType() == Property::String)
        {
          std::string defaultValue = ((StringProperty*)(iter->second))->getValue();
          std::cout << "(" << defaultValue << "):" << std::endl;
          std::string readValue;
          std::string parameterValue;
          std::getline(std::cin,readValue);
          if(readValue == "")
          {
            parameterValue = defaultValue;
          } else {
            parameterValue = readValue;
          }
          constructedChannelProperties[iter->second->getName()] = new StringProperty(
              iter->second->getName(),
              parameterValue,
              iter->second->getDescription()
            );
        }
      }

      //now find all Readers that are supported by that channel
      std::string readerType = inputChannelDescriptions[selectedChannel].getReaderType();
      std::vector<ReaderDescription> readerDescriptions = readerFactory.getReadersOfType(readerType);
      std::cout << "Available Readers for this Channel:" << std::endl;
      for(int i = 0; i < readerDescriptions.size(); i++)
      {
        std::cout << i << ": " << readerDescriptions[i].getReaderName() << ": " << readerDescriptions[i].getReaderDescription() << std::endl;
      }

      //let the user pick a reader
      std::cout << "Please select a reader:" << std::endl;
      int selectedReader;
      std::cin >> selectedReader;
      std::cin.ignore();

      //get the properties for that reader and let the user provide values
      std::map<std::string,Property*> readerProperties = readerDescriptions[selectedReader].getReaderProperties();
      std::map<std::string, Property*> constructedReaderProperties;
      for(std::map<std::string,Property*>::iterator iter = readerProperties.begin(); iter != readerProperties.end(); ++iter)
      {
        std::cout << iter->second->getName();
        if(iter->second->getType() == Property::Double)
        {
          double defaultValue = ((DoubleProperty*)(iter->second))->getValue();
          std::cout << "(" << defaultValue << "):" << std::endl;
          std::string readValue;
          double parameterValue;
          std::getline(std::cin,readValue);
          if(readValue == "")
          {
            parameterValue = defaultValue;
          } else {
            parameterValue = boost::lexical_cast<double>(readValue);
          }
          constructedReaderProperties[iter->second->getName()] = new DoubleProperty(
              iter->second->getName(),
              parameterValue,
              iter->second->getDescription()
            );
        } else if(iter->second->getType() == Property::Integer)
        {
          int defaultValue = ((IntegerProperty*)(iter->second))->getValue();
          std::cout << "(" << defaultValue << "):" << std::endl;
          std::string readValue;
          int parameterValue;
          std::getline(std::cin,readValue);
          if(readValue == "")
          {
            parameterValue = defaultValue;
          } else {
            parameterValue = boost::lexical_cast<int>(readValue);
          }
          constructedReaderProperties[iter->second->getName()] = new IntegerProperty(
              iter->second->getName(),
              parameterValue,
              iter->second->getDescription()
            );
        } else if(iter->second->getType() == Property::String)
        {
          std::string defaultValue = ((StringProperty*)(iter->second))->getValue();
          std::cout << "(" << defaultValue << "):" << std::endl;
          std::string readValue;
          std::string parameterValue;
          std::getline(std::cin,readValue);
          if(readValue == "")
          {
            parameterValue = defaultValue;
          } else {
            parameterValue = readValue;
          }
          constructedReaderProperties[iter->second->getName()] = new StringProperty(
              iter->second->getName(),
              parameterValue,
              iter->second->getDescription()
            );
        }
      }

      Reader* reader = readerFactory.create(readerDescriptions[selectedReader].getReaderName(), constructedReaderProperties);
      InputChannel* channel = channelFactory.create(inputChannelDescriptions[selectedChannel].getChannelName(), reader, constructedChannelProperties);
      channel->start();

      std::vector< std::vector<int> > spikes;

      while(true)
      {
        //get fired spikes
        spikes = channel->getFiring();
        channel->step();
        std::cout << spikes.size() << std::endl;

        if(spikes.size() > 0)
        {
          /*for(unsigned int i = 0; i < spikes.size(); i++)
          {
            controller->setFiring(1, &(spikes.at(i)));
            controller->stepOutputChannel(1);
          }*/
          std::cout << "[";
          for(unsigned int i = 0; i < spikes.front().size(); i++)
          {
            std::cout << spikes.front().at(i) << ",";
          }
          std::cout << "]" << std::endl;
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(200));
      }

      return 1;
}

      /*ChannelController* controller = new ChannelController();
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
      controller->inputChannelSubscribe(1);
      char* outputValues[] = {"-90", "90", "0.1", "20"};
      std::vector<std::string> outputArguments(outputValues, outputValues + sizeof(outputValues) / sizeof(char*) );
      controller->outputChannelSubscribe(1, outputArguments);
      std::vector< std::vector<int> > spikes;

      while(true)
      {
        //get fired spikes
        controller->stepInputChannel(1);
        spikes = controller->getFiring(1);
        std::cout << spikes.size() << std::endl;

        if(spikes.size() > 0)
        {
          for(unsigned int i = 0; i < spikes.size(); i++)
          {
            controller->setFiring(1, &(spikes.at(i)));
            controller->stepOutputChannel(1);
          }
          std::cout << "[";
          for(unsigned int i = 0; i < spikes.front().size(); i++)
          {
            std::cout << spikes.front().at(i) << ",";
          }
          std::cout << "]" << std::endl;
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(200));
      }
  return 1;
}

//return UnitTest::RunAllTests();

if any spikes have been fired
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
