#include <iSpike/YarpPortDetails.hpp>
using namespace ispike;

/** Constructor */
YarpPortDetails(string ip, int port, string type)  {
  setIp(ip);
  setPort(port);
  setType(type);
}


/** Copy constructor */
YarpPortDetails(const YarpPortDetails& ypd){
	ip= ypd.ip;
	port = ypd.port;
	type = ypd.type;
}


/** Assignment operator */
YarpPortDetails& operator=(const YarpPortDetails& rhs){
	if(this == &rhs)
		return *this;

	ip= rhs.ip;
	port = rhs.port;
	type = rhs.type;

	return *this;
}
