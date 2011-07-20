#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <ios>
#include <boost/lexical_cast.hpp>
#include <boost/math/constants/constants.hpp>
#include <iSpike/Property.hpp>
#include <map>
#include <iSpike/Log/Log.hpp>
#include <boost/timer.hpp>
#include <iSpike/ISpikeException.hpp>

//Local debugging information
//#define DEBUG
//#define RECORD_TIMING


/** Default constructor, initialises the default channel properties */
JointInputChannel::JointInputChannel() {
	// First define the properties of this channel
	propertyMap["Degree Of Freedom"] = IntegerProperty(0, "Degree Of Freedom", "The degree of freedom to read from this joint", true);
	propertyMap["Standard Deviation"] = DoubleProperty(0.5, "Standard Deviation", "The standard deviation as a percentage of neurons covered", true);
	propertyMap["Minimum Angle"] = DoubleProperty(-90, "Minimum Angle", "The minimum angle to read", true);
	propertyMap["Maximum Angle"] = DoubleProperty(90, "Maximum Angle", "The maximum angle to read", true);
	propertyMap["Neuron Width"] = IntegerProperty(10, "Neuron Width", "Width of the neuron network", true);
	propertyMap["Neuron Height"] = IntegerProperty(1, "Neuron Height", "Height of the neuron network", true);
	propertyMap["Parameter A"] = DoubleProperty(0.1, "Parameter A", "Parameter A of the Izhikevich Neuron Model", false);
	propertyMap["Parameter B"] = DoubleProperty(0.2, "Parameter B", "Parameter B of the Izhikevich Neuron Model", false);
	propertyMap["Parameter C"] = DoubleProperty(-65, "Parameter C", "Parameter C of the Izhikevich Neuron Model", false);
	propertyMap["Parameter D"] = DoubleProperty(2, "Parameter D", "Parameter D of the Izhikevich Neuron Model", false);
	propertyMap["Current Factor"] = DoubleProperty(400, "Current Factor", "Incoming current is multiplied by this value", false);
	propertyMap["Constant Current"] = DoubleProperty(0, "Constant Current", "This value is added to the incoming current", false);

	// Now let's create the description
	channelDescription = InputChannelDescription("Joint Input Channel", "This is a joint input channel", "Angle Reader");

	//Initialize variables
	reader = NULL;
	copyProperties = false;
	izhikevichNeuronSim = NULL;
}


