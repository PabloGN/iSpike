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
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>

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
  LOG(LOG_INFO) << "The thread has started.";
  while(!stopRequested)
  {
    ///Retrieve the colour oponent image
    Bitmap* opponentMap = new Bitmap(this->filter->getOpponencyMap());
    ///If got it
    if(opponentMap->getWidth() > 0)
    {
      LOG(LOG_DEBUG) << "Got an opponentmap, width: " << opponentMap->getWidth();
      ///Lock so that we can update the buffer
      boost::mutex::scoped_lock lock(this->mutex);
      //std::vector<double>* voltages = new std::vector<double>(rPlusGMinus.getWidth() * rPlusGMinus.getHeight());
      ///create a current map for the neuron simulator
      std::vector<double>* currents = new std::vector<double>(this->width * this->height);
      for(int i = 0; i < this->width; i++)
        for(int j = 0; j < this->height; j++)
        {
          //double voltage = (unsigned int)rPlusGMinus.getContents()[i];
          ///retrieve the pixel intensity at the coordinates
		  //LOG(LOG_DEBUG) << "Current " << i << " " << j << " " << (unsigned int)((opponentMap->getPixel(this->xOffset + i,this->yOffset + j)));
          double current = (unsigned int)opponentMap->getPixel(this->xOffset + i,this->yOffset + j);
          ///move it to the current map
          currents->at(j*(this->width) + i) = current;
        }
      this->buffer->push_back(*(this->neuronSim->getSpikes(currents)));
      delete currents;
    }
    if(!stopRequested)
    {
      LOG(LOG_DEBUG) << "VisualInputChannel: Falling asleep...";
      boost::mutex::scoped_lock lk(this->wait_mutex);
      this->sleeping = true;
      this->wait_condition.wait(lk);
      this->sleeping = false;
    }
  }
}

void VisualInputChannel::start()
{
  if(!initialised)
  {
    this->buffer = new std::vector< std::vector<int> >();
    this->reader->start();
    this->dataReducer = new LogPolarVisualDataReducer(this->reader, 10, this->getWidth(), this->getHeight());
	this->filter = new DOGVisualFilter(this->dataReducer, 10, plusSigma, minusSigma, ratio1, ratio2, this->opponentMap);
    this->movementFilter = new MovementFilter(this->filter, 1);
    this->neuronSim = new IzhikevichNeuronSim(this->width * this->height, this->parameterA, this->parameterB, this->parameterC, this->parameterD, this->currentFactor, this->constantCurrent);
    this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&VisualInputChannel::workerFunction, this))));
    initialised = true;
    LOG(LOG_DEBUG) << "X OFFSET: " << this->xOffset << " Y OFFSET: " << this->yOffset;
  }
}

/**
 * Updates the properties by first checking if any are read-only
 */
void VisualInputChannel::updateProperties(std::map<std::string,Property*> properties, bool updateReadOnly)
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
        if(paramName == "Opponency Map")
          this->opponentMap = value;
        else if (paramName == "Neuron Width")
          this->width = value;
        else if (paramName == "Neuron Height")
          this->height = value;
        else if (paramName == "Image Offset X")
          this->xOffset = value;
        else if (paramName == "Image Offset Y")
          this->yOffset = value;
        break;
      }
      case Property::Double:
      {
        double value = ((DoubleProperty*)(iter->second))->getValue();
        if (paramName == "Parameter A")
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
		else if (paramName == "Plus Sigma")
			this->plusSigma = value;
		else if (paramName == "Minus Sigma")
			this->minusSigma = value;
		else if (paramName == "Ratio 1")
			this->ratio1 = value;
		else if (paramName == "Ratio 2")
			this->ratio2 = value;

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

/**
 * Initialises the properties of this channel
 */
void VisualInputChannel::initialise(VisualReader* reader, std::map<std::string,Property*> properties)
{
  this->initialised = false;
  this->setReader(reader);
  this->stopRequested = false;
  this->sleeping = false;
  this->updateProperties(properties, true);
}

void VisualInputChannel::step()
{
  while(!this->sleeping){}
  this->wait_condition.notify_all();
}
