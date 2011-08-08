//iSpike includes
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>
using namespace ispike;

//Property names
#define MIN_ANGLE_NAME "Minimum Angle"
#define MAX_ANGLE_NAME "Maximum Angle"
#define RATE_OF_DECAY_NAME "Rate of Decay"
#define NEURON_WIDTH_NAME "Neuron Width"
#define NEURON_HEIGHT_NAME "Neuron Height"


/** Constructor */
JointOutputChannel::JointOutputChannel() : OutputChannel() {
	//First define the properties of this channel
	addProperty(Property(Property::Double, -90.0,  MIN_ANGLE_NAME, "The minimum angle to read", true));
	addProperty(Property(Property::Double, 90.0, MAX_ANGLE_NAME, "The maximum angle to read", true));
	addProperty(Property(Property::Double, 0.25, RATE_OF_DECAY_NAME,  "The rate of decay of the angle variables", false));
	addProperty(Property(Property::Integer, 10, NEURON_WIDTH_NAME, "Width of the neuron network", true));
	addProperty(Property(Property::Integer, 1, NEURON_HEIGHT_NAME, "Height of the neuron network", true));

	//Create the description
	channelDescription = Description("Joint Output Channel", "This channel converts a pattern of spikes into an angle and writes it", "Angle Writer");

	//Initialize variables
	writer = NULL;
}


/** Destructor */
JointOutputChannel::~JointOutputChannel(){
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


//Inherited from PropertyHolder
void JointOutputChannel::setProperties(map<string, Property>& properties){
	updateProperties(properties);
}


//Inherited from OutputChannel
void JointOutputChannel::initialize(Writer* writer, map<string, Property>& properties){
	//This class requires an angle writer, so cast and check
	this->writer = dynamic_cast<AngleWriter*>(writer);
	if(this->writer == NULL)
		throw ISpikeException("Cannot initialize JointOutputChannel with a null reader.");

	//Update properties in this and dependent classes
	updateProperties(properties);

	//Start the writer thread running
	this->writer->start();

	//Set up current variables
	for(int i=0; i<size(); ++i)
		currentVariables.push_back(0.0);

	// Calculate angle covered by each current variable */
	double angleDist = (maxAngle - minAngle) / double(size()-1);

	// Populate the current variable angles
	for(unsigned n=0; n < size(); ++n) {
		currentVariableAngles.push_back(minAngle + n * angleDist);
	}

	setInitialized(true);
}


//Inherited from Channel
void JointOutputChannel::step(){
	//Check writer for errors
	if(writer->isError()){
		LOG(LOG_CRITICAL)<<"AngleWriter Error: "<<writer->getErrorMessage();
		throw ISpikeException("Error in AngleWriter");
	}

	// Exponential decay of variables
	for(vector<double>::iterator iter = currentVariables.begin(); iter != currentVariables.end(); ++iter) {
		(*iter) *= rateOfDecay;
	}

	/* Now work out the weighted sum */
	double angleSum = 0.0;
	double weightSum = 0.0;
	for(unsigned n = 0; n < size(); ++n) {
		angleSum += currentVariableAngles.at(n) * currentVariables.at(n);
		weightSum += currentVariables.at(n);
	}

	//Check angle is in range and set it in writer
	double  newAngle = weightSum ? (angleSum / weightSum) : 0.0;
	if(newAngle > maxAngle){
		LOG(LOG_WARNING)<<"JointOutputChannel: New angle ("<<newAngle<<") exceeds the maximum ("<<maxAngle<<". Has been limited to the maximum";
		newAngle = maxAngle;
	}
	else if(newAngle < minAngle){
		LOG(LOG_WARNING)<<"JointOutputChannel: New angle ("<<newAngle<<") is less than the minimum ("<<minAngle<<". Has been limited to the minimum";
		newAngle = minAngle;
	}
	writer->setAngle(newAngle);
}


/*--------------------------------------------------------------------*/
/*---------              PROTECTED METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Updates the properties by first checking if any are read-only  */
void JointOutputChannel::updateProperties(map<string, Property>& properties) {
	if(propertyMap.size() != properties.size())
		throw ISpikeException("JointOutputChannel: Current properties do not match new properties.");

	updatePropertyCount = 0;
	for(map<string,Property>::iterator iter = properties.begin(); iter != properties.end(); ++iter){
		//In updateReadOnly mode, only update properties that are not read only
		if((isInitialized() && !propertyMap[iter->first].isReadOnly()) || !isInitialized()) {
			string paramName = iter->second.getName();
			switch (iter->second.getType()) {
				case Property::Integer: {
					if (paramName == NEURON_WIDTH_NAME)
						setWidth(updateIntegerProperty(iter->second));
					else if (paramName == NEURON_HEIGHT_NAME)
						setHeight(updateIntegerProperty(iter->second));
				}
				break;
				case Property::Double:{
					if (paramName == MIN_ANGLE_NAME)
						minAngle = updateDoubleProperty(iter->second);
					else if (paramName == MAX_ANGLE_NAME)
						maxAngle = updateDoubleProperty(iter->second);
					else if (paramName == RATE_OF_DECAY_NAME)
						rateOfDecay = updateDoubleProperty(iter->second);
				}
				break;
				case Property::Combo: break;
				case Property::String: break;
				default:
					throw ISpikeException("Property type not recognized.");
			}
		}
	}

	//Check angles are sensible
	if(maxAngle <= minAngle) {
		throw ISpikeException("JointOutputChannel: Maximum angle must be greater than minimum angle");
	}

	//Check all properties were updated
	if(!isInitialized() && updatePropertyCount != propertyMap.size())
		throw ISpikeException("JointOutputChannel: Some or all of the properties were not updated: ", updatePropertyCount);
}


