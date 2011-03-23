/*
 * VisualInputChannel.hpp
 *
 *  Created on: 31 Dec 2010
 *      Author: Edgars Lazdins
 */

#ifndef VISUALINPUTCHANNEL_HPP_
#define VISUALINPUTCHANNEL_HPP_

#include <string>
#include <vector>
#include <iSpike/Channel/InputChannel/InputChannel.hpp>
#include <iSpike/Reader/VisualReader.hpp>
#include <iSpike/VisualDataReducer/VisualDataReducer.hpp>
#include <iSpike/VisualFilter/VisualFilter.hpp>
#include <iSpike/NeuronSim/NeuronSim.hpp>
#include <iSpike/Property.hpp>
#include <boost/thread/mutex.hpp>

/**
 * @class VisualInputChannel
 * @brief VisualInputChannel class
 *
 * Class that converts an image input into a spike pattern
 *
 * @author Edgars Lazdins
 *
 */
class VisualInputChannel : public InputChannel {

private:
  std::vector< std::vector<int> >* buffer;
  VisualReader* reader;
  void workerFunction();
  boost::shared_ptr<boost::thread> threadPointer;
  boost::mutex mutex;
  bool initialised;
  VisualDataReducer* dataReducer;
  VisualFilter* filter;
  NeuronSim* neuronSim;
  double parameterA;
  double parameterB;
  double parameterC;
  double parameterD;
  int numOfNeurons;

public:

  VisualInputChannel()
  {
    /**
     * First define the properties of this channel
     */
    std::map<std::string,Property*> properties;
    properties["Number Of Neurons"] = new IntegerProperty(
          "Number Of Neurons",
          76800,
          "The number of neurons to create"
        );
    properties["Parameter A"] = new DoubleProperty(
          "Parameter A",
          0.1,
          "Parameter A of the Izhikevich Neuron Model"
        );
    properties["Parameter B"] = new DoubleProperty(
          "Parameter B",
          0.2,
          "Parameter B of the Izhikevich Neuron Model"
        );
    properties["Parameter C"] = new DoubleProperty(
          "Parameter C",
          -65,
          "Parameter C of the Izhikevich Neuron Model"
        );
    properties["Parameter D"] = new DoubleProperty(
          "Parameter D",
          2,
          "Parameter D of the Izhikevich Neuron Model"
        );
    /**
     * Now let's create the description
     */
    this->channelDescription = new InputChannelDescription(
          "Visual Input Channel",
          "This is a visual input channel",
          "Visual Reader",
          properties
        );
  }

  /*
   * Initialises the channel with the default parameters
   */
  void initialise(VisualReader* reader)
  {
    initialise(reader, channelDescription->getChannelProperties());
  }

  /**
   * @param The reader where the image is retrieved from
   */
  void initialise(VisualReader* reader, std::map<std::string,Property*> properties);

  /**
   *  Initialised the channel and starts the conversion thread
   */
  void start();

  /**
   * Retrieves the spike pattern
   */
  std::vector< std::vector<int> > getFiring();

  boost::shared_ptr<boost::thread> getThreadPointer()
  {
      return threadPointer;
  }

  void setThreadPointer(boost::shared_ptr<boost::thread> threadPointer)
  {
      this->threadPointer = threadPointer;
  }

  VisualReader* getReader() const
  {
      return reader;
  }

  void setReader(VisualReader *reader)
  {
      this->reader = reader;
  }

  bool isInitialised()
  {
    return this->initialised;
  }

  void step();

};

#endif /* VISUALINPUTCHANNEL_HPP_ */
