/*
 * YarpAngleReader.cpp
 *
 *  Created on: 22 Feb 2011
 *      Author: cembo
 */

#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <iSpike/YarpConnection.hpp>

std::vector<double> YarpAngleReader::getData()
{
  boost::mutex::scoped_lock lock(this->mutex);
  return *(this->buffer);
}

YarpAngleReader::YarpAngleReader(std::string portName)
{
  this->setPortName(portName);
  this->buffer = new std::vector<double>();
  this->initialised = false;
}

void YarpAngleReader::workerFunction()
{
  std::map<std::string, YarpPortDetails*>* portMap = YarpInterface::Instance()->getPortMap();
  std::map<std::string, YarpPortDetails*>::iterator iter = portMap->find(this->getPortName());
  std::string ip;
  std::string port;
  std::string type;
  if (iter != portMap->end() )
  {
      ip = iter->second->getIp();
      port = iter->second->getPort();
      type = iter->second->getType();
      std::cout << "IP: " << ip << std::endl;
  } else {
    std::cout << "Iterator is empty!" << std::endl;
  }
  YarpInterface::Instance()->connect_to_port(ip, port);
  YarpInterface::Instance()->prepare_to_read_text();
  while(true)
  {
    std::string contentLine = YarpInterface::Instance()->read_until("\n");
    if(contentLine.length() > 0 && contentLine.substr(0,2) != "do")
    {
      std::vector<double> *angles = new std::vector<double>();
      boost::regex split_string(" ");
      std::list<std::string> lines;
      boost::regex_split(std::back_inserter(lines), contentLine, split_string);
      while(lines.size() > 1)
      {
        std::string current_string = *(lines.begin());
        lines.pop_front();
        try
        {
        double angle = boost::lexical_cast<double>(current_string);
        angles->push_back(angle);
        }
        catch(int i)
        {
            std::cout << "could not convert " << current_string << " to double" << std::endl;
            break;
        }
      }
      boost::mutex::scoped_lock lock(this->mutex);
      delete this->buffer;
      this->buffer = angles;
    }
  }
}

void YarpAngleReader::start()
{
  if(!initialised)
  {
    this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpAngleReader::workerFunction, this))));
    initialised = true;
  }
}

