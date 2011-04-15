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
#include <iSpike/VisualFilter/MovementFilter.hpp>

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
  MovementFilter* movementFilter;
  NeuronSim* neuronSim;
  double parameterA;
  double parameterB;
  double parameterC;
  double parameterD;
  double currentFactor;
  double constantCurrent;
  int xOffset;
  int yOffset;
  int opponentMap;

protected:
  void updateProperties(std::map<std::string,Property*> properties, bool updateReadOnly);

public:

  /**
   * Default constructor, initialises the default parameters
   */
  VisualInputChannel()
  {
    /**
     * First define the properties of this channel
     */
    std::map<std::string,Property*> properties;
    this->initialised = false;
    properties["Image Offset X"] = new IntegerProperty(
          "Image Offset X",
          0,
          "Horizontal offset in the colour oponent image",
          true
        );
    properties["Image Offset Y"] = new IntegerProperty(
          "Image Offset Y",
          0,
          "Vertical offset in the colour oponent image",
          true
        );
    properties["Neuron Width"] = new IntegerProperty(
          "Neuron Width",
          320,
          "Width of the neuron network",
          true
        );
    properties["Neuron Height"] = new IntegerProperty(
          "Neuron Height",
          240,
          "Height of the neuron network",
          true
        );
    properties["Opponency Map"] = new IntegerProperty(
          "Opponency Map",
          0,
          "Which colour oponency map to use (0 = R+G-; 1 = G+R-; 2 = B+Y-)",
          true
        );
    properties["Parameter A"] = new DoubleProperty(
          "Parameter A",
          0.1,
          "Parameter A of the Izhikevich Neuron Model",
          true
        );
    properties["Parameter B"] = new DoubleProperty(
          "Parameter B",
          0.2,
          "Parameter B of the Izhikevich Neuron Model",
          true
        );
    properties["Parameter C"] = new DoubleProperty(
          "Parameter C",
          -65,
          "Parameter C of the Izhikevich Neuron Model",
          true
        );
    properties["Parameter D"] = new DoubleProperty(
          "Parameter D",
          2,
          "Parameter D of the Izhikevich Neuron Model",
          true
        );
    properties["Current Factor"] = new DoubleProperty(
          "Current Factor",
          20,
          "Incoming current is multiplied by this value",
          true
        );
    properties["Constant Current"] = new DoubleProperty(
          "Constant Current",
          0,
          "This value is added to the incoming current",
          true
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

  ~VisualInputChannel()
  {
    if(this->initialised)
    {
      this->threadPointer->interrupt();
      this->threadPointer->join();
      delete this->threadPointer.get();
      delete this->buffer;
    }
  }

  /**
   * Initialises the channel with the default parameters
   */
  void initialise(VisualReader* reader)
  {
    initialise(reader, channelDescription->getChannelProperties());
  }

  /**
   * @param reader The reader where the image is retrieved from
   * @param properties tThe initialisation properties
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
