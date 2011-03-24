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

std::vector< std::vector<int> > JointInputChannel::getFiring()
{
  boost::mutex::scoped_lock lock(this->mutex);
  std::vector< std::vector<int> > result = *(this->buffer);
  this->buffer->clear();
  return result;
}

void JointInputChannel::workerFunction()
{
  std::cout << "The thread has started." << std::endl;
  
  IzhikevichNeuronSim neuronSim(this->numOfNeurons, 0.1, 0.2, -65, 2, 20, 0);

  while(true)
  {
    ///calculate the standard deviation as a percentage of the image
    ///3 standard deviations in each direction cover almost all of the range
    int totalNeurons = this->width * this->height;
    double standardDeviation = (totalNeurons * this->sd) / 6;

    std::vector<double> angles = this->reader->getData();
    std::vector<double> currents(this->width * this->height);
    if(angles.size() > 0)
    {
      std::cout << "Angle: " << angles[this->degreeOfFreedom] << std::endl;
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
        std::cout << "Angle: " << currentAngle << "Current: " << main * exp(-exponent) << std::endl;
      }

      boost::mutex::scoped_lock lock(this->mutex);
      std::vector<int>* spikes = neuronSim.getSpikes(&currents);
      this->buffer->push_back(*spikes);
      delete spikes;
    }

    std::cout << "About to yield..." << std::endl;
    boost::mutex::scoped_lock lk(this->wait_mutex);
    this->wait_condition.wait(lk);
  }
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
      std::cout << "initialised" << std::endl;
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
  this->reader = reader;
}
