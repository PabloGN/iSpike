#ifndef IZHIKEVICHNEURONSIM_HPP_
#define IZHIKEVICHNEURONSIM_HPP_

//Other includes
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <vector>
using namespace std;

namespace ispike {

/** Neuron simulator using the Izhikevich model of neurons */
class IzhikevichNeuronSim {
public:
	IzhikevichNeuronSim();
	IzhikevichNeuronSim(int dimensions, float a, float b, float c, float d, float currentFactor, float constantCurrent);
	~IzhikevichNeuronSim();
	vector<int>* step(std::vector<double>* voltages);
	void initialize();

private:
vector<int>* spikes;
float a;
float b;
float c;
float d;
float* v;
float* u;
float currentFactor;
float constantCurrent;

};


#endif /* IZHIKEVICHNEURONSIM_HPP_ */
