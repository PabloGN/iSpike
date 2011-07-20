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
	copyProperties = false;
}


/** Destructor */
JointOutputChannel::~JointOutputChannel(){
	if(isInitialized()){
		delete [] variables;
	}
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

//Inherited from OutputChannel
void JointOutputChannel::setFiring(std::vector<int>& buffer){
	//Work through the neuron ids in the buffer
	for(vector<int>::iterator iter =buffer.begin(); iter != buffer.end(); ++ iter){
		variables[*iter] += currentIncrement;
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
	numVariables = getWidth() * getHeight();
	variables = new double[numVariables];
	for(int i=0; i<numVariables; ++i)
		variables[i] = 0.0;

	setInitialized(true);
}


//Inherited from Channel
void JointOutputChannel::step(){
	isStepping = true;

	//Calculate the angle from the current variables
	double angleSum = 0;
	double weightSum = 0;
	for(unsigned int j = 0; j < variables.size(); j++)     {
        double currentAngle = (this->maxAngle - this->minAngle) / ((this->width * this->height)-1) * j + this->minAngle;
        angleSum += variables[j] * currentAngle;
        weightSum += variables[j];
	}
	if(!weightSum == 0) {
        double angle = angleSum / weightSum;
        LOG(LOG_DEBUG) << "Angle: " << angle;
        this->currentAngle = angle;
        this->writer->addAngle(angle);
	}

	/* Decay the variables according to the following function:
     * N(t+1) = N(t)*e^(-rateOfDecay*t)
     */
	for(unsigned int i=0; i < numVariables; ++i) {
		variables[i] = variables[i] * exp(-(this->rateOfDecay) * times[i]);
		//FIXME MAKE THIS A PROPER EXPONENTIAL FUNCTION
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

/** Updates the properties by first checking if any are read-only  */
void JointOutputChannel::updateProperties(map<string, Property> properties, bool updateReadOnly) {
	if(propertyMap.size() != properties.size())
		throw iSpikeException("JointOutputChannel: Current properties do not match new properties.");

	for(std::map<std::string,Property*>::const_iterator iter = properties.begin(); iter != properties.end(); ++iter){
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
			switch (iter->second->getType()) {
				case Property::Integer: {
					int value = ((IntegerProperty*)(iter->second))->getValue();
					((IntegerProperty)propertyMap[paramName]).setValue(value);

					if (paramName == "Neuron Width")
						setWidth(value);
					else if (paramName == "Neuron Height")
						setHeight(value);
				}
				break;
				case Property::Double:{
					double value = ((DoubleProperty*)(iter->second))->getValue();
					((DoubleProperty)propertyMap[paramName]).setValue(value);

					if (paramName == "Minimum Angle")
						this->minAngle = value;
					else if (paramName == "Maximum Angle")
						this->maxAngle = value;
					else if (paramName == "Rate Of Decay")
						this->rateOfDecay = value;
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


