/*
 * IzhikevichNeuronSim.cpp
 *
 *  Created on: 6 Feb 2011
 *      Author: cembo
 */

#include <vector>
#include <SpikeAdapter/Bitmap.hpp>
#include <SpikeAdapter/NeuronSim/IzhikevichNeuronSim.hpp>
#include <iostream>

std::vector<int>* IzhikevichNeuronSim::getSpikes(Bitmap* voltageMap)
{
  bool* fired = new bool[voltageMap->getWidth() * voltageMap->getHeight()];
  for(int n = 0; n < voltageMap->getWidth() * voltageMap->getHeight(); n++)
  {
    fired[n] = false;
  }
  float* I = new float[voltageMap->getWidth() * voltageMap->getHeight()];
  for(int n = 0; n < voltageMap->getWidth() * voltageMap->getHeight(); n++)
  {
    I[n] = (float) ( ( ( voltageMap->getContents()[n] ) / 16 ) + 4 );
  }

  std::vector<int>* result = new std::vector<int>();
  for(int n = 0; n < voltageMap->getWidth() * voltageMap->getHeight(); n++)
  {
    for(unsigned int t=0; t<4; ++t) {
             if(!fired[n]) {
                     this->v[n] += 0.25 * ((0.04* this->v[n] + 5.0) * this->v[n] + 140.0 - this->u[n] + I[n]);
                     this->u[n] += 0.25 * (this->a * (this->b * this->v[n] - this->u[n]));
                     //std::cout << I[n] << ",";
                     fired[n] = v[n] >= 30.0;
             }
    }

    if(fired[n]) {
             v[n] = this->c;
             u[n] += this->d;
             result->push_back(n);
    }
  }
  delete fired;
  delete I;
  //std::cout << "[" << v[0] << "," << u[0] << "]" << std::endl;
  return result;
}

