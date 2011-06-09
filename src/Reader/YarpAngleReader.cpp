/*
 * YarpAngleReader.cpp
 *
 *  Created on: 22 Feb 2011
 *      Author: Edgars Lazdins
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
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>

std::vector<double> YarpAngleReader::getData()
{
  boost::mutex::scoped_lock lock(this->mutex);
  return *(this->buffer);
}

/**
 * Initialises the Reader with the default properties
 */
void YarpAngleReader::initialise()
{
  YarpAngleReader::initialise(this->getReaderDescription().getReaderProperties());
}

/**
 * Creates a new empty buffer and initialises the port name
 */
void YarpAngleReader::initialise(std::map<std::string,Property*> properties)
{
  this->setPortName(((ComboProperty*)(properties["Port Name"]))->getValue());
  LOG(LOG_DEBUG) << "POOOOOOOOOOOOOORT " << this->getPortName();
  this->buffer = new std::vector<double>();
  this->initialised = false;
}

void YarpAngleReader::workerFunction()
{
  std::map<std::string, YarpPortDetails*>::iterator iter = this->portMap->find(this->getPortName());
  std::string ip;
  std::string port;
  std::string type;
  if (iter != this->portMap->end() )
  {
      ip = iter->second->getIp();
      port = iter->second->getPort();
      type = iter->second->getType();
      LOG(LOG_INFO) << "YarpAngleReader: Yarp Port IP: " << ip << " Port: " << port;
  } else {
    throw ISpikeException("YarpAngleReader: Yarp IP/Port map is empty!");
  }
  this->yarpConnection->connect_to_port(ip, port);
  this->yarpConnection->prepare_to_read_text();
  while(true)
  {
    std::string contentLine = this->yarpConnection->read_until("\n");
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
        double angle = boost::lexical_cast<double>(current_string);
        angles->push_back(angle);
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

