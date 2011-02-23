/*
 * JointOutputChannel.cpp
 *
 *  Created on: 23 Feb 2011
 *      Author: cembo
 */
#include <iSpike/Channel/JointOutputChannel.hpp>
#include <iostream>

void JointOutputChannel::setFiring(std::vector<int>* buffer)
{
  boost::mutex::scoped_lock lock(this->mutex);
  this->buffer->push(*buffer);
}

void JointOutputChannel::start()
{
  if(!initialised)
  {
    this->buffer = new std::queue< std::vector<int> >();
    this->writer->start();
    this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&JointOutputChannel::workerFunction, this))));
    initialised = true;
  }
}

JointOutputChannel::JointOutputChannel(YarpAngleWriter* writer)
{
  this->initialised = false;
  this->setWriter(writer);
}

void JointOutputChannel::workerFunction()
{
  unsigned int numOfFrames = 10;
  int numOfNeurons = 10;
  double minAngle = 0;
  double maxAngle = 90;
  unsigned int counter = 0;
  int sleepAmount = 1;
  while(true)
  {
    bool enoughFrames = false;
    {
      boost::mutex::scoped_lock lock(this->mutex);
      enoughFrames = this->buffer->size() >= numOfFrames;
    }
    if(enoughFrames)
    {
      std::vector<int> variables(numOfNeurons,0);
      for(int i = 0; i < numOfFrames; i++)
      {
        std::vector<int> currentFrame = this->buffer->front();
        {
          boost::mutex::scoped_lock lock(this->mutex);
          this->buffer->pop();
        }
        for(int neuronID = 0; neuronID < currentFrame.size(); neuronID++)
        {
          variables[currentFrame[neuronID]]++;
        }
      }
      double angleSum = 0;
      double weightSum = 0;
      for(int j = 0; j < variables.size(); j++)
      {
        double currentAngle = (maxAngle - minAngle) / (numOfNeurons-1) * j + minAngle;
        angleSum += variables[j] * currentAngle;
        weightSum += variables[j];
      }
      double angle = angleSum / weightSum;
      this->writer->addAngle(angle);
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(sleepAmount));
  }
}
