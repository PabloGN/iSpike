/*
 * JointOutputChannel.hpp
 *
 *  Created on: 22 Feb 2011
 *      Author: cembo
 */

#ifndef JOINTOUTPUTCHANNEL_HPP_
#define JOINTOUTPUTCHANNEL_HPP_

#include <iSpike/Channel/OutputChannel.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <queue>
#include <string>
#include <vector>
#include <iSpike/Writer/AngleWriter.hpp>

class JointOutputChannel : public OutputChannel {
private:
  std::queue< std::vector<int> >* buffer;
  AngleWriter* writer;
  void workerFunction();
  boost::shared_ptr<boost::thread> threadPointer;
  boost::condition wait_condition;
  boost::mutex mutex, wait_mutex;
  bool initialised;
  int numOfNeurons;
  double minAngle;
  double maxAngle;
  double rateOfDecay;

public:

  /**
   * Sets the current spike pattern
   */
  void setFiring(std::vector<int>* buffer);

  /**
   * Initialises the channel
   */
  void start(std::vector<std::string> arguments);

  AngleWriter* getWriter() const
  {
      return writer;
  }

  void setWriter(AngleWriter *writer)
  {
      this->writer = writer;
  }

  JointOutputChannel(AngleWriter* writer);

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
