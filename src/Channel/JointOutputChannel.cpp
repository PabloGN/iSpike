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

JointOutputChannel::JointOutputChannel(YarpAngleWriter* writer, double maxAngle, double minAngle, double rateOfDecay, double numOfNeurons)
{
  this->initialised = false;
  this->setWriter(writer);
  this->maxAngle = maxAngle;
  this->minAngle = minAngle;
  this->rateOfDecay = rateOfDecay;
  this->numOfNeurons = numOfNeurons;
}

void JointOutputChannel::workerFunction()
{
  int sleepAmount = 1;
  std::vector<double> variables(this->numOfNeurons,0);
  while(true)
  {
    bool enoughFrames = false;
    {
      boost::mutex::scoped_lock lock(this->mutex);
      enoughFrames = !(this->buffer->empty());
    }
    if(enoughFrames)
    {
      std::vector<int> currentFrame = this->buffer->front();
      {
        boost::mutex::scoped_lock lock(this->mutex);
        this->buffer->pop();
      }
      for(unsigned int neuronID = 0; neuronID < currentFrame.size(); neuronID++)
      {
        variables[currentFrame[neuronID]]++;
      }
      /**
       * Decay the variables according to the following function:
       * N(t+1) = N(t)*e^(-rateOfDecay)
       */
      std::cout << "Variables" << std::endl;
      std::cout << "[";
      for(unsigned int i = 0; i < variables.size(); i++)
      {
        variables[i] = variables[i] * exp(-(this->rateOfDecay));
        std::cout << variables[i] << ", ";
      }
      std::cout << "]" << std::endl;
      double angleSum = 0;
      double weightSum = 0;
      for(unsigned int j = 0; j < variables.size(); j++)
      {
        double currentAngle = (this->maxAngle - this->minAngle) / (this->numOfNeurons-1) * j + this->minAngle;
        angleSum += variables[j] * currentAngle;
        weightSum += variables[j];
      }
      if(!weightSum == 0)
      {
        double angle = angleSum / weightSum;
        std::cout << "Angle: " << angle << std::endl;
        this->writer->addAngle(angle);
      }
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(sleepAmount));
  }
}
