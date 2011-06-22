/*
 * ConsoleAngleReader.cpp
 *
 *  Created on: 9 Mar 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/Reader/ConsoleAngleReader.hpp>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>

std::vector<double> ConsoleAngleReader::getData()
{
  std::cout << "Please provide a Joint Angle:" << std::endl;
  double angle;
  std::cin >> angle;
  std::vector<double>* result = new std::vector<double>(1);
  result->at(0) = angle;
  return *(result);
}

void ConsoleAngleReader::start()
{
  if(!initialised)
  {
    initialised = true;
    LOG(LOG_INFO) << "ConsoleAngleReader: Started" << std::endl;
  }
}

void ConsoleAngleReader::initialise(std::map<std::string,Property*> properties)
{
}
