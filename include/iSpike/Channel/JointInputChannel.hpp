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
  YarpAngleReader* reader;
  void workerFunction();
  boost::shared_ptr<boost::thread> threadPointer;
  boost::mutex mutex;
  bool initialised;

public:

  /**
   * @param The reader where the image is retrieved from
   */
  JointInputChannel(YarpAngleReader* reader);

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

  YarpAngleReader* getReader() const
  {
      return reader;
  }

  void setReader(YarpAngleReader *reader)
  {
      this->reader = reader;
  }

};

#endif /* JOINTINPUTCHANNEL_HPP_ */
