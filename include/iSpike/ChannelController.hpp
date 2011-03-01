/*
 * ChannelController.hpp
 *
 *  Created on: 7 Jan 2011
 *      Author: cembo
 */

#ifndef CHANNELCONTROLLER_HPP_
#define CHANNELCONTROLLER_HPP_

#include <map>
#include <vector>
#include <string>
#include <iSpike/Channel/InputChannel.hpp>
#include <iSpike/Channel/OutputChannel.hpp>

class ChannelController
{
  private:
    std::map<int, InputChannel*>* inputChannelDirectory;
    std::map<int, OutputChannel*>* outputChannelDirectory;

  public:
    ChannelController();
    void inputChannelSubscribe(int channelId, std::vector<std::string> arguments);
    void outputChannelSubscribe(int channelId, std::vector<std::string> arguments);
    std::vector< std::vector<int> > getFiring(int channelId);
    void setFiring(int channelId, std::vector<int>* spikes);
    void stepInputChannel(int channelId);
    void stepOutputChannel(int channelId);
    std::map<int, std::string>* getInputChannels();
    std::map<int, std::string>* getOutputChannels();

};

#endif /* CHANNELCONTROLLER_HPP_ */
