//iSpike includes
#include <iSpike/Channel/InputChannel/JointInputChannel.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Reader/AngleReader.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;

//Local debugging information
//#define DEBUG
//#define RECORD_TIMING

//Names of properties
#define DEGREE_OF_FREEDOM_NAME "Degree Of Freedom"
#define STANDARD_DEVIATION_NAME "Standard Deviation"
#define MIN_ANGLE_NAME "Minimum Angle"
#define MAX_ANGLE_NAME "Maximum Angle"
#define NEURON_WIDTH_NAME "Neuron Width"
#define NEURON_HEIGHT_NAME "Neuron Height"
#define PARAM_A_NAME "Parameter A"
#define PARAM_B_NAME "Parameter B"
#define PARAM_C_NAME "Parameter C"
#define PARAM_D_NAME "Parameter D"
#define PEAK_CURRENT_NAME "Peak Current"
#define CONSTANT_CURRENT_NAME "Constant Current"



/** Default constructor, initialises the default channel properties */
JointInputChannel::JointInputChannel() {
	// First define the properties of this channel
	addProperty(Property(Property::Integer, 0, DEGREE_OF_FREEDOM_NAME, "The degree of freedom to read from this joint", false));
	addProperty(Property(Property::Double, 0.5, STANDARD_DEVIATION_NAME, "The standard deviation as a percentage of neurons covered", true));
	addProperty(Property(Property::Double, -90.0, MIN_ANGLE_NAME, "The minimum angle to read", true));
	addProperty(Property(Property::Double, 90.0, MAX_ANGLE_NAME, "The maximum angle to read", true));
	addProperty(Property(Property::Integer, 10, NEURON_WIDTH_NAME, "Width of the neuron network", true));
	addProperty(Property(Property::Integer, 1, NEURON_HEIGHT_NAME, "Height of the neuron network", true));
	addProperty(Property(Property::Double, 0.1, PARAM_A_NAME, "Parameter A of the Izhikevich Neuron Model", false));
	addProperty(Property(Property::Double, 0.2, PARAM_B_NAME, "Parameter B of the Izhikevich Neuron Model", false));
	addProperty(Property(Property::Double, -65.0, PARAM_C_NAME, "Parameter C of the Izhikevich Neuron Model", false));
	addProperty(Property(Property::Double, 2.0, PARAM_D_NAME, "Parameter D of the Izhikevich Neuron Model", false));
	addProperty(Property(Property::Double, 40.0, PEAK_CURRENT_NAME, "Maximum current that will be injected into neuron", true));
	addProperty(Property(Property::Double, 0.0, CONSTANT_CURRENT_NAME, "This value is added to the incoming current", false));

	//Create the description
	channelDescription = Description("Joint Input Channel", "This is a joint input channel", "Angle Reader");

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

	/* Angle covered by each neuron */
	double angleDist = (maxAngle - minAngle) / double(size()-1);

	//Standard deviation expressed in angle
	double sdAngle = standardDeviation * angleDist;

	//Create normal distribution and calculate current factor
	normalDistribution = boost::math::normal_distribution<double>(0.0, sdAngle);
	currentFactor = peakCurrent / pdf(normalDistribution, 0.0);

	/* populate the angles */
	for(unsigned n=0; n < size(); ++n) {
		neuronAngles.push_back(minAngle + n * angleDist);
	}

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

	//Get angle and check it is in range
	double tmpAngle = reader->getAngle();
	if(tmpAngle > maxAngle){
		LOG(LOG_WARNING)<<"JointInputChannel: New angle ("<<tmpAngle<<") exceeds the maximum ("<<maxAngle<<"). Has been limited to the maximum";
		tmpAngle = maxAngle;
	}
	else if(tmpAngle < minAngle){
		LOG(LOG_WARNING)<<"JointInputChannel: New angle ("<<tmpAngle<<") is less than the minimum ("<<minAngle<<"). Has been limited to the minimum";
		tmpAngle = minAngle;
	}

	//Set input currents to neurons
	for(int index = 0; index < size(); ++index)	{
		neuronSim.setInputCurrent( index, constantCurrent + currentFactor * pdf(normalDistribution, neuronAngles[index]-tmpAngle) );
	}

	//Step the simulator
	neuronSim.step();
}


/*--------------------------------------------------------------------*/
/*---------              PROTECTED METHODS                     -------*/
/*--------------------------------------------------------------------*/

/**  Updates the properties.
	If UpdateReadOnly is set to true only the read only properties will be updated */
void JointInputChannel::updateProperties(map<string, Property>& properties){
	if(propertyMap.size() != properties.size())
		throw ISpikeException("JointInputChannel: Current properties do not match new properties.");

	updatePropertyCount = 0;
	for(map<string, Property>::iterator iter = properties.begin(); iter != properties.end(); ++iter)  {
		//In updateReadOnly mode, only update properties that are not read only
		if( ( isInitialized() && !propertyMap[iter->first].isReadOnly() ) || !isInitialized()) {
			string paramName = iter->second.getName();
			switch (iter->second.getType()) {
				case Property::Integer: {
					if(paramName == DEGREE_OF_FREEDOM_NAME)
						reader->setDegreeOfFreedom(updateIntegerProperty(iter->second));
					else if (paramName == NEURON_WIDTH_NAME)
						setWidth(updateIntegerProperty(iter->second));
					else if (paramName == NEURON_HEIGHT_NAME)
						setHeight(updateIntegerProperty(iter->second));
				}
				break;
				case Property::Double:  {
					if (paramName == PARAM_A_NAME)
						neuronSim.setParameterA(updateDoubleProperty(iter->second));
					else if (paramName == PARAM_B_NAME)
						neuronSim.setParameterB(updateDoubleProperty(iter->second));
					else if (paramName == PARAM_C_NAME)
						neuronSim.setParameterC(updateDoubleProperty(iter->second));
					else if (paramName == PARAM_D_NAME)
						neuronSim.setParameterD(updateDoubleProperty(iter->second));
					else if (paramName == PEAK_CURRENT_NAME)
						peakCurrent = updateDoubleProperty(iter->second);
					else if (paramName == CONSTANT_CURRENT_NAME)
						constantCurrent = updateDoubleProperty(iter->second);
					else if(paramName == STANDARD_DEVIATION_NAME)
						standardDeviation = updateDoubleProperty(iter->second);
					else if (paramName == MIN_ANGLE_NAME)
						minAngle = updateDoubleProperty(iter->second);
					else if (paramName == MAX_ANGLE_NAME)
						maxAngle = updateDoubleProperty(iter->second);
				}
				break;
				case Property::Combo: break;
				case Property::String:	break;
				default:
					throw ISpikeException("Property type not recognized.");
			}
		}
	}

	//Check angles are sensible
	if(maxAngle <= minAngle) {
		throw ISpikeException("Maximum angle must be greater than minimum angle");
	}

	//Check all properties were updated
	if(!isInitialized() && updatePropertyCount != propertyMap.size())
		throw ISpikeException("Some or all of the properties were not updated: ", updatePropertyCount);
}


