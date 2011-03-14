/*
 * VisualInputChannel.hpp
 *
 *  Created on: 31 Dec 2010
 *      Author: cembo
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

public:

  /**
   * @param The reader where the image is retrieved from
   */
  VisualInputChannel(VisualReader* reader);

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

};

#endif /* VISUALINPUTCHANNEL_HPP_ */
