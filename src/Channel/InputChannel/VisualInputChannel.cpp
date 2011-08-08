//iSpike includes
#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;


//Names of properties
#define NEURON_WIDTH_NAME "Neuron Width"
#define NEURON_HEIGHT_NAME "Neuron Height"
#define FOVEA_RADIUS_NAME "Fovea Radius"
#define POSITIVE_SIGMA_NAME "Positive Sigma"
#define NEGATIVE_SIGMA_NAME "Negative Sigma"
#define POSITIVE_FACTOR_NAME "Positive Factor"
#define NEGATIVE_FACTOR_NAME "Negative Factor"
#define OPPONENCY_MAP_NAME "Opponency Map"
#define PARAM_A_NAME "Parameter A"
#define PARAM_B_NAME "Parameter B"
#define PARAM_C_NAME "Parameter C"
#define PARAM_D_NAME "Parameter D"
#define CURRENT_FACTOR_NAME "Current Factor"
#define CONSTANT_CURRENT_NAME "Constant Current"


/** Constructor */
VisualInputChannel::VisualInputChannel() {
	// Properties of log polar foveation
	addProperty(Property(Property::Integer, 50, NEURON_WIDTH_NAME, "Width of the neuron network", true));
	addProperty(Property(Property::Integer, 50, NEURON_HEIGHT_NAME, "Height of the neuron network", true));
	addProperty(Property(Property::Double, 20.0, FOVEA_RADIUS_NAME, "Radius of the central foveated area", true));

	//Properties of the difference of gaussians filter
	addProperty(Property(Property::Double, 2.0, POSITIVE_SIGMA_NAME, "Positive Gaussian Sigma", false));
	addProperty(Property(Property::Double, 4.0, NEGATIVE_SIGMA_NAME, "Negative Gaussian Sigma", false));
	addProperty(Property(Property::Double, 4.0, POSITIVE_FACTOR_NAME, "Multiplication ratio for positive image during subtraction", false));
	addProperty(Property(Property::Double, 2.0, NEGATIVE_FACTOR_NAME, "Multiplication ratio for negative image during subtraction", false));
	addProperty(Property(Property::Integer, 0, OPPONENCY_MAP_NAME, "Which colour oponency map to use (0 = R+G-; 1 = G+R-; 2 = B+Y-)", true));

	//Properties of the neural simulator
	addProperty(Property(Property::Double, 0.1, PARAM_A_NAME, "Parameter A of the Izhikevich Neuron Model", false));
	addProperty(Property(Property::Double, 0.2, PARAM_B_NAME,"Parameter B of the Izhikevich Neuron Model",false));
	addProperty(Property(Property::Double, -65, PARAM_C_NAME,"Parameter C of the Izhikevich Neuron Model",false));
	addProperty(Property(Property::Double, 2.0, PARAM_D_NAME, "Parameter D of the Izhikevich Neuron Model",false));
	addProperty(Property(Property::Double, 20.0, CURRENT_FACTOR_NAME, "Incoming current is multiplied by this value",false));
	addProperty(Property(Property::Double, 0.0, CONSTANT_CURRENT_NAME, "This value is added to the incoming current", false));

	//Create the description
	channelDescription = Description("Visual Input Channel", "This is a visual input channel", "Visual Reader");

	//Set up visual processing classes and neural simulator
	dataReducer = new LogPolarVisualDataReducer();
	dogFilter = new DOGVisualFilter(dataReducer);

	//Initialize variables
	reader = NULL;
	currentImageID = 0;
}


