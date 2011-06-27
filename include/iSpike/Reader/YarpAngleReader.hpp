/*
 * YarpAngleReader.hpp
 *
 *  Created on: 22 Feb 2011
 *      Author: Edgars Lazdins
 */

#ifndef YARPANGLEREADER_HPP_
#define YARPANGLEREADER_HPP_

#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Bitmap.hpp>
#include <iSpike/Property.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <map>
#include <iSpike/YarpPortDetails.hpp>
#include "iSpike/iSpikeThread.hpp"
using namespace ispike;

/**
 * @class YarpVisualReader
 * @brief YarpVisualReader class
 *
 * This class represents a YarpAngleReader, it retrieves a vector of joint angles from a given yarp port
 * and makes it available upon request
 *
 * @author Edgars Lazdins
 *
 */
class YarpAngleReader : public AngleReader, public iSpikeThread {

	private:
		std::vector<double> buffer;
		boost::shared_ptr<boost::thread> threadPointer;
		void workerFunction();
		boost::mutex mutex;
		bool initialised;
		std::string portName;
		YarpConnection* yarpConnection;
		std::map<std::string, YarpPortDetails*>* portMap;

	public:

		YarpAngleReader(std::string nameserverIP, std::string nameserverPort);
		virtual ~YarpAngleReader();

		std::vector<double> getData();
		void initialise();
		void initialise(std::map<std::string,Property*> properties);
		std::string getPortName(){ return this->portName; }
		void setPortName(std::string portName){ this->portName = portName;}
		bool getInitialised() const	{ return initialised; }
		void setInitialised(bool initialised){ this->initialised = initialised;	}
		boost::shared_ptr<boost::thread> getThreadPointer() const {	return threadPointer; }
		void start();
		void setThreadPointer(boost::shared_ptr<boost::thread> threadPointer){ this->threadPointer = threadPointer;	}

};

#endif /* YARPANGLEREADER_HPP_ */
