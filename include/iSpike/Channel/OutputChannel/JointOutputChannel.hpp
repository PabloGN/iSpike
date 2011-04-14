/*
 * JointOutputChannel.hpp
 *
 *  Created on: 22 Feb 2011
 *      Author: Edgars Lazdins
 */

#ifndef JOINTOUTPUTCHANNEL_HPP_
#define JOINTOUTPUTCHANNEL_HPP_

#include <iSpike/Channel/OutputChannel/OutputChannel.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <queue>
#include <string>
#include <vector>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/Log/Log.hpp>

class JointOutputChannel : public OutputChannel {
private:
  std::queue< std::vector<int> >* buffer;
  AngleWriter* writer;
  void workerFunction();
  boost::shared_ptr<boost::thread> threadPointer;
  boost::condition_variable wait_condition;
  boost::mutex mutex, wait_mutex;
  bool initialised;
  double minAngle;
  double maxAngle;
  double rateOfDecay;
  double currentAngle;
  bool stopRequested;
  bool sleeping;

public:

	double getCurrentAngle()
	{
		  return currentAngle;
	}

  JointOutputChannel()
  {
    /**
     * First define the properties of this channel
     */
    std::map<std::string,Property*> properties;
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
    properties["Rate Of Decay"] = new DoubleProperty(
          "Rate Of Decay",
          0.005,
          "The rate of decay of the angle variables"
        );
    properties["Neuron Width"] = new IntegerProperty(
          "Neuron Width",
          10,
          "Width of the neuron network"
        );
    properties["Neuron Height"] = new IntegerProperty(
          "Neuron Height",
          1,
          "Height of the neuron network"
        );
    /**
     * Now let's create the description
     */
    this->channelDescription = new OutputChannelDescription(
          "Joint Output Channel",
          "This channel converts a pattern of spikes into an angle and writes it",
          "Angle Writer",
          properties
        );
  }

  ~JointOutputChannel()
  {
    LOG(LOG_DEBUG) << "Entering JointOutputChannel destructor";
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
    LOG(LOG_DEBUG) << "Exiting JointOutputChannel destructor";
  }

  /**
   * Sets the current spike pattern
   */
  void setFiring(std::vector<int>* buffer);

  /**
   * Initialises the channel
   */
  void start();

  AngleWriter* getWriter() const
  {
      return writer;
  }

  void setWriter(AngleWriter *writer)
  {
      this->writer = writer;
  }

  void initialise(AngleWriter* writer)
  {
    initialise(writer, channelDescription->getChannelProperties());
  }

  void initialise(AngleWriter* writer, std::map<std::string,Property*> properties);

  boost::shared_ptr<boost::thread> getThreadPointer()
  {
      return threadPointer;
  }

  void setThreadPointer(boost::shared_ptr<boost::thread> threadPointer)
  {
      this->threadPointer = threadPointer;
  }

  bool isInitialised()
  {
    return this->initialised;
  }

  void step();

};

#endif /* JOINTOUTPUTCHANNEL_HPP_ */
