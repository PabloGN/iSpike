#ifndef OUTPUTCHANNEL_H_
#define OUTPUTCHANNEL_H_
#include <string>
#include <vector>
#include <iSpike/Channel/Channel.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <iSpike/Channel/OutputChannel/OutputChannelDescription.hpp>

/**
 * @class OutputChannel
 * @brief Output Channel class
 *
 * This class represents a Channel that can be written to e.g. it is possible
 * to send spikes to this channel, which are decoded into something else
 *
 * @author Edgars Lazdins
 *
 */

class OutputChannel : public Channel {
protected:
  OutputChannelDescription* channelDescription;
  boost::condition wait_condition;
  boost::mutex mutex, wait_mutex;
public:

  virtual ~OutputChannel()
  {
    delete this->channelDescription;
  }

  /**
   * Sets the current spike pattern
   */
  virtual void setFiring(std::vector<int>* buffer) = 0;

  /**
   * Initialises the channel
   */
  virtual void start() = 0;

  virtual void step() = 0;

  OutputChannelDescription getChannelDescription()
  {
    return *(channelDescription);
  }
};

#endif /* OUTPUTCHANNEL_H_ */
