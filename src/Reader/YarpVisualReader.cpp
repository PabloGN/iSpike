#include <SpikeAdapter/Reader/YarpVisualReader.hpp>
#include <SpikeAdapter/Bitmap.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <SpikeAdapter/YarpInterface.hpp>

Bitmap YarpVisualReader::getData()
{
  boost::mutex::scoped_lock lock(this->mutex);
  return *(this->buffer);
}

void YarpVisualReader::start()
{
  if(!initialised)
  {
    this->setThreadPointer(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&YarpVisualReader::workerFunction, this))));
    initialised = true;
  }
}

void YarpVisualReader::workerFunction()
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
  YarpInterface::Instance()->prepare_to_read_binary();
  bool toggler = false;
  while(true)
  {
    Bitmap* image = YarpInterface::Instance()->read_image();
    if(image != NULL)
    {
      boost::mutex::scoped_lock lock(this->mutex);
      if(this->buffer != NULL)
        free(this->buffer->getContents());
      this->buffer = image;
    }
  }
}

YarpVisualReader::YarpVisualReader(std::string portName)
{
  this->setPortName(portName);
  this->buffer = new Bitmap(0,0,0,NULL);
  this->initialised = false;
}
