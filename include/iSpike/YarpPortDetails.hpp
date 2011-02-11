/*
 * YarpPortDetails.hpp
 *
 *  Created on: 31 Dec 2010
 *      Author: cembo
 */

#ifndef YARPPORTDETAILS_HPP_
#define YARPPORTDETAILS_HPP_

#include <string>

class YarpPortDetails
{
  private:
    std::string ip;
    std::string port;
    std::string type;

  public:
    std::string getIp()
    {
        return ip;
    }

    std::string getPort()
    {
        return port;
    }

    std::string getType()
    {
        return type;
    }

    void setIp(std::string ip)
    {
        this->ip = ip;
    }

    void setPort(std::string port)
    {
        this->port = port;
    }

    void setType(std::string type)
    {
        this->type = type;
    }

    YarpPortDetails(std::string ip, std::string port, std::string type)
    {
      setIp(ip);
      setPort(port);
      setType(type);
    }

};

#endif /* YARPPORTDETAILS_HPP_ */
