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
#include <iSpike/ISpikeException.hpp>

void JointOutputChannel::setFiring(std::vector<int>* buffer)
{
  //boost::mutex::scoped_lock lock(this->mutex);
  //delete this->buffer;
  //this->buffer = new std::vector<int>(*buffer);
}

void JointOutputChannel::start()
{
  LOG(LOG_DEBUG) << "Starting JointOutputChannel";
  if(!initialised)
  {
    //this->buffer = new std::queue< std::vector<int> >();
    this->buffer = new std::vector<int>();
    this->writer->start();
    this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&JointOutputChannel::workerFunction, this))));
    initialised = true;
  }
}

/**
 * Updates the properties by first checking if any are read-only
 */
void JointOutputChannel::updateProperties(std::map<std::string,Property*> properties, bool updateReadOnly)
{
  for(std::map<std::string,Property*>::const_iterator iter = properties.begin(); iter != properties.end(); ++iter)
  {
    if(!updateReadOnly)
      ///Check if any of the properties are read only
      if(this->getChannelDescription().getChannelProperties()[iter->first]->isReadOnly())
        throw ISpikeException("Cannot update read-only parameters");

    ///Update the properties, this is ugly and should be improved
    std::string paramName = iter->second->getName();
    switch (iter->second->getType())
    {
      case Property::Integer:
      {
        int value = ((IntegerProperty*)(iter->second))->getValue();
        if (paramName == "Neuron Width")
          this->width = value;
        else if (paramName == "Neuron Height")
          this->height = value;
        break;
      }
      case Property::Double:
      {
        double value = ((DoubleProperty*)(iter->second))->getValue();
        if (paramName == "Minimum Angle")
          this->minAngle = value;
        else if (paramName == "Maximum Angle")
          this->maxAngle = value;
        else if (paramName == "Rate Of Decay")
          this->rateOfDecay = value;
        break;
      }
      case Property::Combo:
      case Property::String:
      {
        std::string value;
        if(iter->second->getType() == Property::String)
          value = ((StringProperty*)(iter->second))->getValue();
        else
          value = ((ComboProperty*)(iter->second))->getValue();
        break;
      }
    }
  }
}

void JointOutputChannel::initialise(AngleWriter* writer, std::map<std::string,Property*> properties)
{
  LOG(LOG_DEBUG) << "Entering JointOutputChannel initialisation";
  this->initialised = false;
  this->stopRequested = false;
  this->sleeping = false;
  this->setWriter(writer);
  this->updateProperties(properties, true);
}

void JointOutputChannel::step()
{
  while(!this->sleeping){}
	this->wait_condition.notify_all();
}

void JointOutputChannel::workerFunction()
{
  std::vector<double> variables(this->width * this->height,0);
  std::vector<int> times(this->width * this->height,0);
  while(!stopRequested)
  {
    LOG(LOG_DEBUG) << "Entering JointOutputChannel work loop, stopRequested is " << stopRequested;
    bool enoughFrames = false;
    {
      boost::mutex::scoped_lock lock(this->mutex);
      //enoughFrames = !(this->buffer->empty());
      enoughFrames = !(this->buffer->empty());
    }
    /**
     * Spikes received
     */
    if(enoughFrames)
    {
      //std::vector<int> currentFrame = this->buffer->front();
      std::vector<int> currentFrame = *(this->buffer);
      {
        boost::mutex::scoped_lock lock(this->mutex);
        //this->buffer->pop();
        this->buffer->clear();
      }
      for(unsigned int neuronID = 0; neuronID < currentFrame.size(); neuronID++)
      {
        variables[currentFrame[neuronID]]++;
        times[currentFrame[neuronID]] = 0;
      }
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
    /**
     * Decay the variables according to the following function:
     * N(t+1) = N(t)*e^(-rateOfDecay*t)
     */
    for(unsigned int i = 0; i < variables.size(); i++)
    {
      variables[i] = variables[i] * exp(-(this->rateOfDecay) * times[i]);
      //variableText << variables[i] << ", ";
      //fileStream << variables[i] << ",";
      if(times[i] < 100000)
        times[i]++;
    }
    if(!stopRequested)
    {
      LOG(LOG_DEBUG) << "JointOutputChannel: Falling asleep";
      boost::mutex::scoped_lock lk(this->wait_mutex);
      this->sleeping = true;
      this->wait_condition.wait(lk);
      this->sleeping = false;
      LOG(LOG_DEBUG) << "woke up!";
    }
  }
  LOG(LOG_INFO) << "JointOutputChannel: Exiting worker thread";
}
