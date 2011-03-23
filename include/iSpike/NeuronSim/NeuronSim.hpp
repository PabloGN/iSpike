/*
 * NeuronSim.hpp
 *
 *  Created on: 6 Feb 2011
 *      Author: Edgars Lazdins
 */

#ifndef NEURONSIM_HPP_
#define NEURONSIM_HPP_

/**
 * @class NeuronSim
 * @brief NeuronSim class
 *
 * This class represents a Neuron Simulation layer
 * This class retrieves a voltage map and runs it through a layer
 * of Neurons, outputting the resultant spikes
 *
 * @author Edgars Lazdins
 *
 */
class NeuronSim {

private:

public:

/**
 * Convert voltage map into a spike pattern
 */
virtual std::vector<int>* getSpikes(std::vector<double>* voltages) = 0;

};


#endif /* NEURONSIM_HPP_ */
