/*
 * TextReader.hpp
 *
 *  Created on: 1 Jan 2011
 *      Author: cembo
 */

#ifndef YARPTEXTREADER_HPP_
#define YARPTEXTREADER_HPP_

#include <iSpike/Reader/Reader.hpp>
#include <iSpike/YarpConnection.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

class YarpTextReader : public Reader {

private:
  std::string buffer;
  boost::shared_ptr<boost::thread> threadPointer;
  void workerFunction();
  boost::mutex mutex;
  std::string portName;
  YarpConnection* yarpConnection;

public:
  std::string getData();
  YarpTextReader(std::string portName);

  std::string getPortName()
  {
    return this->portName;
  }

  void setPortName(std::string portName)
  {
    this->portName = portName;
  }

  boost::shared_ptr<boost::thread> getThreadPointer() const
  {
      return threadPointer;
  }

  void setThreadPointer(boost::shared_ptr<boost::thread> threadPointer)
  {
      this->threadPointer = threadPointer;
  }

};

#endif /* YARPTEXTREADER_HPP_ */
