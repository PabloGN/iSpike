/*
 * YarpAngleWriter.cpp
 *
 *  Created on: 23 Feb 2011
 *      Author: cembo
 */

#include <iSpike/Writer/YarpAngleWriter.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include <iSpike/YarpConnection.hpp>
#include <sstream>
#include <iostream>

YarpAngleWriter::YarpAngleWriter(std::string portName)
{
  this->setPortName(portName);
  this->angleList = new std::queue<double>();
  this->initialised = false;
  this->yarpConnection = new YarpConnection("localhost", "10000");
}

void YarpAngleWriter::addAngle(double angle)
{
  boost::mutex::scoped_lock lock(this->mutex);
  this->angleList->push(angle);
}

void YarpAngleWriter::start()
{
  if(!initialised)
  {
    this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpAngleWriter::workerFunction, this))));
    initialised = true;
  }
}

void YarpAngleWriter::workerFunction()
{
  int degreeOfFreedom = 0;
  int sleepAmount = 1;
  std::map<std::string, YarpPortDetails*>* portMap = this->yarpConnection->getPortMap();
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
  this->yarpConnection->connect_to_port(ip, port);
  this->yarpConnection->write_text("CONNECT foo\n");
  while(true)
  {
    if(this->angleList->size() > 0)
    {
      double angle = this->angleList->front();
      {
        boost::mutex::scoped_lock lock(this->mutex);
        this->angleList->pop();
      }
      this->yarpConnection->write_text("d\n");
      std::ostringstream commandStream;
      commandStream << "set pos " << degreeOfFreedom << " " << angle << "\n";
      std::string command = commandStream.str();
      std::cout << command;
      this->yarpConnection->write_text(command);
      std::cout << this->yarpConnection->read_until("\n");
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(sleepAmount));
  }
}
