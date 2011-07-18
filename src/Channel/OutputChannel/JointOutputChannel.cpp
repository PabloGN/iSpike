//iSpike includes
#include <iSpike/Channel/OutputChannel/JointOutputChannel.hpp>
#include <iSpike/Log/Log.hpp>
#include <iSpike/ISpikeException.hpp>
using namespace ispike;

//Other includes
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <sstream>


/** Constructor */
JointOutputChannel::JointOutputChannel(){
	//First define the properties of this channel
	properties["Minimum Angle"] = new DoubleProperty(-90, "Minimum Angle", "The minimum angle to read", true);
	properties["Maximum Angle"] = new DoubleProperty(90, "Maximum Angle", "The maximum angle to read", true);
	properties["Rate Of Decay"] = new DoubleProperty(0.005,"Rate Of Decay",  "The rate of decay of the angle variables", false);
	properties["Neuron Width"] = new IntegerProperty(10, "Neuron Width", "Width of the neuron network", true);
	properties["Neuron Height"] = new IntegerProperty(1, "Neuron Height", "Height of the neuron network", true);

	//Create the description
	channelDescription.reset(new OutputChannelDescription("Joint Output Channel", "This channel converts a pattern of spikes into an angle and writes it", "Angle Writer", properties));
}


/** Destructor */
JointOutputChannel::~JointOutputChannel(){
	LOG(LOG_DEBUG) << "Entering JointOutputChannel destructor";
	if(isInitialized()){
		delete [] variables;
	}
	LOG(LOG_DEBUG) << "Exiting JointOutputChannel destructor";
}


void JointOutputChannel::setFiring(std::vector<int>& buffer){
	//Work through the neuron ids in the buffer
	for(vector<int>::iterator iter =buffer.begin(); iter != buffer.end(); ++ iter){
		variables[*iter] += currentIncrement;
	}
}


/** Initialises the Joint Input Channel with the parameters
	 @param reader The associated Angle Reader */
void JointOutputChannel::initialise(AngleWriter* writer, map<string, Property> properties){
	if(reader == NULL)
		throw iSpikeException("Cannot initialize JointOutputChannel with a null reader.");
	this->writer = writer;
	this->writer->start();

	updateProperties(properties, false);

	//Set up current variables
	numVariables = getWidth() * getHeight();
	variables = new double[numVariables];

	setInitialized(true);
}


void JointOutputChannel::step(){
	isStepping = true;

    LOG(LOG_DEBUG) << "Entering JointOutputChannel work loop, stopRequested is " << stopRequested;
    bool enoughFrames = false;
    {
      boost::mutex::scoped_lock lock(this->mutex);
      //enoughFrames = !(this->buffer->empty());
      enoughFrames = !(this->buffer->empty());
    }
    /**
     * Spikes received
     */
	if(enoughFrames) {
      //std::vector<int> currentFrame = this->buffer->front();
      std::vector<int> currentFrame = *(this->buffer);
      {
        boost::mutex::scoped_lock lock(this->mutex);
        //this->buffer->pop();
        this->buffer->clear();
      }
      for(unsigned int neuronID = 0; neuronID < currentFrame.size(); neuronID++)
      {
        variables[currentFrame[neuronID]]++;
        times[currentFrame[neuronID]] = 0;
      }
      double angleSum = 0;
      double weightSum = 0;
      for(unsigned int j = 0; j < variables.size(); j++)
      {
        double currentAngle = (this->maxAngle - this->minAngle) / ((this->width * this->height)-1) * j + this->minAngle;
        angleSum += variables[j] * currentAngle;
        weightSum += variables[j];
      }
      if(!weightSum == 0)
      {
        double angle = angleSum / weightSum;
        LOG(LOG_DEBUG) << "Angle: " << angle;
        this->currentAngle = angle;
        this->writer->addAngle(angle);
      }
    }
	/* Decay the variables according to the following function:
     * N(t+1) = N(t)*e^(-rateOfDecay*t)
     */
	for(unsigned int i=0; i < numVariables; ++i) {
      variables[i] = variables[i] * exp(-(this->rateOfDecay) * times[i]);
	  FIXME MAKE THIS A PROPER EXPONENTIAL FUNCTION

      //variableText << variables[i] << ", ";
      //fileStream << variables[i] << ",";
      if(times[i] < 100000)
        times[i]++;
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
void JointOutputChannel::updateProperties(std::map<std::string,Property*> properties, bool updateReadOnly) {
  for(std::map<std::string,Property*>::const_iterator iter = properties.begin(); iter != properties.end(); ++iter)
  {
	if(!updateReadOnly)
	  ///Check if any of the properties are read only
	  if(this->getChannelDescription().getChannelProperties()[iter->first]->isReadOnly())
		throw ISpikeException("Cannot update read-only parameters");

	///Update the properties, this is ugly and should be improved
	std::string paramName = iter->second->getName();
	switch (iter->second->getType())
	{
	  case Property::Integer:
	  {
		int value = ((IntegerProperty*)(iter->second))->getValue();
		if (paramName == "Neuron Width")
		  this->width = value;
		else if (paramName == "Neuron Height")
		  this->height = value;
		break;
	  }
	  case Property::Double:
	  {
		double value = ((DoubleProperty*)(iter->second))->getValue();
		if (paramName == "Minimum Angle")
		  this->minAngle = value;
		else if (paramName == "Maximum Angle")
		  this->maxAngle = value;
		else if (paramName == "Rate Of Decay")
		  this->rateOfDecay = value;
		break;
	  }
	  case Property::Combo:
	  case Property::String:
	  {
		std::string value;
		if(iter->second->getType() == Property::String)
		  value = ((StringProperty*)(iter->second))->getValue();
		else
		  value = ((ComboProperty*)(iter->second))->getValue();
		break;
	  }
	}
  }
}


