/*
 * JointOutputChannel.hpp
 *
 *  Created on: 22 Feb 2011
 *      Author: cembo
 */

#ifndef JOINTOUTPUTCHANNEL_HPP_
#define JOINTOUTPUTCHANNEL_HPP_

#include <iSpike/Writer/YarpAngleWriter.hpp>
#include <iSpike/Channel/OutputChannel.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <queue>
#include <string>
#include <vector>

class JointOutputChannel : public OutputChannel {
private:
  std::queue< std::vector<int> >* buffer;
  YarpAngleWriter* writer;
  void workerFunction();
  boost::shared_ptr<boost::thread> threadPointer;
  boost::mutex mutex;
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

  YarpAngleWriter* getWriter() const
  {
      return writer;
  }

  void setWriter(YarpAngleWriter *writer)
  {
      this->writer = writer;
  }

  JointOutputChannel(YarpAngleWriter* writer);

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

};

#endif /* JOINTOUTPUTCHANNEL_HPP_ */
