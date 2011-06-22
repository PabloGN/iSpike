/*
 * YarpAngleWriter.cpp
 *
 *  Created on: 23 Feb 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/Writer/YarpAngleWriter.hpp>
#include <iSpike/YarpPortDetails.hpp>
#include <iSpike/YarpConnection.hpp>
#include <sstream>
#include <iostream>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>

void YarpAngleWriter::initialise()
{
  YarpAngleWriter::initialise(this->getWriterDescription().getWriterProperties());
}

void YarpAngleWriter::initialise(std::map<std::string,Property*> properties)
{
  this->setPortName(((ComboProperty*)(properties["Port Name"]))->getValue());
  this->degreeOfFreedom = ((IntegerProperty*)(properties["Degree Of Freedom"]))->getValue();
  this->angleList = new std::queue<double>();
  this->initialised = false;
}

void YarpAngleWriter::addAngle(double angle)
{
  boost::mutex::scoped_lock lock(this->mutex);
  if(abs(angle - this->previousAngle) < 0.5)
    return;
  if(this->angleList->size() > 0)
  {
    double lastAngle = this->angleList->back();
    if(abs(angle - lastAngle) < 0.5)
    {
      return;
    }
  }
  this->previousAngle = angle;
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
  int sleepAmount = 1;
  std::map<std::string, YarpPortDetails*>::iterator iter = this->portMap->find(this->getPortName());
  std::string ip;
  std::string port;
  std::string type;
  if (iter != this->portMap->end() )
  {
      ip = iter->second->getIp();
      port = iter->second->getPort();
      type = iter->second->getType();
      LOG(LOG_INFO) << "YarpAngleWriter: Yarp Port IP: " << ip << " Port: " << port;
  } else {
    throw ISpikeException("YarpAngleWriter: Yarp IP/Port map is empty!");
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
      commandStream << "set pos " << this->degreeOfFreedom << " " << angle << "\n";
      std::string command = commandStream.str();
      LOG(LOG_DEBUG) << "YarpAngleWriter: Sent command " << command;
      this->yarpConnection->write_text(command);
      LOG(LOG_DEBUG) << "YarpAngleWriter: Received reply " << this->yarpConnection->read_until("\n");
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(sleepAmount));
  }
}
