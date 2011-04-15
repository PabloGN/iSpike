/*
 * JointInputChannel.hpp
 *
 *  Created on: 31 Dec 2010
 *      Author: Edgars Lazdins
 */

#ifndef JOINTINPUTCHANNEL_HPP_
#define JOINTINPUTCHANNEL_HPP_

#include <string>
#include <vector>
#include <iSpike/Channel/InputChannel/InputChannel.hpp>
#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/Channel/InputChannel/InputChannelDescription.hpp>
#include <map>

/**
 * @class JointInputChannel
 * @brief JointInputChannel class
 *
 * Class that converts a joint angle input into a spike pattern
 *
 * @author Edgars Lazdins
 *
 */
class JointInputChannel : public InputChannel {

private:
  std::vector< std::vector<int> >* buffer;
  AngleReader* reader;
  void workerFunction();
  boost::shared_ptr<boost::thread> threadPointer;
  bool initialised;
  bool stopRequested;
  bool sleeping;
  int degreeOfFreedom;
  double currentAngle;
  double sd;
  double minAngle;
  double maxAngle;
  double parameterA;
  double parameterB;
  double parameterC;
  double parameterD;
  double currentFactor;
  double constantCurrent;

protected:
  void updateProperties(std::map<std::string,Property*> properties, bool updateReadOnly);

public:

  double getCurrentAngle()
  {
	  return currentAngle;
  }

  /**
   * Default constructor, initialises the default channel properties
   */
  JointInputChannel()
  {
    /**
     * First define the properties of this channel
     */
    std::map<std::string,Property*> properties;
    this->initialised = false;
    properties["Degree Of Freedom"] = new IntegerProperty(
          "Degree Of Freedom",
          0,
          "The degree of freedom to read from this joint",
          true
        );
    properties["Standard Deviation"] = new DoubleProperty(
          "Standard Deviation",
          0.5,
          "The standard deviation as a percentage of neurons covered",
          true
        );
    properties["Minimum Angle"] = new DoubleProperty(
          "Minimum Angle",
          -90,
          "The minimum angle to read",
          true
        );
    properties["Maximum Angle"] = new DoubleProperty(
          "Maximum Angle",
          90,
          "The maximum angle to read",
          true
        );
    properties["Neuron Width"] = new IntegerProperty(
          "Neuron Width",
          10,
          "Width of the neuron network",
          true
        );
    properties["Neuron Height"] = new IntegerProperty(
          "Neuron Height",
          1,
          "Height of the neuron network",
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
          400,
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
          "Joint Input Channel",
          "This is a joint input channel",
          "Angle Reader",
          properties
        );
  }

  ~JointInputChannel()
  {
    LOG(LOG_DEBUG) << "Entering JointInputChannel destructor";
    if(this->initialised)
    {
      LOG(LOG_DEBUG) << "Setting stop requested to true";
      this->stopRequested = true;
      LOG(LOG_DEBUG) << "Waking up the thread";
      {
        this->wait_condition.notify_all();
      }
      LOG(LOG_DEBUG) << "Waiting";
      this->threadPointer->join();
      delete this->buffer;
      this->threadPointer.reset();
    }
    LOG(LOG_DEBUG) << "Exiting JointInputChannel destructor";
  }

  /**
   * Initialises the Joint Input Channel with the default parameters
   * @param reader The associated Angle Reader
   */
  void initialise(AngleReader* reader)
  {
    initialise(reader, channelDescription->getChannelProperties());
  }

  /**
   * Initialises the Joint Input Channel with the parameters provided
   * @param reader The associated Angle Reader
   * @param properties The initialisation property map
   */
  void initialise(AngleReader* reader, std::map<std::string,Property*> properties);

  /**
   *  Initialises the channel and starts the conversion thread
   */
  void start();

  /**
   * Retrieves the spike pattern
   * @return The spike pattern
   */
  std::vector< std::vector<int> > getFiring();

  /**
   * Steps the conversion loop
   */
  void step();

  /**
   * Returns the current initialisation status of this channel
   * @return Current initialisation status
   */
  bool isInitialised()
  {
    return this->initialised;
  }

};

#endif /* JOINTINPUTCHANNEL_HPP_ */
