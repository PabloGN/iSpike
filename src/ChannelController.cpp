/*
 * ChannelController.cpp
 *
 *  Created on: 7 Jan 2011
 *      Author: cembo
 */
#include <string>
#include <iostream>
#include <iSpike/ChannelController.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Reader/FileVisualReader.hpp>
#include <iSpike/Reader/YarpVisualReader.hpp>
#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/Channel/VisualInputChannel.hpp>
#include <iSpike/Channel/JointInputChannel.hpp>
#include <iSpike/Channel/JointOutputChannel.hpp>
#include <iSpike/Writer/YarpAngleWriter.hpp>

void ChannelController::getInputChannels()
{

}

void ChannelController::getOutputChannels()
{

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
  YarpAngleReader* jointReader = new YarpAngleReader("/icubSim/left_arm/state:o");
  this->inputChannelDirectory->insert(std::pair<int, InputChannel*>(1, new JointInputChannel(jointReader)));
  YarpAngleWriter* jointWriter = new YarpAngleWriter("/icubSim/left_arm/rpc:i");
  this->outputChannelDirectory->insert(std::pair<int, OutputChannel*>(1, new JointOutputChannel(jointWriter)));
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
void ChannelController::outputChannelSubscribe(int channelId)
{
  std::map<int, OutputChannel*>::iterator iter = outputChannelDirectory->find(channelId);
  if (iter != outputChannelDirectory->end() )
  {
    iter->second->start();
  } else {
    std::cout << "Iterator is empty!" << std::endl;
  }
}
