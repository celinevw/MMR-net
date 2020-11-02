//
// Created by Celly on 20/10/2020.
//

#include "ModelInstance.h"

ModelInstance::ModelInstance(NetworkArray net, ParameterObj par, float x) {
	position = net.mismatchsite; // starting position and mismatch position must be the same
	network = net;
	stepsize = 1;
	state = 1;					// graph is symmetric, so let all start in state 1
	diffusioncoefficient = net.diffusion.at(state);
	nick1 = -1;
	nick2 = -1;
	currenttime = 0;
	topology = par.top;
}

ModelInstance::ModelInstance() : ModelInstance(NetworkArray(), ParameterObj(), 0.0){}

int ModelInstance::getState() {
	return this->state;
}

int ModelInstance::getPosition() {
	return position;
}

/* Randomly decide direction. Check whether new position lies within boundaries
 * If so, update step, else don't.
 */
void ModelInstance::setStep(float x) {
	//TODO: find step size per type of complex

	// Choose direction
	int direction;
	if(x<0.5){
		direction = -1;
	}
	else{
		direction = 1;
	}

	// If not stepping off DNA, add step to position
	if(position + direction * stepsize >= 0 && position + direction * stepsize < network.length){
		position += (direction * stepsize);
	}

}

/* Choose transition to follow based on random number x.
 */
void ModelInstance::transition(float x) {
	// No outgoing edges, then stay in this state
	if (std::accumulate(network.transitions.at(state).begin(), network.transitions.at(state).end(), 0.0) == 0){
		return;
	}

	// Iterate over outgoing edges, find the one to take depending on the random number x
	std::array<float, 36> cumulative{};
	std::partial_sum(network.transitions.at(state).begin(), network.transitions.at(state).end(), cumulative.begin());
	int index = 0;
	for (float threshold : cumulative){
		if (x < threshold){
			//It is not one of transitions where Si binds the mismatch
			bool attachingSi = (state < 6 && index == state + 6) ||
							   (state % 6 == 0 && index == state + 1);
			if (!attachingSi || std::abs(position - network.mismatchsite) < 2 * stepsize){
				// if not adding Si, position does not matter, else make sure it is close enough or do nothing
				state = index;
				//update stepsize/diffusion coefficient
			}
			break;
		}
		index++;
	}
}

void ModelInstance::nicking(){
	if (state > 29 || state % 6 == 5){ // if one complex is SLH, possible nicking
		if (position - network.nickingsite1 < stepsize){
			nick1 = currenttime;
		}
		if (position - network.nickingsite2 < stepsize){
			nick2 = currenttime;
		}
	}
}

/* main method, one run of a model instance
 */
void ModelInstance::main() {
	float shorttime = 0.001;
	float longtime = 0.5;

	//ToDo: way to iterate over random numbers
	float x = 0.5;

	for(int i=0; i < 300000; i++) { //ToDO: make general for other durations and time steps
		setStep(x);

		if(i % 500 == 0) {
			currenttime = shorttime * i; //update only needed when time may be used
			transition(x);
			nicking();
		}
	}
}