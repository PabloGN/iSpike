#ifndef OUTPUTCHANNEL_H_
#define OUTPUTCHANNEL_H_
#include <string>
#include <vector>
#include "Channel.hpp"

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

class OutputChannel : Channel {
public:

  /**
   * Sets the current spike pattern
   */
  virtual void setFiring(std::vector<int>* buffer) = 0;

  /**
   * Initialises the channel
   */
  virtual void start() = 0;
};

#endif /* OUTPUTCHANNEL_H_ */
