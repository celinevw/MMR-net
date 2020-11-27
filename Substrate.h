//
// Created by Celly on 23/11/2020.
//

#ifndef MARKOVNET_SUBSTRATE_H
#define MARKOVNET_SUBSTRATE_H

#include <vector>
#include <tuple>
#include <algorithm>
#include "ModelInstance.h"

class Substrate{
private:
	NetworkArray network;
	ParameterObj parameters;
	XoshiroCpp::Xoshiro128PlusPlus gen;
	std::uniform_real_distribution<> dist;
public:
	Substrate(NetworkArray net, ParameterObj par, bool allow_loading, std::uint64_t seed);
	Substrate();
	void assign(NetworkArray net, ParameterObj par, bool allow_loading, std::uint64_t seed);
	std::vector<ModelInstance> complexes;
	float currenttime{};
	bool mult_loading{};
	float nick1{};
	float nick2{};
	void main();
};

#endif //MARKOVNET_SUBSTRATE_H
