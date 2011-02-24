#include <iSpike/Channel/JointInputChannel.hpp>
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <sys/time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <ios>
#include <time.h>

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
  timeval time;
  gettimeofday(&time,NULL);
  int numOfNeurons = 10;
  int degreeOfFreedom = 0;
  double sd = 2;
  double minAngle = -90;
  double maxAngle = 90;
  int sleepAmount = 1;
  IzhikevichNeuronSim neuronSim(numOfNeurons, 0.1, 0.2, -65, 2);

  while(true)
  {
    boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration start( time.time_of_day() );

    //std::cout << "Trying to retrieve the image..." << std::endl;
    std::vector<double> angles = this->reader->getData();
    std::vector<double> voltages(numOfNeurons);
    if(angles.size() > 0)
    {
      for(int i = 0; i < numOfNeurons; i++)
      {
        double currentAngle;
        if (i == 0)
          currentAngle = minAngle;
        else
          currentAngle = (maxAngle - minAngle) / (numOfNeurons-1) * i + minAngle;
        //std::cout << "Current Angle: " << currentAngle << std::endl;
        double main = 1 / sqrt(2 * M_PI * pow(sd,2));
        double exponent = pow((currentAngle - angles[degreeOfFreedom]),2) / (2 * pow(sd,2));
        voltages[i] = main * exp(-exponent);
      }
      /*for(int i = 0; i < voltages.size(); i ++)
        std::cout << voltages[i] << " ";
      std::cout << std::endl;*/

      boost::mutex::scoped_lock lock(this->mutex);
      std::vector<int>* spikes = neuronSim.getSpikes(&voltages);
      this->buffer->push_back(*spikes);
      delete spikes;
    }
    //std::cout << angles[0] << std::endl;
    //if(rPlusGMinus.getWidth() > 0)
    //{
      //boost::mutex::scoped_lock lock(this->mutex);
      //this->buffer->push_back(*(this->neuronSim->getSpikes(&rPlusGMinus)));
    //}
    //std::cout << this->buffer->size() << std::endl;
    //boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    time = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration finish( time.time_of_day() );
    boost::this_thread::sleep(boost::posix_time::milliseconds(sleepAmount));
    //boost::this_thread::sleep(boost::posix_time::microseconds(1000 - (finish.total_microseconds() - start.total_microseconds())));
    //std::cout << finish.total_microseconds() - start.total_microseconds() << "yo" << std::endl;
  }
}

void JointInputChannel::start()
{
  if(!initialised)
  {
    this->buffer = new std::vector< std::vector<int> >();
    this->reader->start();
    this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&JointInputChannel::workerFunction, this))));
    initialised = true;
    std::cout << "initialised" << std::endl;
  }
}

JointInputChannel::JointInputChannel(YarpAngleReader* reader)
{
  this->initialised = false;
  this->setReader(reader);
}
