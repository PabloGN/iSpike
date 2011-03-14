#ifndef INPUTCHANNEL_H_
#define INPUTCHANNEL_H_
#include <string>
#include <vector>
#include <iSpike/Channel/Channel.hpp>

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
public:

  /**
   * Retrieves the spike pattern
   */
  virtual std::vector< std::vector<int> > getFiring() = 0;

  /**
   * Initialised the channel
   */
  virtual void start() = 0;

  virtual void step() = 0;
};

#endif /* INPUTCHANNEL_H_ */
