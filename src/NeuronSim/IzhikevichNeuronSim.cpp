#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iSpike/ISpikeException.hpp>
#include <iSpike/Log/Log.hpp>

#include <algorithm>

using namespace ispike;


/** Empty constructors - parameters have to be set separately */
IzhikevichNeuronSim::IzhikevichNeuronSim() :
	numNeurons(0),
	aParam(0.1),
	bParam(0.2),
	cParam(-65.0),
	dParam(2.0)
{
	;
}


/** Constructor with parameters */
IzhikevichNeuronSim::IzhikevichNeuronSim(size_t numNeurons,
		double a, double b, double c, double d) :
	numNeurons(numNeurons),
	aParam(a),
	bParam(b),
	cParam(c),
	dParam(d),
	vArray(numNeurons, c),
	uArray(numNeurons, b*c),
	iArray(numNeurons, 0.0),
	firedArray(numNeurons, false)
{
	;
}



/*--------------------------------------------------------------------*/
/*---------                 PUBLIC METHODS                     -------*/
/*--------------------------------------------------------------------*/

/** Re-initializes the simulator, creating arrays and setting parameters */
void IzhikevichNeuronSim::initialize(size_t numNeurons){

	LOG(LOG_DEBUG)<<"Initializing neuron simulator with "<<numNeurons<<" neurons.";
	this->numNeurons = numNeurons;

	vArray.clear();
	uArray.clear();
	iArray.clear();
	firedArray.clear();

	vArray.resize(numNeurons, cParam);
	uArray.resize(numNeurons, bParam * cParam);
	iArray.resize(numNeurons, 0.0);
	firedArray.resize(numNeurons, false);
}


/** Sets the input current to a particular neuron */
void IzhikevichNeuronSim::setInputCurrent(unsigned index, double current){
	if(index >= numNeurons) {
		throw ISpikeException("IzhikevichNeuronSim: Index is out of range.");
	}
	iArray[index] = current;
}


/** Advances simulator by one time step */
void IzhikevichNeuronSim::step(){

	//Clear spikes and fired array
	spikeVector.clear();
	std::fill(firedArray.begin(), firedArray.end(), false);

	//Calculate state of neurons
	for(unsigned n = 0; n < numNeurons; ++n) {
		for(unsigned t=0; t<4; ++t) {
			if(!firedArray[n]) {
				vArray[n] += 0.25 * ((0.04* vArray[n] + 5.0) * vArray[n] + 140.0 - uArray[n] + iArray[n]);
				uArray[n] += 0.25 * (aParam * (bParam * vArray[n] - uArray[n]));
				firedArray[n] = vArray[n] >= 30.0;
			}
		}
		iArray[n] = 0.0;

		//Neuron has fired -add spike to buffer
		if(firedArray[n]) {
			vArray[n] = cParam;
			uArray[n] += dParam;
			spikeVector.push_back(n);
		}
	}
}

