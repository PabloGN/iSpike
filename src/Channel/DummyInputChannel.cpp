#include <SpikeAdapter/Channel/DummyInputChannel.hpp>
#include <SpikeAdapter/VisualDataReducer/LogpolarVisualDataReducer.hpp>
#include <SpikeAdapter/VisualDataReducer/VisualDataReducer.hpp>
#include <SpikeAdapter/VisualFilter/DOGVisualFilter.hpp>
#include <SpikeAdapter/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>
#include <sys/time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <ios>
#include <time.h>

std::vector< std::vector<int> > DummyInputChannel::getFiring()
{
  boost::mutex::scoped_lock lock(this->mutex);
  std::vector< std::vector<int> > result = *(this->buffer);
  this->buffer->clear();
  return result;
}

void DummyInputChannel::workerFunction()
{
  std::cout << "The thread has started." << std::endl;
  timeval time;
  gettimeofday(&time,NULL);
  while(true)
  {
    boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration start( time.time_of_day() );

    //std::cout << "Trying to retrieve the image..." << std::endl;
    Bitmap rPlusGMinus = this->filter->getRPlusGMinus();
    if(rPlusGMinus.getWidth() > 0)
    {
      boost::mutex::scoped_lock lock(this->mutex);
      this->buffer->push_back(*(this->neuronSim->getSpikes(&rPlusGMinus)));
    }
    //std::cout << this->buffer->size() << std::endl;
    //boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    time = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration finish( time.time_of_day() );
    //boost::this_thread::sleep(boost::posix_time::microseconds(1000 - (finish.total_microseconds() - start.total_microseconds())));
    //std::cout << finish.total_microseconds() - start.total_microseconds() << "yo" << std::endl;
  }
}

void DummyInputChannel::start()
{
  if(!initialised)
  {
    this->buffer = new std::vector< std::vector<int> >();
    this->reader->start();
    this->dataReducer = new LogPolarVisualDataReducer(this->reader, 100);
    this->filter = new DOGVisualFilter(this->dataReducer, 100);
    this->neuronSim = new IzhikevichNeuronSim(320*240, 0.1, 0.2, -65, 2);
    this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&DummyInputChannel::workerFunction, this))));
    initialised = true;
  }
}

DummyInputChannel::DummyInputChannel(VisualReader* reader)
{
  this->initialised = false;
  this->setReader(reader);
}
