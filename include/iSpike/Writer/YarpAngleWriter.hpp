/*
 * YarpAngleWriter.hpp
 *
 *  Created on: 23 Feb 2011
 *      Author: Edgars Lazdins
 */

#ifndef YARPANGLEWRITER_HPP_
#define YARPANGLEWRITER_HPP_

#include <queue>
#include <string>
#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <iSpike/Writer/AngleWriter.hpp>
#include <iSpike/YarpConnection.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/Writer/WriterDescription.hpp>
#include <map>
#include <iSpike/YarpPortDetails.hpp>

class YarpAngleWriter : public AngleWriter {

	private:
		std::queue<double>* angleList;
		boost::shared_ptr<boost::thread> threadPointer;
		void workerFunction();
		boost::mutex mutex;
		bool initialised;
		std::string portName;
		int degreeOfFreedom;
		YarpConnection* yarpConnection;
		std::map<std::string, YarpPortDetails*>* portMap;
		double previousAngle;


	public:
		YarpAngleWriter(std::string nameserverIP, std::string nameserverPort);
		virtual ~YarpAngleWriter();
		void addAngle(double angle);
		bool getInitialised() { return initialised; }
		std::string getPortName() { return this->portName; }
		boost::shared_ptr<boost::thread> getThreadPointer() const {return threadPointer; }
		void initialise();
		void initialise(std::map<std::string,Property*> properties);
		void setPortName(std::string portName) { this->portName = portName; }
		void setInitialised(bool initialised) { this->initialised = initialised; }
		void setThreadPointer(boost::shared_ptr<boost::thread> threadPointer) { this->threadPointer = threadPointer; }
		void start();

};

#endif /* YARPANGLEWRITER_HPP_ */
