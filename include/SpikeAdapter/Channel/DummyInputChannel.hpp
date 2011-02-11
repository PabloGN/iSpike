/*
 * DummyInputChannel.hpp
 *
 *  Created on: 31 Dec 2010
 *      Author: cembo
 */

#ifndef DUMMYINPUTCHANNEL_HPP_
#define DUMMYINPUTCHANNEL_HPP_

#include <string>
#include <vector>
#include <SpikeAdapter/Channel/InputChannel.hpp>
#include <SpikeAdapter/Reader/VisualReader.hpp>
#include <SpikeAdapter/VisualDataReducer/VisualDataReducer.hpp>
#include <SpikeAdapter/VisualFilter/VisualFilter.hpp>
#include <SpikeAdapter/NeuronSim/NeuronSim.hpp>
#include <boost/thread/mutex.hpp>

/**
 * @class DummyInputChannel
 * @brief DummyInputChannel class
 *
 * Temporary class that converts an image input into a spike pattern
 *
 * @author Edgars Lazdins
 *
 */
class DummyInputChannel : public InputChannel {

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

public:

  /**
   * @param The reader where the image is retrieved from
   */
  DummyInputChannel(VisualReader* reader);

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

};

#endif /* DUMMYINPUTCHANNEL_HPP_ */
