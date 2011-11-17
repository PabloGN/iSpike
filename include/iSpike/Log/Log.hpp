#ifndef LOG_HPP_
#define LOG_HPP_

//Other includes
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

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

namespace ispike {

	/** A simple logging class based on this article: http://drdobbs.com/cpp/201804215?pgno=1  */
	class Log {
		public:
			Log();
			virtual ~Log();
			ostringstream& Get(TLogLevel level = LOG_ERROR);
			static TLogLevel& ReportingLevel();
			static int currentId;

		protected:
			ostringstream os;


		private:
			Log(const Log&);
			Log& operator =(const Log&);
			string logLevelToString(TLogLevel level);
	};

}


#endif /* LOG_HPP_ */
