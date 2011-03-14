/*
 * ChannelController.cpp
 *
 *  Created on: 7 Jan 2011
 *      Author: Edgars Lazdins
 */
#include <string>
#include <iostream>
#include <iSpike/ChannelController.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/Reader/YarpVisualReader.hpp>
#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>
#include <iSpike/Writer/YarpAngleWriter.hpp>
#include <iSpike/Reader/FileAngleReader.hpp>
#include <iSpike/Writer/FileAngleWriter.hpp>

std::map<int, std::string>* ChannelController::getInputChannels()
{
  std::map<int, InputChannel*>::iterator i;
  std::map<int, std::string>* result = new std::map<int, std::string>();
  for (i = this->inputChannelDirectory->begin(); i != this->inputChannelDirectory->end(); i++)
    result->insert(std::pair<int, std::string>(i->first, i->second->getDescription()));
  return result;
}

std::map<int, std::string>* ChannelController::getOutputChannels()
{
  std::map<int, OutputChannel*>::iterator i;
  std::map<int, std::string>* result = new std::map<int, std::string>();
  for (i = this->outputChannelDirectory->begin(); i != this->outputChannelDirectory->end(); i++)
    result->insert(std::pair<int, std::string>(i->first, i->second->getDescription()));
  return result;
}

void ChannelController::stepInputChannel(int channelId)
{
  std::map<int, InputChannel*>::iterator iter = inputChannelDirectory->find(channelId);
    if (iter != inputChannelDirectory->end() )
    {
      if(iter->second->isInitialised())
        return iter->second->step();
      else
        std::cout << "Input Channel " << channelId << " has not been initialised!";
    } else {
      std::cout << "Iterator is empty!" << std::endl;
    }
}

void ChannelController::stepOutputChannel(int channelId)
{
  std::map<int, OutputChannel*>::iterator iter = outputChannelDirectory->find(channelId);
    if (iter != outputChannelDirectory->end() )
    {
      if(iter->second->isInitialised())
        return iter->second->step();
      else
        std::cout << "Output Channel " << channelId << " has not been initialised!";
    } else {
      std::cout << "Iterator is empty!" << std::endl;
    }
}

std::vector< std::vector<int> > ChannelController::getFiring(int channelId)
{
  std::map<int, InputChannel*>::iterator iter = inputChannelDirectory->find(channelId);
  if (iter != inputChannelDirectory->end() )
  {
    if(iter->second->isInitialised())
      return iter->second->getFiring();
    else
      std::cout << "Input Channel " << channelId << " has not been initialised!";
  } else {
    std::cout << "Iterator is empty!" << std::endl;
  }
}

void ChannelController::setFiring(int channelId, std::vector<int>* spikes)
{
  std::map<int, OutputChannel*>::iterator iter = outputChannelDirectory->find(channelId);
  if (iter != outputChannelDirectory->end() )
  {
    if(iter->second->isInitialised())
      return iter->second->setFiring(spikes);
    else
      std::cout << "Output Channel " << channelId << " has not been initialised!";
  } else {
    std::cout << "Iterator is empty!" << std::endl;
  }
}

ChannelController::ChannelController()
{
  this->inputChannelDirectory = new std::map<int, InputChannel*>();
  this->outputChannelDirectory = new std::map<int, OutputChannel*>();
  //YarpAngleReader* jointReader = new YarpAngleReader("/icubSim/left_arm/state:o", "127.0.0.1", "10006");
  //FileAngleReader* jointReader = new FileAngleReader();
  //JointInputChannel* readerChannel = new JointInputChannel(jointReader);
  //this->inputChannelDirectory->insert(std::pair<int, InputChannel*>(1, readerChannel));
  //YarpAngleWriter* jointWriter = new YarpAngleWriter("/icubSim/left_arm/rpc:i", "127.0.0.1", "10006");
  FileAngleWriter* jointWriter = new FileAngleWriter("anglesOut.txt");
  JointOutputChannel* writerChannel = new JointOutputChannel(jointWriter);
  this->outputChannelDirectory->insert(std::pair<int, OutputChannel*>(1, writerChannel));
  //const char* filename = "C:\\Users\\cembo\\workspace\\SpikeAdapter\\bin\\image.ppm";
  //FileVisualReader* fileReader = new FileVisualReader(filename);
  //this->inputChannelDirectory->insert(std::pair<int, InputChannel*>(1, new VisualInputChannel(fileReader)));
}

void ChannelController::inputChannelSubscribe(int channelId)
{
  std::map<int, InputChannel*>::iterator iter = inputChannelDirectory->find(channelId);
  if (iter != inputChannelDirectory->end() )
  {
    iter->second->start();
  } else {
    std::cout << "Iterator is empty!" << std::endl;
  }
}
void ChannelController::outputChannelSubscribe(int channelId, std::vector<std::string> channelArguments)
{
  std::map<int, OutputChannel*>::iterator iter = outputChannelDirectory->find(channelId);
  if (iter != outputChannelDirectory->end() )
  {
    iter->second->start(channelArguments);
  } else {
    std::cout << "Iterator is empty!" << std::endl;
  }
}
