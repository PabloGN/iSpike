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
  int numOfNeurons;
  int degreeOfFreedom;
  double sd;
  double minAngle;
  double maxAngle;

public:

  JointInputChannel()
  {
    /**
     * First define the properties of this channel
     */
    std::map<std::string,Property*> properties;
    properties["Degree Of Freedom"] = new IntegerProperty(
          "Degree Of Freedom",
          0,
          "The degree of freedom to read from this joint"
        );
    properties["Standard Deviation"] = new DoubleProperty(
          "Standard Deviation",
          2,
          "The standard deviation to apply to the angles"
        );
    properties["Minimum Angle"] = new DoubleProperty(
          "Minimum Angle",
          -90,
          "The minimum angle to read"
        );
    properties["Maximum Angle"] = new DoubleProperty(
          "Maximum Angle",
          90,
          "The maximum angle to read"
        );
    properties["Number Of Neurons"] = new IntegerProperty(
          "Number Of Neurons",
          20,
          "The number of neurons to create"
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

  void initialise(AngleReader* reader)
  {
    initialise(reader, channelDescription->getChannelProperties());
  }

  /**
   * @param The reader where the image is retrieved from
   */
  void initialise(AngleReader* reader, std::map<std::string,Property*> properties);

  /**
   *  Initialises the channel and starts the conversion thread
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

  void step();

  void setThreadPointer(boost::shared_ptr<boost::thread> threadPointer)
  {
      this->threadPointer = threadPointer;
  }

  AngleReader* getReader() const
  {
      return reader;
  }

  void setReader(AngleReader *reader)
  {
      this->reader = reader;
  }

  bool isInitialised()
  {
    return this->initialised;
  }

};

#endif /* JOINTINPUTCHANNEL_HPP_ */