/*! Destructor */
JointInputChannel::~JointInputChannel(){
	if(izhikevichNeuronSim != NULL)
		delete izhikevichNeuronSim;
	if(reader != NULL)
		delete reader;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

//Inherited from InputChannel
void JointInputChannel::initialize(AngleReader* reader, map<string, Property> properties){
	if(reader == NULL)
		throw iSpikeException("Cannot initialize JointInputChannel with a null reader.");
	this->reader = reader;
	this->reader->start();

	updateProperties(properties, false);

	//Set up Izhikevich simulation
	neuronSim = new IzhikevichNeuronSim(this->width * this->height, this->parameterA, this->parameterB, this->parameterC, this->parameterD, this->currentFactor, this->constantCurrent);

	setInitialized(true);
}


//Inherited from Channel. This will be done immediately if we are not stepping or deferred until the end of the step */
void JointInputChannel::setProperties(map<string,Property>& properties){
	if(isStepping){
		newPropertyMap = properties;
		copyProperties = true;
	}
	else{
		if(isInitialized())
			updateProperties(properties, true);
		else
			updateProperties(properties, false);
	}
}


//Inherited from InputChannel
void JointInputChannel::step(){
	isStepping = true;

	//Check reader for errors
	if(reader->isError()){
		LOG(LOG_CRITICAL)<<"AngleReader Error: "<<reader->getErrorMessage();
		throw iSpikeException("Error in AngleReader");
	}

	#ifdef RECORD_TIMING
		std::ofstream timeStream;
		timeStream.open("timings.txt", std::fstream::out | std::fstream::app);
	#endif//RECORD_TIMING
	boost::timer t;

	///calculate the standard deviation as a percentage of the image 3 standard deviations in each direction cover almost all of the range
	int totalNeurons = this->width * this->height;
	int angleDist = (this->maxAngle - this->minAngle) / totalNeurons;
	double standardDeviation = ((totalNeurons * this->sd) / 6) * angleDist;

	std::vector<double> angles = this->reader->getData();
	std::vector<double> currents(this->width * this->height);
	if(angles.size() > 0){
		double initial = t.elapsed();
		LOG(LOG_INFO) << "Actual Read Angle: " << angles[this->degreeOfFreedom];
		this->currentAngle = angles[this->degreeOfFreedom];
		///Iterate over the each neuron
		for(int i = 0; i < this->width * this->height; i++)	{
			double currentAngle;
			if (i == 0)
				currentAngle = this->minAngle;
			///Generate the current angle by interpolating the angle range over the neuron count
			else
				currentAngle = (this->maxAngle - this->minAngle) / (this->width * this->height-1) * i + this->minAngle;

			///Put a normal distribution at the centre of the observed angle with sd as defined earlier
			double main = 1 / sqrt(2 * boost::math::constants::pi<double>() * pow(standardDeviation,2));
			double exponent = pow((currentAngle - angles[this->degreeOfFreedom]),2) / (2 * pow(standardDeviation,2));

			///Update the current map with the value for this angle
			currents[i] = main * exp(-exponent);

			//Output debugging information if required
			#ifdef DEBUG
				std::ostringstream currentText;
				currentText << "Angle: " << currentAngle << " Currents: ";
				for(unsigned int n = 0; n < currents.size(); n++) {
					currentText << currents[n] << ", ";
				}
				LOG(LOG_DEBUG) << currentText.str();
				LOG(LOG_DEBUG) << "Angle: " << currentAngle << " Current: " << main * exp(-exponent);
				std::ofstream currentStream;
				currentStream.open("currents.txt", std::fstream::out | std::fstream::app);
				for(unsigned int n = 0; n < currents.size(); n++){
					currentStream << currents[n] << ", ";
				}
				currentStream << std::endl;
				currentStream.close();
			#endif//DEBUG
		}
		double prespikes = t.elapsed() - initial;
		std::vector<int>* spikes = neuronSim->getSpikes(&currents);
		double postspikes = t.elapsed() - prespikes;
		buffer.push_back(*spikes);
		delete spikes;

		#ifdef RECORD_TIMING
			timeStream << this->width << "," << prespikes << "," << postspikes << std::endl;
		#endif//RECORD_TIMING
	}

	//Update properties if this has been requested
	if(copyProperties){
		updateProperties(newPropertyMap, true);
		copyProperties = false;
	}

	isStepping = false;
}


/*--------------------------------------------------------------------*/
/*---------              PROTECTED METHODS                     -------*/
/*--------------------------------------------------------------------*/

/**  Updates the properties.
	If UpdateReadOnly is set to true only the read only properties will be updated */
void JointInputChannel::updateProperties(map<string, Property>& properties, bool updateReadOnly){
	if(propertyMap.size() != properties.size())
		throw iSpikeException("JointInputChannel: Current properties do not match new properties.");

	for(map<string, Property>::const_iterator iter = properties.begin(); iter != properties.end(); ++iter)  {
		//Check property exists
		if(propertyMap.count(iter->first) == 0){
			LOG(LOG_CRITICAL) << "JointIntputChannel: Property does not exist: " << iter->first<<endl;
			throw ISpikeException("JointInputChannel: Property not recognized");
		}

		//Check that property name matches the map name
		std::string paramName = iter->second.getName();
		if(paramName != iter->first){
			LOG(LOG_CRITICAL) << "JointIntputChannel: Property name mismatch: " << iter->first<<", "<<paramName<<endl;
			throw ISpikeException("JointInputChannel: Property name mismatch");
		}

		//In updateReadOnly mode, only update properties that are not read only
		if((updateReadOnly && !propertyMap[iter->first].isReadOnly()) || !updateReadOnly) {
			switch (iter->second.getType()) {
				case Property::Integer: {
					//Update the property in the map
					int value = ((IntegerProperty)(iter->second)).getValue();
					((IntegerProperty)propertyMap[paramName]).setValue(value);

					//Update the corresponding variable
					if(paramName == "Degree Of Freedom")
						this->degreeOfFreedom = value;
					else if (paramName == "Neuron Width")
						this->width = value;
					else if (paramName == "Neuron Height")
						this->height = value;
				}
				break;
				case Property::Double:  {
					//Update the property in the map
					double value = ((DoubleProperty)(iter->second)).getValue();
					((DoubleProperty)propertyMap[paramName]).setValue(value);

					//Update the corresponding variable
					if(paramName == "Standard Deviation")
						this->sd = value;
					else if (paramName == "Minimum Angle")
						this->minAngle = value;
					else if (paramName == "Maximum Angle")
						this->maxAngle = value;
					else if (paramName == "Parameter A")
						this->parameterA = value;
					else if (paramName == "Parameter B")
						this->parameterB = value;
					else if (paramName == "Parameter C")
						this->parameterC = value;
					else if (paramName == "Parameter D")
						this->parameterD = value;
					else if (paramName == "Current Factor")
						this->currentFactor = value;
					else if (paramName == "Constant Current")
						this->constantCurrent = value;
				}
				break;
				case Property::Combo: break;
				case Property::String:	break;
				default:
					throw iSpikeException("Property type not recognized.");
			}
		}
	}
}


