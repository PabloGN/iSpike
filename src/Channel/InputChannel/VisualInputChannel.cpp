//iSpike includes
#include <iSpike/Channel/InputChannel/VisualInputChannel.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;


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
	addProperty(DoubleProperty(-65, "Parameter C","Parameter C of the Izhikevich Neuron Model",false));
	addProperty(DoubleProperty(2, "Parameter D", "Parameter D of the Izhikevich Neuron Model",false));
	addProperty(DoubleProperty(20, "Current Factor", "Incoming current is multiplied by this value",false));
	addProperty(DoubleProperty(0, "Constant Current", "This value is added to the incoming current", false));

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
		Bitmap tmpBitmap(reader->getBitmap());//Get a copy to avoid threading issues
		dataReducer->setBitmap(tmpBitmap);
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
	bool updateReadOnly = !isInitialized();
	for(map<string,Property>::iterator iter = properties.begin(); iter != properties.end(); ++iter) {
		//In updateReadOnly mode, only update properties that are not read only
		if( (updateReadOnly && !iter->second.isReadOnly()) || !updateReadOnly) {
			string paramName = iter->second.getName();
			switch (iter->second.getType()){
				case Property::Integer: {
					if(paramName == "Opponency Map")
						dogFilter->setOpponencyTypeID(updatePropertyValue(dynamic_cast<IntegerProperty&>(iter->second)));
					else if (paramName == "Neuron Width")
						dataReducer->setOutputWidth(updatePropertyValue(dynamic_cast<IntegerProperty&>(iter->second)));
					else if (paramName == "Neuron Height")
						dataReducer->setOutputHeight(updatePropertyValue(dynamic_cast<IntegerProperty&>(iter->second)));
					break;
				}
				case Property::Double: {
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
					else if (paramName == "Positive Sigma")
						dogFilter->setPositiveSigma(updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second)));
					else if (paramName == "Negative Sigma")
						dogFilter->setNegativeSigma(updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second)));
					else if (paramName == "Positive Factor")
						dogFilter->setPositiveFactor(updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second)));
					else if (paramName == "Negative Factor")
						dogFilter->setNegativeFactor(updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second)));
					else if (paramName == "Fovea Radius")
						dataReducer->setFoveaRadius(updatePropertyValue(dynamic_cast<DoubleProperty&>(iter->second)));
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
}

