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

/**
 * Worker loop, retrieves the colour oponent map and gets the spikes from a neuron simulator
 */
void VisualInputChannel::workerFunction()
{
  std::cout << "The thread has started." << std::endl;
  while(true)
  {
    //std::cout << "Trying to retrieve the image..." << std::endl;
    ///Retrieve the colour oponent image
    Bitmap opponentMap(0, 0, 0, NULL);
    if(this->opponentMap == 0)
      opponentMap = this->filter->getRPlusGMinus();
    else if (this->opponentMap == 1)
      opponentMap = this->filter->getGPlusRMinus();
    else if(this->opponentMap == 2)
      opponentMap = this->filter->getBPlusYMinus();
    ///If got it
    if(opponentMap.getWidth() > 0)
    {
      ///Lock so that we can update the buffer
      boost::mutex::scoped_lock lock(this->mutex);
      //std::vector<double>* voltages = new std::vector<double>(rPlusGMinus.getWidth() * rPlusGMinus.getHeight());
      ///create a current map for the neuron simulator
      std::vector<double>* currents = new std::vector<double>(this->width * this->height);
      for(int i = 0; i < this->width; i++)
        for(int j = 0; i < this->height; j++)
        {
          //double voltage = (unsigned int)rPlusGMinus.getContents()[i];
          ///retrieve the pixel intensity at the coordinates
          double current = (unsigned int)opponentMap.getPixel(this->xOffset + i,this->yOffset + j);
          ///move it to the current map
          currents->at(j*(this->width) + i) = current;
        }
      this->buffer->push_back(*(this->neuronSim->getSpikes(currents)));
      delete currents;
    }
    std::cout << "About to yield..." << std::endl;
    boost::mutex::scoped_lock lk(this->wait_mutex);
    this->wait_condition.wait(lk);
  }
}

void VisualInputChannel::start()
{
  if(!initialised)
  {
    this->buffer = new std::vector< std::vector<int> >();
    this->reader->start();
    this->dataReducer = new LogPolarVisualDataReducer(this->reader, 100);
<<<<<<< .mine
    this->filter = new DOGVisualFilter(this->dataReducer, 100);
    this->neuronSim = new IzhikevichNeuronSim(320*240, 0.1, 0.2, -65, 2, 20, 0);
=======
    this->filter = new DOGVisualFilter(this->dataReducer, 100, 3, 2);
    this->neuronSim = new IzhikevichNeuronSim(this->width * this->height, this->parameterA, this->parameterB, this->parameterC, this->parameterD);
>>>>>>> .r48
    this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&VisualInputChannel::workerFunction, this))));
    initialised = true;
  }
}

/**
 * Initialises the properties of this channel
 */
void VisualInputChannel::initialise(VisualReader* reader, std::map<std::string,Property*> properties)
{
  this->initialised = false;
  this->setReader(reader);
  this->parameterA = ((DoubleProperty*)(properties["Parameter A"]))->getValue();
  this->parameterB = ((DoubleProperty*)(properties["Parameter B"]))->getValue();
  this->parameterC = ((DoubleProperty*)(properties["Parameter C"]))->getValue();
  this->parameterD = ((DoubleProperty*)(properties["Parameter D"]))->getValue();
  this->opponentMap = ((IntegerProperty*)(properties["Opponency Map"]))->getValue();
  this->width = ((IntegerProperty*)(properties["Neuron Width"]))->getValue();
  this->height = ((IntegerProperty*)(properties["Neuron Height"]))->getValue();
  this->xOffset = ((IntegerProperty*)(properties["Image Offset X"]))->getValue();
  this->yOffset = ((IntegerProperty*)(properties["Image Offset Y"]))->getValue();
}

void VisualInputChannel::step()
{
  this->wait_condition.notify_all();
}
