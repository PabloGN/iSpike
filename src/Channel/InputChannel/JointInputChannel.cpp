#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <ios>
#include <boost/lexical_cast.hpp>
#include <boost/math/constants/constants.hpp>
#include <iSpike/Property.hpp>
#include <map>
#include <iSpike/Log/Log.hpp>

std::vector< std::vector<int> > JointInputChannel::getFiring()
{
  boost::mutex::scoped_lock lock(this->mutex);
  std::vector< std::vector<int> > result = *(this->buffer);
  this->buffer->clear();
  return result;
}

void JointInputChannel::workerFunction()
{
  LOG(LOG_INFO) << "The thread has started.";
  
  IzhikevichNeuronSim neuronSim(this->width * this->height, this->parameterA, this->parameterB, this->parameterC, this->parameterD, this->currentFactor, this->constantCurrent);

  while(!stopRequested)
  {
    ///calculate the standard deviation as a percentage of the image
    ///3 standard deviations in each direction cover almost all of the range
    int totalNeurons = this->width * this->height;
    int angleDist = (this->maxAngle - this->minAngle) / totalNeurons;
    double standardDeviation = ((totalNeurons * this->sd) / 6) * angleDist;

    LOG(LOG_DEBUG) << "standard deviation: " << standardDeviation;

    std::vector<double> angles = this->reader->getData();
    std::vector<double> currents(this->width * this->height);
    if(angles.size() > 0)
    {
      LOG(LOG_DEBUG) << "Actual Read Angle: " << angles[this->degreeOfFreedom];
      this->currentAngle = angles[this->degreeOfFreedom];
      ///Iterate over the each neuron
      for(int i = 0; i < this->width * this->height; i++)
      {
        double currentAngle;
        if (i == 0)
          currentAngle = this->minAngle;
        else
        	///Generate the current angle by interpolating the angle range over the neuron count
        	currentAngle = (this->maxAngle - this->minAngle) / (this->width * this->height-1) * i + this->minAngle;
        ///Put a normal distribution at the centre of the observed angle with sd as defined earlier
        double main = 1 / sqrt(2 * boost::math::constants::pi<double>() * pow(standardDeviation,2));
        double exponent = pow((currentAngle - angles[this->degreeOfFreedom]),2) / (2 * pow(standardDeviation,2));
        ///Update the current map with the value for this angle
        currents[i] = main * exp(-exponent);
        LOG(LOG_DEBUG) << "Angle: " << currentAngle << " Current: " << main * exp(-exponent);
      }

      boost::mutex::scoped_lock lock(this->mutex);
      std::vector<int>* spikes = neuronSim.getSpikes(&currents);
      this->buffer->push_back(*spikes);
      delete spikes;
    }

    LOG(LOG_INFO) << "About to yield...";
    boost::mutex::scoped_lock lk(this->wait_mutex);
    this->wait_condition.wait(lk);
  }
  LOG(LOG_NOTICE) << "JointInputChannel: Exiting worker thread";
}

void JointInputChannel::step()
{
  this->wait_condition.notify_all();
}

void JointInputChannel::start()
{
  if(!initialised)
  {
      this->reader->start();
      this->threadPointer = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&JointInputChannel::workerFunction, this)));
      initialised = true;
      LOG(LOG_INFO) << "initialised";
  }
}

void JointInputChannel::initialise(AngleReader* reader, std::map<std::string,Property*> properties)
{
  this->initialised = false;
  this->buffer = new std::vector< std::vector<int> >();
  this->degreeOfFreedom = ((IntegerProperty*)(properties["Degree Of Freedom"]))->getValue();
  this->sd = ((DoubleProperty*)(properties["Standard Deviation"]))->getValue();
  this->minAngle = ((DoubleProperty*)(properties["Minimum Angle"]))->getValue();
  this->maxAngle = ((DoubleProperty*)(properties["Maximum Angle"]))->getValue();
  this->width = ((IntegerProperty*)(properties["Neuron Width"]))->getValue();
  this->height = ((IntegerProperty*)(properties["Neuron Height"]))->getValue();
  this->parameterA = ((DoubleProperty*)(properties["Parameter A"]))->getValue();
  this->parameterB = ((DoubleProperty*)(properties["Parameter B"]))->getValue();
  this->parameterC = ((DoubleProperty*)(properties["Parameter C"]))->getValue();
  this->parameterD = ((DoubleProperty*)(properties["Parameter D"]))->getValue();
  this->currentFactor = ((DoubleProperty*)(properties["Current Factor"]))->getValue();
  this->constantCurrent = ((DoubleProperty*)(properties["Constant Current"]))->getValue();
  this->reader = reader;
}
