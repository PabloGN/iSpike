//iSpike includes
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/Property.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;

//Other includes
//#include <map>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <ios>
//using namespace std;

//Local debugging information
//#define DEBUG
//#define RECORD_TIMING


/** Default constructor, initialises the default channel properties */
JointInputChannel::JointInputChannel() {
	// First define the properties of this channel
	addProperty(IntegerProperty(0, "Degree Of Freedom", "The degree of freedom to read from this joint", true));
	addProperty(DoubleProperty(0.5, "Standard Deviation", "The standard deviation as a percentage of neurons covered", true));
	addProperty(DoubleProperty(-90, "Minimum Angle", "The minimum angle to read", true));
	addProperty(DoubleProperty(90, "Maximum Angle", "The maximum angle to read", true));
	addProperty(IntegerProperty(10, "Neuron Width", "Width of the neuron network", true));
	addProperty(IntegerProperty(1, "Neuron Height", "Height of the neuron network", true));
	addProperty(DoubleProperty(0.1, "Parameter A", "Parameter A of the Izhikevich Neuron Model", false));
	addProperty(DoubleProperty(0.2, "Parameter B", "Parameter B of the Izhikevich Neuron Model", false));
	addProperty(DoubleProperty(-65, "Parameter C", "Parameter C of the Izhikevich Neuron Model", false));
	addProperty(DoubleProperty(2, "Parameter D", "Parameter D of the Izhikevich Neuron Model", false));
	addProperty(DoubleProperty(400, "Current Factor", "Incoming current is multiplied by this value", false));
	addProperty(DoubleProperty(0, "Constant Current", "This value is added to the incoming current", false));

	//Create the description
	channelDescription = InputChannelDescription("Joint Input Channel", "This is a joint input channel", "Angle Reader");

	//Initialize variables
	reader = NULL;
}


/*! Destructor */
JointInputChannel::~JointInputChannel(){
	if(reader != NULL)
		delete reader;
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

//Inherited from InputChannel
void JointInputChannel::initialize(Reader* reader, map<string, Property>& properties){
	//This class requires an angle reader, so cast it and check
	this->reader = dynamic_cast<AngleReader*>(reader);
	if(this->reader == NULL)
		throw ISpikeException("Cannot initialize JointInputChannel with a null reader.");

	//Update and store properties
	updateProperties(properties);

	//Start the reader thread running
	this->reader->start();

	//Set up Izhikevich simulation
	neuronSim.initialize(size());

	setInitialized(true);
}


//Inherited from Channel. This will be done immediately if we are not stepping or deferred until the end of the step */
void JointInputChannel::setProperties(map<string,Property>& properties){
	updateProperties(properties);
}


//Inherited from InputChannel
void JointInputChannel::step(){
	//Check reader for errors
	if(reader->isError()){
		LOG(LOG_CRITICAL)<<"AngleReader Error: "<<reader->getErrorMessage();
		throw ISpikeException("Error in AngleReader");
	}

	///Calculate the standard deviation as a percentage of the image 3 standard deviations in each direction cover almost all of the range
	int totalNeurons = this->width * this->height;
	int angleDist = (this->maxAngle - this->minAngle) / totalNeurons;
	double standardDeviation = ((totalNeurons * this->sd) / 6) * angleDist;

	double tmpAngle = reader->getAngle();
//	///Iterate over the each neuron
//	for(int i = 0; i < this->width * this->height; i++)	{
//		double currentAngle;
//		if (i == 0)
//			currentAngle = this->minAngle;
//		///Generate the current angle by interpolating the angle range over the neuron count
//		else
//			currentAngle = (this->maxAngle - this->minAngle) / (this->width * this->height-1) * i + this->minAngle;

//		///Put a normal distribution at the centre of the observed angle with sd as defined earlier
//		double main = 1 / sqrt(2 * boost::math::constants::pi<double>() * pow(standardDeviation,2));
//		double exponent = pow((currentAngle - angles[this->degreeOfFreedom]),2) / (2 * pow(standardDeviation,2));

//		///Update the current map with the value for this angle
//		neuronSim->setInputCurrent(currentFactor * main * exp(-exponent));
//	}
}


/*--------------------------------------------------------------------*/
/*---------              PROTECTED METHODS                     -------*/
/*--------------------------------------------------------------------*/

/**  Updates the properties.
	If UpdateReadOnly is set to true only the read only properties will be updated */
void JointInputChannel::updateProperties(map<string, Property>& properties){
	if(propertyMap.size() != properties.size())
		throw ISpikeException("JointInputChannel: Current properties do not match new properties.");

	bool updateReadOnly = !isInitialized();
	for(map<string, Property>::iterator iter = properties.begin(); iter != properties.end(); ++iter)  {
		//In updateReadOnly mode, only update properties that are not read only
		if((updateReadOnly && !propertyMap[iter->first].isReadOnly()) || !updateReadOnly) {
			string paramName = iter->second.getName();
			switch (iter->second.getType()) {
				case Property::Integer: {
					if(paramName == "Degree Of Freedom")
						reader->setDegreeOfFreedom(updatePropertyValue(dynamic_cast<IntegerProperty&>(iter->second)));
					else if (paramName == "Neuron Width")
						setWidth(updatePropertyValue(dynamic_cast<IntegerProperty&>(iter->second)));
					else if (paramName == "Neuron Height")
						setHeight(updatePropertyValue(dynamic_cast<IntegerProperty&>(iter->second)));
				}
				break;
				case Property::Double:  {
					if (paramName == "Parameter A")
						neuronSim.setParameterA(updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second)));
					else if (paramName == "Parameter B")
						neuronSim.setParameterB(updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second)));
					else if (paramName == "Parameter C")
						neuronSim.setParameterC(updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second)));
					else if (paramName == "Parameter D")
						neuronSim.setParameterD(updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second)));
					else if (paramName == "Current Factor")
						currentFactor = updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second));
					else if (paramName == "Constant Current")
						constantCurrent = updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second));
					else if(paramName == "Standard Deviation")
						sd = updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second));
					else if (paramName == "Minimum Angle")
						minAngle = updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second));
					else if (paramName == "Maximum Angle")
						maxAngle = updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second));
				}
				break;
				case Property::Combo: break;
				case Property::String:	break;
				default:
					throw ISpikeException("Property type not recognized.");
			}
		}
	}
}


