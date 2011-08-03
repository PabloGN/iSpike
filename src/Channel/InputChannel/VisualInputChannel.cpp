//iSpike includes
#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>
#include <iSpike/VisualDataReducer/LogpolarVisualDataReducer.hpp>
#include <iSpike/VisualDataReducer/VisualDataReducer.hpp>
#include <iSpike/VisualFilter/DOGVisualFilter.hpp>
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;

//Other includes
#include <boost/asio.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <boost/date_time/posix_time/posix_time_types.hpp>


/** Constructor */
VisualInputChannel::VisualInputChannel() {
	// Properties of log polar foveation
	addProperty(IntegerProperty(100, "Neuron Width", "Width of the neuron network", true));
	addProperty(IntegerProperty(100, "Neuron Height", "Height of the neuron network", true));
	addProperty(DoubleProperty(20, "Fovea Radius", "Radius of the central foveated area", true));

	//Properties of the difference of gaussians filter
	addProperty(DoubleProperty(2, "Positive Sigma", "Positive Gaussian Sigma", false));
	addProperty(DoubleProperty(4, "Negative Sigma", "Negative Gaussian Sigma", false));
	addProperty(DoubleProperty(4, "Positive Factor", "Multiplication ratio for positive image during subtraction", false));
	addProperty(DoubleProperty(2, "Negative Factor", "Multiplication ratio for negative image during subtraction", false));
	addProperty(IntegerProperty(0, "Opponency Map", "Which colour oponency map to use (0 = R+G-; 1 = G+R-; 2 = B+Y-)", true));

	//Properties of the neural simulator
	addProperty(DoubleProperty(0.1, "Parameter A", "Parameter A of the Izhikevich Neuron Model", false));
	addProperty(DoubleProperty(0.2, "Parameter B","Parameter B of the Izhikevich Neuron Model",false));
	addProperty(DoubleProperty(-65, "Parameter C","Parameter C of the Izhikevich Neuron Model",false));I
	addProperty(DoubleProperty(2, "Parameter D", "Parameter D of the Izhikevich Neuron Model",false));
	addProperty(DoubleProperty(20, "Current Factor", "Incoming current is multiplied by this value",false));
	addProperty(DoubleProperty(0, "Constant Current", "This value is added to the incoming current", false));

	//Create the description
	channelDescription = InputChannelDescription("Visual Input Channel", "This is a visual input channel", "Visual Reader");

	//Set up visual processing classes and neural simulator
	dataReducer = new LogPolarVisualDataReducer();
	dogFilter = new DOGVisualFilter(dataReducer);

	//Initialize variables
	reader = NULL;
	currentImageID = 0;
}


/** Destructor */
VisualInputChannel::~VisualInputChannel() {
	LOG(LOG_DEBUG) << "Entering VisualInputChannel destructor";
	if(isInitialised()) {
		delete reader;
		delete dogFilter;
		delete dataReducer;
	}
	LOG(LOG_DEBUG) << "Exiting VisualInputChannel destructor";
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Initialises the channel with the default parameters */
void VisualInputChannel::initialize(VisualReader* reader, std::map<std::string,Property*> properties) {
	if(reader == NULL)
		throw iSpikeException("Cannot initialize VisualInputChannel with a null reader.");
	this->reader = reader;
	this->reader->start();

	//Store properties in this and dependent classes
	updateProperties(properties, false);

	//Initialize neural simulator
	neuronSim.initialize(getWidth() * getHeight());

	setInitialized(true);
}


/** Sets the properties. This will be done immediately if we are not stepping or deferred until the end of the step */
void JointInputChannel::setProperties(map<string,Property>& properties){
	updateProperties(properties, true);
}


//Inherited from Channel
void VisualInputChannel::step() {
	//Check reader for errors
	if(reader->isError()){
		LOG(LOG_CRITICAL)<<"AngleReader Error: "<<reader->getError();
		throw ISpikeException("Error in AngleReader");
	}

	//Update visual maps if necessary
	if(reader->getImageID() != currentImageID){
		currentImageID = reader->getImageID();
		Bitmap tmpBitmap(reader->getBitmap());//Get a copy to avoid threading issues
		dataReducer->setBitmap(tmpBitmap);
		dogFilter->update();
	}


	//Load opponency data into neural simulator
	if(!opponencyMap.isEmpty()) {
		//Get reference to opponency map
		Bitmap& opponencyMap = dogFilter->getOpponencyMap();
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
void VisualInputChannel::updateProperties(map<string, Property>& properties, bool updateReadOnly) {
	if(propertyMap.size() != properties.size())
		throw iSpikeException("VisualInputChannel: Current properties do not match new properties.");

	//Update properties in the property map and the appropriate class
	bool updateReadOnly = !isInitialized();
	for(map<string,Property>::iterator iter = properties.begin(); iter != properties.end(); ++iter) {
		//In updateReadOnly mode, only update properties that are not read only
		if( (updateReadOnly && !iter->second.isReadOnly()) || !updateReadOnly) {
			string paramName = iter->second.getName();
			switch (iter->second.getType()){
				case Property::Integer: {
					if(paramName == "Opponency Map")
						dogFilter->setOpponencyTypeID(updatePropertyValue(iter->second));
					else if (paramName == "Neuron Width")
						dataReducer->setOuputWidth(updatePropertyValue(iter->second));
					else if (paramName == "Neuron Height")
						dataReducer->setOuputHeight(updatePropertyValue(iter->second));
					break;
				}
				case Property::Double: {
					if (paramName == "Parameter A")
						neuronSim->setParameterA(updatePropertyValue(iter->second));
					else if (paramName == "Parameter B")
						neuronSim->setParameterB(updatePropertyValue(iter->second));
					else if (paramName == "Parameter C")
						neuronSim->setParameterC(updatePropertyValue(iter->second));
					else if (paramName == "Parameter D")
						neuronSim->setParameterD(updatePropertyValue(iter->second));
					else if (paramName == "Current Factor")
						currentFactor = updatePropertyValue(iter->second);
					else if (paramName == "Constant Current")
						constantCurrent = updatePropertyValue(iter->second);
					else if (paramName == "Positive Sigma")
						dogFilter->setPositiveSigma(updatePropertyValue(iter->second));
					else if (paramName == "Negative Sigma")
						dogFilter->setNegativeSigma(updatePropertyValue(iter->second));
					else if (paramName == "Positive Factor")
						dogFilter->setPositiveFactor(updatePropertyValue(iter->second));
					else if (paramName == "Negative Factor")
						dogFilter->setNegativeFactor(updatePropertyValue(iter->second));
					else if (paramName == "Fovea Radius")
						dataReducer->setFoveaRadius(updatePropertyValue(iter->second));
					break;
				}
				case Property::Combo:
				break;
				case Property::String:
				break;
				default:
					throw iSpikeException("Property type not recognized.");
			}
		}
	}
}

