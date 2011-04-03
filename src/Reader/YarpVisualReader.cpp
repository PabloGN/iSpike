#include <iSpike/Reader/YarpVisualReader.hpp>
#include <iSpike/Bitmap.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>

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
  std::map<std::string, YarpPortDetails*>::iterator iter = this->portMap->find(this->getPortName());
  std::string ip;
  std::string port;
  std::string type;
  if (iter != this->portMap->end() )
  {
      ip = iter->second->getIp();
      port = iter->second->getPort();
      type = iter->second->getType();
      LOG(LOG_INFO) << "YarpVisualReader: Yarp Port IP: " << ip << " Port: " << port;
  } else {
    throw ISpikeException("YarpVisualReader: Yarp IP/Port map is empty!");
  }
  this->yarpConnection->connect_to_port(ip, port);
  this->yarpConnection->prepare_to_read_binary();
  while(true)
  {
    Bitmap* image = this->yarpConnection->read_image();
    if(image != NULL)
    {
      boost::mutex::scoped_lock lock(this->mutex);
      if(this->buffer != NULL)
        free(this->buffer->getContents());
      this->buffer = image;
    }
  }
}

void YarpVisualReader::initialise(std::map<std::string,Property*> properties)
{
  this->setPortName(((ComboProperty*)(properties["Port Name"]))->getValue());
  this->buffer = new Bitmap(0,0,0,NULL);
  this->initialised = false;
}
