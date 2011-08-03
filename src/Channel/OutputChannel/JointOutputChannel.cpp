//iSpike includes
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>
using namespace ispike;

//Other includes
#include <iostream>
using namespace std;

/** Constructor */
JointOutputChannel::JointOutputChannel() : OutputChannel() {
	//First define the properties of this channel
	properties["Minimum Angle"] = new DoubleProperty(-90, "Minimum Angle", "The minimum angle to read", true);
	properties["Maximum Angle"] = new DoubleProperty(90, "Maximum Angle", "The maximum angle to read", true);
	properties["Rate Of Decay"] = new DoubleProperty(0.005,"Rate Of Decay",  "The rate of decay of the angle variables", false);
	properties["Neuron Width"] = new IntegerProperty(10, "Neuron Width", "Width of the neuron network", true);
	properties["Neuron Height"] = new IntegerProperty(1, "Neuron Height", "Height of the neuron network", true);

	//Create the description
	channelDescription = OutputChannelDescription("Joint Output Channel", "This channel converts a pattern of spikes into an angle and writes it", "Angle Writer");

	//Initialize variables
	writer = NULL;
}


/** Destructor */
JointOutputChannel::~JointOutputChannel(){
	if(isInitialized()){
		delete [] currentVariables;
	}
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

//Inherited from OutputChannel
void JointOutputChannel::setFiring(std::vector<int>& buffer){
	//Work through the neuron ids in the buffer
	for(vector<int>::iterator iter =buffer.begin(); iter != buffer.end(); ++ iter){
		currentVariables[*iter] += currentIncrement;
	}
}


//Inherited from OutputChannel
void JointOutputChannel::initialise(AngleWriter* writer, map<string, Property> properties){
	if(reader == NULL)
		throw iSpikeException("Cannot initialize JointOutputChannel with a null reader.");
	this->writer = writer;
	this->writer->start();

	updateProperties(properties, false);

	//Set up current variables
	currentVariables = new double[size()];
	for(int i=0; i<size(); ++i)
		currentVariables[i] = 0.0;

	setInitialized(true);
}


//Inherited from Channel
void JointOutputChannel::step(){
	//Calculate the angle from the current variables
	double angleSum = 0;
	double weightSum = 0;
	for(unsigned int j = 0; j < size(); ++j)     {
		double currentAngle = (maxAngle - minAngle) / ((width * height)-1) * j + minAngle;
		angleSum += currentVariables[j] * currentAngle;
		weightSum += currentVariables[j];
	}
	if(weightSum != 0) {
		writer->setAngle(angleSum / weightSum);
	}

	/* Decay the variables according to the following function:
     * N(t+1) = N(t)*e^(-rateOfDecay*t)
     */
	for(unsigned int i=0; i < size(); ++i) {
		currentVariables[i] = currentVariables[i] * exp(-(rateOfDecay) * times[i]);
		//FIXME MAKE THIS A PROPER EXPONENTIAL FUNCTION
	}
}


/*--------------------------------------------------------------------*/
/*---------              PROTECTED METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Updates the properties by first checking if any are read-only  */
void JointOutputChannel::updateProperties(map<string, Property> properties, bool updateReadOnly) {
	if(propertyMap.size() != properties.size())
		throw iSpikeException("JointOutputChannel: Current properties do not match new properties.");

	for(std::map<std::string,Property*>::const_iterator iter = properties.begin(); iter != properties.end(); ++iter){
		//In updateReadOnly mode, only update properties that are not read only
		if((updateReadOnly && !propertyMap[iter->first].isReadOnly()) || !updateReadOnly) {
			switch (iter->second->getType()) {
				case Property::Integer: {
					if (paramName == "Neuron Width")
						setWidth(updatePropertyValue(iter->second));
					else if (paramName == "Neuron Height")
						setHeight(updatePropertyValue(iter->second));
				}
				break;
				case Property::Double:{
					if (paramName == "Minimum Angle")
						minAngle = updatePropertyValue(iter->second);
					else if (paramName == "Maximum Angle")
						maxAngle = updatePropertyValue(iter->second);
					else if (paramName == "Rate Of Decay")
						rateOfDecay = updatePropertyValue(iter->second);
				}
				break;
				case Property::Combo: break;
				case Property::String: break;
				default:
					throw iSpikeException("Property type not recognized.");
			}
		}
	}
}


