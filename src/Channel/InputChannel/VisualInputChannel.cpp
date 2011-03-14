#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>
#include <iSpike/VisualDataReducer/LogpolarVisualDataReducer.hpp>
#include <iSpike/VisualDataReducer/VisualDataReducer.hpp>
#include <iSpike/VisualFilter/DOGVisualFilter.hpp>
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <ios>
#include <boost/lexical_cast.hpp>

std::vector< std::vector<int> > VisualInputChannel::getFiring()
{
  boost::mutex::scoped_lock lock(this->mutex);
  std::vector< std::vector<int> > result = *(this->buffer);
  this->buffer->clear();
  return result;
}

void VisualInputChannel::workerFunction()
{
  std::cout << "The thread has started." << std::endl;
  while(true)
  {
    boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration start( time.time_of_day() );

    //std::cout << "Trying to retrieve the image..." << std::endl;
    Bitmap rPlusGMinus = this->filter->getRPlusGMinus();
    if(rPlusGMinus.getWidth() > 0)
    {
      boost::mutex::scoped_lock lock(this->mutex);
      std::vector<double>* voltages = new std::vector<double>(rPlusGMinus.getWidth() * rPlusGMinus.getHeight());
      for(int i = 0; i < rPlusGMinus.getWidth() * rPlusGMinus.getHeight(); i ++)
      {
        double voltage = (unsigned int)rPlusGMinus.getContents()[i];
        voltages->at(i) = voltage;
      }
      this->buffer->push_back(*(this->neuronSim->getSpikes(voltages)));
      delete voltages;
    }
    //std::cout << this->buffer->size() << std::endl;
    //boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    time = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration finish( time.time_of_day() );
    //boost::this_thread::sleep(boost::posix_time::microseconds(1000 - (finish.total_microseconds() - start.total_microseconds())));
    //std::cout << finish.total_microseconds() - start.total_microseconds() << "yo" << std::endl;
  }
}

void VisualInputChannel::start()
{
  if(!initialised)
  {
    this->buffer = new std::vector< std::vector<int> >();
    this->reader->start();
    this->dataReducer = new LogPolarVisualDataReducer(this->reader, 100);
    this->filter = new DOGVisualFilter(this->dataReducer, 100);
    this->neuronSim = new IzhikevichNeuronSim(320*240, 0.1, 0.2, -65, 2);
    this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&VisualInputChannel::workerFunction, this))));
    initialised = true;
  }
}

VisualInputChannel::VisualInputChannel(VisualReader* reader)
{
  this->initialised = false;
  this->setReader(reader);
}
