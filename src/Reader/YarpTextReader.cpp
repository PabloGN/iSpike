#include "SpikeAdapter/Reader/YarpTextReader.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

std::string YarpTextReader::getData()
{
  boost::mutex::scoped_lock lock(this->mutex);
  return this->buffer;
}

void YarpTextReader::workerFunction()
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
    boost::mutex::scoped_lock lock(this->mutex);
    this->buffer = YarpInterface::Instance()->read_until("\n");
  }
}

YarpTextReader::YarpTextReader(std::string portName)
{
  this->setPortName(portName);
  this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpTextReader::workerFunction, this))));
}
