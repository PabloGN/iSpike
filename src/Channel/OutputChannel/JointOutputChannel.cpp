/*
 * JointOutputChannel.cpp
 *
 *  Created on: 23 Feb 2011
 *      Author: Edgars Lazdins
 */
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <iSpike/Log/Log.hpp>
#include <sstream>

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

void JointOutputChannel::initialise(AngleWriter* writer, std::map<std::string,Property*> properties)
{
  this->initialised = false;
  this->setWriter(writer);
  LOG(LOG_DEBUG) << "before properties";
  this->maxAngle = ((DoubleProperty*)(properties["Maximum Angle"]))->getValue();
  this->minAngle = ((DoubleProperty*)(properties["Minimum Angle"]))->getValue();
  this->rateOfDecay = ((DoubleProperty*)(properties["Rate Of Decay"]))->getValue();
  this->width = ((IntegerProperty*)(properties["Neuron Width"]))->getValue();
  this->height = ((IntegerProperty*)(properties["Neuron Height"]))->getValue();

}

void JointOutputChannel::step()
{
	this->wait_condition.notify_all();
}

void JointOutputChannel::workerFunction()
{
  std::vector<double> variables(this->width * this->height,0);
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
      std::ofstream fileStream;

      fileStream.open("variables.txt", std::fstream::out | std::fstream::app);
      std::ostringstream variableText;
      variableText << "Variables" << std::endl;
      variableText << "[";
      for(unsigned int i = 0; i < variables.size(); i++)
      {
        variables[i] = variables[i] * exp(-(this->rateOfDecay));
        variableText << variables[i] << ", ";
        fileStream << variables[i] << ",";
      }
      fileStream << std::endl;
      fileStream.close();
      variableText << "]";
      LOG(LOG_DEBUG) << variableText.str();
      double angleSum = 0;
      double weightSum = 0;
      for(unsigned int j = 0; j < variables.size(); j++)
      {
        double currentAngle = (this->maxAngle - this->minAngle) / ((this->width * this->height)-1) * j + this->minAngle;
        angleSum += variables[j] * currentAngle;
        weightSum += variables[j];
      }
      if(!weightSum == 0)
      {
        double angle = angleSum / weightSum;
        LOG(LOG_DEBUG) << "Angle: " << angle;
        this->currentAngle = angle;
        this->writer->addAngle(angle);
      }
    }
    boost::mutex::scoped_lock lk(this->wait_mutex);
    this->wait_condition.wait(lk);
  }
}