/** Destructor */
VisualInputChannel::~VisualInputChannel() {
	if(isInitialized()) {
		delete reader;
		delete dogFilter;
		delete dataReducer;
	}
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Initialises the channel with the default parameters */
void VisualInputChannel::initialize(Reader* reader, map<string,Property>& properties) {
	//This class requires a visual reader, so reinterpret it and check
	this->reader = dynamic_cast<VisualReader*>(reader);
	if(this->reader == NULL)
		throw ISpikeException("Cannot initialize VisualInputChannel with a null reader.");

	//Store properties in this and dependent classes
	updateProperties(properties);

	//Start the reader thread running
	this->reader->start();

	//Initialize neural simulator
	neuronSim.initialize(size());

	setInitialized(true);
}


/** Sets the properties. This will be done immediately if we are not stepping or deferred until the end of the step */
void VisualInputChannel::setProperties(map<string,Property>& properties){
	updateProperties(properties);
}


//Inherited from Channel
void VisualInputChannel::step() {
	//Check reader for errors
	if(reader->isError()){
		LOG(LOG_CRITICAL)<<"AngleReader Error: "<<reader->getErrorMessage();
		throw ISpikeException("Error in AngleReader");
	}

	//Update visual maps if necessary
	if(reader->getImageID() != currentImageID){
		currentImageID = reader->getImageID();
		dataReducer->setBitmap(reader->getBitmap());
		dogFilter->update();
	}

	//Load opponency data into neural simulator
	Bitmap& opponencyMap = dogFilter->getOpponencyBitmap();
	if(!opponencyMap.isEmpty()) {
		//Get reference to opponency map
		int opponencyMapSize = opponencyMap.size();
		unsigned char* opponencyMapContents = opponencyMap.getContents();

		if(opponencyMapSize != size())
			throw ISpikeException("VisualInputChannel: Incoming map size does not match size of visual channel");

		//Convert pixels to currents in simulator
		for(int i=0; i<opponencyMapSize; ++i){
			//Set the input current to the neurons
			neuronSim.setInputCurrent(i, currentFactor * opponencyMapContents[i] + constantCurrent);
		}
	}

	//Step the simulator
	neuronSim.step();
}


/*--------------------------------------------------------------------*/
/*---------              PROTECTED METHODS                     -------*/
/*--------------------------------------------------------------------*/

/**  Updates the properties by first checking if any are read-only */
void VisualInputChannel::updateProperties(map<string, Property>& properties) {
	if(propertyMap.size() != properties.size())
		throw ISpikeException("VisualInputChannel: Current properties do not match new properties.");

	//Update properties in the property map and the appropriate class
	updatePropertyCount = 0;
	for(map<string,Property>::iterator iter = properties.begin(); iter != properties.end(); ++iter) {
		//When initialized, only update properties that are not read only
		if( (isInitialized() && !iter->second.isReadOnly()) || !isInitialized()) {
			string paramName = iter->second.getName();
			switch (iter->second.getType()){
				case Property::Integer: {
					if(paramName == OPPONENCY_MAP_NAME)
						dogFilter->setOpponencyTypeID(updateIntegerProperty(iter->second));
					else if (paramName == NEURON_WIDTH_NAME){
						setWidth(updateIntegerProperty(iter->second));
						dataReducer->setOutputWidth(getWidth());
					}
					else if (paramName == NEURON_HEIGHT_NAME){
						setHeight(updateIntegerProperty(iter->second));
						dataReducer->setOutputHeight(getHeight());
					}
					break;
				}
				case Property::Double: {
					if (paramName == PARAM_A_NAME)
						neuronSim.setParameterA(updateDoubleProperty(iter->second));
					else if (paramName == PARAM_B_NAME)
						neuronSim.setParameterB(updateDoubleProperty(iter->second));
					else if (paramName == PARAM_C_NAME)
						neuronSim.setParameterC(updateDoubleProperty(iter->second));
					else if (paramName == PARAM_D_NAME)
						neuronSim.setParameterD(updateDoubleProperty(iter->second));
					else if (paramName == CURRENT_FACTOR_NAME)
						currentFactor = updateDoubleProperty(iter->second);
					else if (paramName == CONSTANT_CURRENT_NAME)
						constantCurrent = updateDoubleProperty(iter->second);
					else if (paramName == POSITIVE_SIGMA_NAME)
						dogFilter->setPositiveSigma(updateDoubleProperty(iter->second));
					else if (paramName == NEGATIVE_SIGMA_NAME)
						dogFilter->setNegativeSigma(updateDoubleProperty(iter->second));
					else if (paramName == POSITIVE_FACTOR_NAME)
						dogFilter->setPositiveFactor(updateDoubleProperty(iter->second));
					else if (paramName == NEGATIVE_FACTOR_NAME)
						dogFilter->setNegativeFactor(updateDoubleProperty(iter->second));
					else if (paramName == FOVEA_RADIUS_NAME)
						dataReducer->setFoveaRadius(updateDoubleProperty(iter->second));
					break;
				}
				case Property::Combo:
				break;
				case Property::String:
				break;
				default:
					throw ISpikeException("Property type not recognized.");
			}
		}
	}

	//Check all properties were updated
	if(!isInitialized() && updatePropertyCount != propertyMap.size())
		throw ISpikeException("Some or all of the properties were not updated: ", updatePropertyCount);
}

