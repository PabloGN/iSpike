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
  IzhikevichNeuronSim neuronSim(this->numOfNeurons, 0.1, 0.2, -65, 2);

  while(true)
  {
    //boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
    //boost::posix_time::time_duration start( time.time_of_day() );

    std::vector<double> angles = this->reader->getData();
    std::vector<double> voltages(this->numOfNeurons);
    if(angles.size() > 0)
    {
      std::cout << "Angle: " << angles[this->degreeOfFreedom] << std::endl;
      for(int i = 0; i < this->numOfNeurons; i++)
      {
        double currentAngle;
        if (i == 0)
          currentAngle = this->minAngle;
        else
          currentAngle = (this->maxAngle - this->minAngle) / (this->numOfNeurons-1) * i + this->minAngle;
        double main = 1 / sqrt(2 * boost::math::constants::pi<double>() * pow(this->sd,2));
        double exponent = pow((currentAngle - angles[this->degreeOfFreedom]),2) / (2 * pow(this->sd,2));
        voltages[i] = main * exp(-exponent);
      }

      boost::mutex::scoped_lock lock(this->mutex);
      std::vector<int>* spikes = neuronSim.getSpikes(&voltages);
      this->buffer->push_back(*spikes);
      delete spikes;
    }

    //time = boost::posix_time::microsec_clock::local_time();
    //boost::posix_time::time_duration finish( time.time_of_day() );
   // boost::this_thread::sleep(boost::posix_time::milliseconds(sleepAmount));
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
      this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&JointInputChannel::workerFunction, this))));
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
  this->numOfNeurons = ((IntegerProperty*)(properties["Number Of Neurons"]))->getValue();
  this->setReader(reader);
}
