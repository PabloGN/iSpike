/*
 * Log.hpp
 *
 *  Created on: 2 Apr 2011
 *      Author: Edgars Lazdins
 */

#ifndef LOG_HPP_
#define LOG_HPP_

#include <string>
#include <iostream>
#include <sstream>

/**
 * A simple logging class based on this article: http://drdobbs.com/cpp/201804215?pgno=1
 */

/**
 * For a description on how to use the log levels, see http://blog.pantheios.org/2010/10/choosing-severity-levels.html
 */
enum TLogLevel {
  LOG_EMERGENCY, LOG_ALERT, LOG_CRITICAL, LOG_ERROR,
  LOG_WARNING, LOG_NOTICE, LOG_INFO, LOG_DEBUG
};

#define LOG(level) \
if (level > Log::ReportingLevel()) ; \
else Log().Get(level)


class Log
{
  public:
     Log()
     {

     }
     virtual ~Log();
     std::ostringstream& Get(TLogLevel level = LOG_INFO);
  public:
     static TLogLevel& ReportingLevel();
  protected:
     std::ostringstream os;
  private:
     Log(const Log&);
     Log& operator =(const Log&);
     std::string logLevelToString(TLogLevel level);
};


#endif /* LOG_HPP_ */
