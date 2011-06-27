/*
 * JointInputChannel.hpp
 *
 *  Created on: 31 Dec 2010
 *      Author: Edgars Lazdins
 */

#ifndef JOINTINPUTCHANNEL_HPP_
#define JOINTINPUTCHANNEL_HPP_

#include <string>
#include <vector>
#include <iSpike/Channel/InputChannel/InputChannel.hpp>
#include <iSpike/Reader/YarpAngleReader.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/Channel/InputChannel/InputChannelDescription.hpp>
#include <map>
#include "iSpike/iSpikeThread.hpp"
using namespace ispike;

/**
 * @class JointInputChannel
 * @brief JointInputChannel class
 *
 * Class that converts a joint angle input into a spike pattern
 *
 * @author Edgars Lazdins
 *
 */
class JointInputChannel : public InputChannel, public iSpikeThread {

	private:
		//==========================  VARIABLES  ==========================
		std::vector< std::vector<int> > buffer;
		AngleReader* reader;
		boost::shared_ptr<boost::thread> threadPointer;
		bool initialised;
		bool sleeping;
		int degreeOfFreedom;
		double currentAngle;
		double sd;
		double minAngle;
		double maxAngle;

		/*! Parameters for Izhikevich neurons */
		double parameterA;
		double parameterB;
		double parameterC;
		double parameterD;

		double currentFactor;
		double constantCurrent;

		//=========================  METHODS  =============================
		void workerFunction();

	protected:
		void updateProperties(std::map<std::string,Property*> properties, bool updateReadOnly);


	public:

		JointInputChannel();
		~JointInputChannel();
		double getCurrentAngle(){ return currentAngle; }
		void updateProperties(std::map<std::string,Property*> properties);

		/**
	   * Initialises the Joint Input Channel with the default parameters
	   * @param reader The associated Angle Reader
	   */
		void initialise(AngleReader* reader){ initialise(reader, channelDescription->getChannelProperties()); }

		/**
	   * Initialises the Joint Input Channel with the parameters provided
	   * @param reader The associated Angle Reader
	   * @param properties The initialisation property map
	   */
		void initialise(AngleReader* reader, std::map<std::string,Property*> properties);

		void start();

		/**
	   * Retrieves the spike pattern
	   * @return The spike pattern
	   */
		std::vector< std::vector<int> > getFiring();

		/**
	   * Steps the conversion loop
	   */
		void step();

		/**
	   * Returns the current initialisation status of this channel
	   * @return Current initialisation status
	   */
		bool isInitialised() { return this->initialised; }

};

#endif /* JOINTINPUTCHANNEL_HPP_ */
