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
#include <boost/timer.hpp>
#include <iSpike/ISpikeException.hpp>


std::vector< std::vector<int> > JointInputChannel::getFiring()
{
  boost::mutex::scoped_lock lock(this->mutex);
  std::vector< std::vector<int> > result = *(this->buffer);
  this->buffer->clear();
  return result;
}

void JointInputChannel::workerFunction()
{
  LOG(LOG_INFO) << "The thread has started. " << stopRequested;
  
  IzhikevichNeuronSim neuronSim(this->width * this->height, this->parameterA, this->parameterB, this->parameterC, this->parameterD, this->currentFactor, this->constantCurrent);
  std::ofstream timeStream;
  //timeStream.open("timings.txt", std::fstream::out | std::fstream::app);
  while(!stopRequested)
  {
    boost::timer t;
    ///calculate the standard deviation as a percentage of the image
    ///3 standard deviations in each direction cover almost all of the range
    int totalNeurons = this->width * this->height;
    int angleDist = (this->maxAngle - this->minAngle) / totalNeurons;
    double standardDeviation = ((totalNeurons * this->sd) / 6) * angleDist;

    LOG(LOG_INFO) << "standard deviation: " << standardDeviation;

    std::vector<double> angles = this->reader->getData();
    std::vector<double> currents(this->width * this->height);
    if(angles.size() > 0)
    {
      double initial = t.elapsed();
      LOG(LOG_INFO) << "Actual Read Angle: " << angles[this->degreeOfFreedom];
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
        /*std::ostringstream currentText;
        currentText << "Angle: " << currentAngle << " Currents: ";
        for(unsigned int n = 0; n < currents.size(); n++)
        {
          currentText << currents[n] << ", ";
        }
        LOG(LOG_DEBUG) << currentText.str();*/
        LOG(LOG_DEBUG) << "Angle: " << currentAngle << " Current: " << main * exp(-exponent);
        /*std::ofstream currentStream;
        currentStream.open("currents.txt", std::fstream::out | std::fstream::app);
        for(unsigned int n = 0; n < currents.size(); n++)
        {
          currentStream << currents[n] << ", ";
        }
        currentStream << std::endl;
        currentStream.close();*/
      }
      double prespikes = t.elapsed() - initial;
      boost::mutex::scoped_lock lock(this->mutex);
      std::vector<int>* spikes = neuronSim.getSpikes(&currents);
      double postspikes = t.elapsed() - prespikes;
      this->buffer->push_back(*spikes);
      delete spikes;
      //timeStream << this->width << "," << prespikes << "," << postspikes << std::endl;
    }
    if(!stopRequested)
    {
      LOG(LOG_DEBUG) << "JointInputChannel: Falling asleep...";
      boost::mutex::scoped_lock lk(this->wait_mutex);
      this->sleeping = true;
      this->wait_condition.wait(lk);
      this->sleeping = false;
    }
  }
  //timeStream.close();
  LOG(LOG_INFO) << "JointInputChannel: Exiting worker thread";
}

void JointInputChannel::step()
{
  while(!this->sleeping){}
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

/**
 * Updates the properties by first checking if any are read-only
 */
void JointInputChannel::updateProperties(std::map<std::string,Property*> properties, bool updateReadOnly)
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
        if(paramName == "Degree Of Freedom")
          this->degreeOfFreedom = value;
        else if (paramName == "Neuron Width")
          this->width = value;
        else if (paramName == "Neuron Height")
          this->height = value;
        break;
      }
      case Property::Double:
      {
        double value = ((DoubleProperty*)(iter->second))->getValue();
        if(paramName == "Standard Deviation")
          this->sd = value;
        else if (paramName == "Minimum Angle")
          this->minAngle = value;
        else if (paramName == "Maximum Angle")
          this->maxAngle = value;
        else if (paramName == "Parameter A")
          this->parameterA = value;
        else if (paramName == "Parameter B")
          this->parameterB = value;
        else if (paramName == "Parameter C")
          this->parameterC = value;
        else if (paramName == "Parameter D")
          this->parameterD = value;
        else if (paramName == "Current Factor")
          this->currentFactor = value;
        else if (paramName == "Constant Current")
          this->constantCurrent = value;
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

void JointInputChannel::initialise(AngleReader* reader, std::map<std::string,Property*> properties)
{
  this->initialised = false;
  this->buffer = new std::vector< std::vector<int> >();
  this->reader = reader;
  this->stopRequested = false;
  this->sleeping = false;
  this->updateProperties(properties, true);
}
