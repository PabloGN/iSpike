#ifndef INPUTCHANNEL_H_
#define INPUTCHANNEL_H_

#include <boost/scoped_ptr.hpp>
#include <string>
#include <vector>
#include <iSpike/Channel/Channel.hpp>
#include <iSpike/Channel/InputChannel/InputChannelDescription.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

/**
 * @class InputChannel
 * @brief Input Channel Class
 *
 * This class represents a directed Channel from a signal source (eg vision)
 * to a spike output, so spikes can be read from this channel
 *
 * @author Edgars Lazdins
 *
 */
class InputChannel : public Channel {
protected:
  boost::scoped_ptr<InputChannelDescription> channelDescription;
  boost::condition_variable wait_condition;
  boost::mutex mutex, wait_mutex;
  virtual void updateProperties(std::map<std::string,Property*> properties, bool updateReadOnly) = 0;

public:

  virtual ~InputChannel()
  {
	  ;
  }

  /**
   * Retrieves the spike pattern
   */
  virtual std::vector< std::vector<int> > getFiring() = 0;

  /**
   * Initialised the channel
   */
  virtual void start() = 0;

  virtual void step() = 0;

  InputChannelDescription getChannelDescription()
  {
    return *(channelDescription);
  }
};

#endif /* INPUTCHANNEL_H_ */
