#include "sdm.hpp"
#include <cstdlib>
/////////////////////////////////////////////////////////////////////////////
// Neuron class
/////////////////////////////////////////////////////////////////////////////
Neuron::Neuron(float prah = 0.0) {
	this->prah = prah;
	this->potencial = 0.0;
}

Neuron::~Neuron(){}

void Neuron::Prah(float prah){
	this->prah = prah;
}

float Neuron::GetPrah(void){
	return this->prah;
}


void Neuron::Potencial(float potencial){
	this->potencial = potencial;
}

float Neuron::GetPotencial(void){
	return this->potencial;
}
