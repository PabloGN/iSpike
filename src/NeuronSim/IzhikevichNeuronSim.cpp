//iSpike includes
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iSpike/Log/Log.hpp>
using namespace ispike;

//Other includes
#include <vector>
using namespace std;

/** Empty constructors - parameters have to be set separately */
IzhikevichNeuronSim(){
	this->a = 0.1;
	this->b = 0.2;
	this->c = -65;
	this->d = 2;
	initialized = false;
}


/** Constructor with parameters */
IzhikevichNeuronSim(int numNeurons, float a, float b, float c, float d, float currentFactor, float constantCurrent){
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	initialize(numNeurons);
}


/** Destructor */
IzhikevichNeuronSim::~IzhikevichNeuronSim(){
	if(isInitialized()){
		delete [] vArray;
		delete [] uArray;
		delete [] firedArray;
		delete [] iArray;
	}
}


/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Initializes the simulator, creating arrays and setting parameters */
void IzhikevichNeuronSim::initialize(int numNeurons){
	this->numNeurons = numNeurons;

	//Initialize data structures
	vArray = new float[numNeurons];
	uArray = new float[numNeurons];
	firedArray = new bool[numNeurons];
	iArray = new float[numNeurons];
	for(int n = 0; n < numNeurons; ++n){
		vArray[n] = 0.0;
		uArray[n] = 0.0;
		iArray[n] = 0.0;
	}

	initialized = true;
}


/** Sets the input current to a particular neuron */
void IzhikevichNeuronSim::setInputCurrent(int index, float current){
	if(index >= numNeurons)
		throw ISpikeException("IzhikevichNeuronSim: Index is out of range.");
	iArray[index] = current;
}


/** Advances simulator by one time step */
void IzhikevichNeuronSim::step(){
	if(!isInitialized())
		throw ISpikeException("Izhikevich simulator cannot be stepped without being initialized.");

	//Clear spikes and fired array
	spikeVector.clear();
	for(int n = 0; n < numNeurons; n++)
		firedArray[n] = false;

	//Calculate state of neurons
	for(unsigned int n = 0; n < voltages->size(); n++) {
		for(unsigned int t=0; t<4; ++t) {
			if(!firedArray[n]) {
				this->v[n] += 0.25 * ((0.04* this->v[n] + 5.0) * this->v[n] + 140.0 - this->u[n] + iArray[n]);
				this->u[n] += 0.25 * (this->a * (this->b * this->v[n] - this->u[n]));
				firedArray[n] = v[n] >= 30.0;
			}
		}

		//Neuron has fired -add spike to buffer
		if(firedArray[n]) {
			v[n] = this->c;
			u[n] += this->d;
			spikes.push_back(n);
		}
	}

	//Clear input currents
	for(int i=0; i<numNeurons; ++i)
		iArray = 0.0f;
}

