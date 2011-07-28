/*
 * IzhikevichNeuronSim.cpp
 *
 *  Created on: 6 Feb 2011
 *      Author: Edgars Lazdins
 */

#include <vector>
#include <iSpike/Bitmap.hpp>
#include <iSpike/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iostream>
#include <sstream>
#include <iSpike/Log/Log.hpp>

/** Empty constructors - parameters have to be set separately */
IzhikevichNeuronSim(){

}


/** Constructor with parameters */
IzhikevichNeuronSim(int dimensions, float a, float b, float c, float d, float currentFactor, float constantCurrent){
	this->v = new float[dimensions];
	this->u = new float[dimensions];
	this->a =  a;
	this->b = b;
	this->c = c;
	this->d = d;
	this->currentFactor = currentFactor;
	this->constantCurrent = constantCurrent;
	for(int n = 0; n < dimensions; n++)
	{
		v[n] = 0.0;
		u[n] = 0.0;
	}
}


/** Advances simulator by one time step */
std::vector<int>* IzhikevichNeuronSim::step(std::vector<double>* voltages){
	bool* fired = new bool[voltages->size()];
	for(unsigned int n = 0; n < voltages->size(); n++){
		fired[n] = false;
	}
	float* I = new float[voltages->size()];
	//std::ostringstream currentText;
	//currentText << "Izhikevich Currents: ";
	for(unsigned int n = 0; n < voltages->size(); n++){
		I[n] = (float) ( ( ( voltages->at(n) ) * this->currentFactor ) + this->constantCurrent );
	}

	std::vector<int>* result = new std::vector<int>();
	for(unsigned int n = 0; n < voltages->size(); n++) {
		for(unsigned int t=0; t<4; ++t) {
			if(!fired[n]) {
				this->v[n] += 0.25 * ((0.04* this->v[n] + 5.0) * this->v[n] + 140.0 - this->u[n] + I[n]);
				this->u[n] += 0.25 * (this->a * (this->b * this->v[n] - this->u[n]));
				fired[n] = v[n] >= 30.0;
			}
		}

		if(fired[n]) {
			v[n] = this->c;
			u[n] += this->d;
			result->push_back(n);
		}
	}
	delete[] I;
	delete[] fired;
	return result;
}

