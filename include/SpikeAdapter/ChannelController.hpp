/*
 * ChannelController.hpp
 *
 *  Created on: 7 Jan 2011
 *      Author: cembo
 */

#ifndef CHANNELCONTROLLER_HPP_
#define CHANNELCONTROLLER_HPP_

#include <map>
#include <SpikeAdapter/Channel/InputChannel.hpp>
#include <SpikeAdapter/Channel/OutputChannel.hpp>

class ChannelController
{
  private:
    std::map<int, InputChannel*>* inputChannelDirectory;
    std::map<int, OutputChannel*>* outputChannelDirectory;

  public:
    ChannelController();
    void inputChannelSubscribe(int channelId);
    void outputChannelSubscribe(int channelId);
    std::vector< std::vector<int> > getFiring(int channelId);
    void setFiring(int channelId, std::vector<int>* spikes);
    void getInputChannels();
    void getOutputChannels();

};

#endif /* CHANNELCONTROLLER_HPP_ */
