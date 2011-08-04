#include <iSpike/YarpPortDetails.hpp>
using namespace ispike;

/** Empty constructor */
YarpPortDetails::YarpPortDetails(){
	setIp("Undefined");
	setPort(0);
	setType("Untyped");
}


/** Constructor */
YarpPortDetails::YarpPortDetails(string ip, unsigned port, string type)  {
  setIp(ip);
  setPort(port);
  setType(type);
}


/** Copy constructor */
YarpPortDetails::YarpPortDetails(const YarpPortDetails& ypd){
	ip= ypd.ip;
	port = ypd.port;
	type = ypd.type;
}


/** Assignment operator */
YarpPortDetails& YarpPortDetails::operator=(const YarpPortDetails& rhs){
	if(this == &rhs)
		return *this;

	ip= rhs.ip;
	port = rhs.port;
	type = rhs.type;

	return *this;
}
