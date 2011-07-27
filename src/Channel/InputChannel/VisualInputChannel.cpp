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
	// Create the properties of this channel
	addProperty(IntegerProperty(320, "Neuron Width", "Width of the neuron network", true));
	addProperty(IntegerProperty(240, "Neuron Height", "Height of the neuron network", true));
	addProperty(DoubleProperty(20, "Fovea Radius", "Radius of the central foveated area", true));
	addProperty(DoubleProperty(2, "Plus Sigma", "Positive Gaussian Sigma", false));
	addProperty(DoubleProperty(4, "Minus Sigma", "Negative Gaussian Sigma", false));
	addProperty(DoubleProperty(4, "Ratio 1", "Multiplication ratio for positive image during subtraction", false));
	addProperty(DoubleProperty(2, "Ratio 2", "Multiplication ratio for negative image during subtraction", false));
	addProperty(IntegerProperty(0, "Opponency Map", "Which colour oponency map to use (0 = R+G-; 1 = G+R-; 2 = B+Y-)", true));
	addProperty(DoubleProperty(0.1, "Parameter A", "Parameter A of the Izhikevich Neuron Model", false));
	addProperty(DoubleProperty(0.2, "Parameter B","Parameter B of the Izhikevich Neuron Model",false));
	addProperty(DoubleProperty(-65, "Parameter C","Parameter C of the Izhikevich Neuron Model",false));
	addProperty(DoubleProperty(2, "Parameter D", "Parameter D of the Izhikevich Neuron Model",false));
	addProperty(DoubleProperty(20, "Current Factor", "Incoming current is multiplied by this value",false));
	addProperty(DoubleProperty(0, "Constant Current", "This value is added to the incoming current", true));

	//Create the description
	channelDescription = InputChannelDescription("Visual Input Channel", "This is a visual input channel", "Visual Reader");

	//Initialize variables
	reader = NULL;
	copyProperties = false;
	neuronSim = NULL;
	currentImageID = 0;
}


/** Destructor */
VisualInputChannel::~VisualInputChannel() {
	LOG(LOG_DEBUG) << "Entering VisualInputChannel destructor";
	if(isInitialised()) {
		delete dogFilter;
		delete dataReducer;
		delete neuronSim;
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

	updateProperties(properties, false);

	//Set up visual processing classes
	dataReducer = new LogPolarVisualDataReducer(getWidth(), getHeight(), foveaRadius);
	dogFilter = new DOGVisualFilter(dataReducer, plusSigma, minusSigma, ratio1, ratio2, opponentMapID);

	//Create neural simulator
	neuronSim = new IzhikevichNeuronSim(getWidth() * getHeight(), parameterA, parameterB, parameterC, parameterD, currentFactor, constantCurrent);

	setInitialized(true);
}


/** Sets the properties. This will be done immediately if we are not stepping or deferred until the end of the step */
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


//Inherited from Channel
void VisualInputChannel::step() {
	isStepping = true;

	//Check reader for errors
	if(reader->isError()){
		LOG(LOG_CRITICAL)<<"AngleReader Error: "<<reader->getError();
		throw ISpikeException("Error in AngleReader");
	}

	//Update visual maps
	if(reader->getImageID() != currentImageID){
		currentImageID = reader->getImageID();
		Bitmap tmpBitmap(reader->getBitmap());
		dataReducer->setBitmap(tmpBitmap);
		dogFilter->update();
	}

	//Get reference to opponency map
	Bitmap& opponencyMap = dogFilter->getOpponencyMap();

	///Check it was retrieved successfully
	if(!opponencyMap.isEmpty()) {
		///create a current map for the neuron simulator
		std::vector<double>* currents = new std::vector<double>(getWidth() * getHeight());
		for(int i = 0; i < width; i++){
			for(int j = 0; j < height; j++){
				///retrieve the pixel intensity at the coordinates
				double current = (unsigned int)opponentMap.getPixel(this->xOffset + i,this->yOffset + j);

				///move it to the current map
				currents->at(j*(this->width) + i) = current;
			}
		}
		buffer->push_back(*(this->neuronSim->getSpikes(currents)));
		delete currents;
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

/**  Updates the properties by first checking if any are read-only */
void VisualInputChannel::updateProperties(map<string, Property>& properties, bool updateReadOnly) {
	if(propertyMap.size() != properties.size())
		throw iSpikeException("VisualInputChannel: Current properties do not match new properties.");

	for(std::map<std::string,Property*>::const_iterator iter = properties.begin(); iter != properties.end(); ++iter) {
		//Check property exists
		if(propertyMap.count(iter->first) == 0){
			LOG(LOG_CRITICAL) << "VisualInputChannel: Property does not exist: " << iter->first<<endl;
			throw ISpikeException("VisualInputChannel: Property not recognized");
		}

		//In updateReadOnly mode, only update properties that are not read only
		if((updateReadOnly && !propertyMap[iter->first].isReadOnly()) || !updateReadOnly) {
			string paramName = iter->second.getName();
			switch (iter->second.getType()){
				case Property::Integer: {
					int value = ((IntegerProperty)(iter->second)).getValue();
					((IntegerProperty)propertyMap[paramName]).setValue(value);

					if(paramName == "Opponency Map")
						this->opponentMap = value;
					else if (paramName == "Neuron Width")
						setWidth(value);
					else if (paramName == "Neuron Height")
						setHeight(value);
					break;
				}
				case Property::Double: {
					double value = ((DoubleProperty)(iter->second)).getValue();
					((DoubleProperty)propertyMap[paramName]).setValue(value);

					if (paramName == "Parameter A")
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
					else if (paramName == "Plus Sigma")
						this->plusSigma = value;
					else if (paramName == "Minus Sigma")
						this->minusSigma = value;
					else if (paramName == "Ratio 1")
						this->ratio1 = value;
					else if (paramName == "Ratio 2")
						this->ratio2 = value;
					else if (paramName == "Fovea Radius")
						foveaRadius = value;

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

