/*
 * IzhikevichNeuronSim.hpp
 *
 *  Created on: 6 Feb 2011
 *      Author: Edgars Lazdins
 */

#ifndef IZHIKEVICHNEURONSIM_HPP_
#define IZHIKEVICHNEURONSIM_HPP_

#include <iSpike/NeuronSim/NeuronSim.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

/**
 * @class IzhikevichNeuronSim
 * @brief IzhikevichNeuronSim class
 *
 * Neuron simulator using the Izhikevich model of neurons
 *
 * @author Edgars Lazdins
 *
 */
class IzhikevichNeuronSim : public NeuronSim {

private:
std::vector<int>* spikes;
float a;
float b;
float c;
float d;
float* v;
float* u;

public:

/**
 * Converts the voltage map into a spike pattern
 */
std::vector<int>* getSpikes(std::vector<double>* voltages);

/**
 * Constructor, parameters a-d as in the Izhikevich model
 */
IzhikevichNeuronSim(int dimensions, float a, float b, float c, float d)
{
  this->v = new float[dimensions];
  this->u = new float[dimensions];
  this->a =  a;
  this->b = b;
  this->c = c;
  this->d = d;
  for(int n = 0; n < dimensions; n++)
  {
    v[n] = 0.0;
    u[n] = 0.0;
  }
}

};


#endif /* IZHIKEVICHNEURONSIM_HPP_ */
