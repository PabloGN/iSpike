/*
 * JointInputChannel.hpp
 *
 *  Created on: 31 Dec 2010
 *      Author: cembo
 */

#ifndef JOINTINPUTCHANNEL_HPP_
#define JOINTINPUTCHANNEL_HPP_

#include <string>
#include <vector>
#include <iSpike/Channel/InputChannel.hpp>
#include <iSpike/Reader/YarpAngleReader.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>


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
  boost::condition wait_condition;
  boost::mutex mutex, wait_mutex;
  bool initialised;
  int numOfNeurons;
  int degreeOfFreedom;
  double sd;
  double minAngle;
  double maxAngle;

public:

  /**
   * @param The reader where the image is retrieved from
   */
  JointInputChannel(AngleReader* reader);

  /**
   *  Initialises the channel and starts the conversion thread
   */
  void start(std::vector<std::string> arguments);

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
