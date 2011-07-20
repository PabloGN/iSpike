#ifndef YARPPORTDETAILS_HPP_
#define YARPPORTDETAILS_HPP_

#include <string>
using namespace std;

namespace ispike {

	/** Holds details about a YARP port */
	class YarpPortDetails {
		private:
			string ip;
			unsigned port;
			string type;

		public:
			YarpPortDetails(string ip, unsigned port, string type);
			YarpPortDetails(const YarpPortDetails& ypd);
			YarpPortDetails& operator=(const YarpPortDetails& rhs);
			string getIp(){ return ip; }
			unsigned getPort() { return port; }
			string getType() { return type; }
			void setIp(string ip) { this->ip = ip;}
			void setPort(unsigned port) { this->port = port; }
			void setType(string type) { this->type = type; }

	};

}

#endif /* YARPPORTDETAILS_HPP_ */
