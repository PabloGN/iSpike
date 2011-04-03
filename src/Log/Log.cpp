/*
 * Log.cpp
 *
 *  Created on: 2 Apr 2011
 *      Author: Edgars Lazdins
 */

#include <iSpike/Log/Log.hpp>
#include <boost/date_time.hpp>
#include <stdio.h>

TLogLevel& Log::ReportingLevel()
{
  static TLogLevel reportingLevel(LOG_DEBUG);
  return reportingLevel;
}

std::string Log::logLevelToString(TLogLevel level)
{
  std::string result;
  switch(level)
  {
    case LOG_EMERGENCY:
      result =  "Emergency";
      break;
    case LOG_ALERT:
      result = "Alert";
      break;
    case LOG_CRITICAL:
      result = "Critical";
      break;
    case LOG_ERROR:
      result = "Error";
      break;
    case LOG_WARNING:
      result = "Warning";
      break;
    case LOG_NOTICE:
      result = "Notice";
      break;
    case LOG_INFO:
      result = "Info";
      break;
    case LOG_DEBUG:
      result = "Debug";
      break;
  }
  return result;
}

std::ostringstream& Log::Get(TLogLevel level)
{
   boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
   os << now;
   os << " " << this->logLevelToString(level) << ": ";
   int size = 9 - this->logLevelToString(level).size();
   os << std::string(size, ' ');
   return os;
}

Log::~Log()
{
  os << std::endl;
  fprintf(stderr, "%s", os.str().c_str());
  fflush(stderr);

}
