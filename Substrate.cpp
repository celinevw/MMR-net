//
// Created by Celly on 23/11/2020.
//

#include "Substrate.h"

void Substrate::assign(NetworkArray net, ParameterObj par, bool allow_loading, std::uint64_t seed) {
	network = net;
	parameters = par;
	mult_loading = allow_loading;
	currenttime = 0.0;
	nick1 = -1.0;
	nick2 = -1.0;

	gen = XoshiroCpp::Xoshiro128PlusPlus (seed);
	dist = std::uniform_real_distribution<> (0,1);
	complexes.emplace_back(network, parameters, gen);
}

Substrate::Substrate(NetworkArray net, ParameterObj par, bool allow_loading, std::uint64_t seed){
	assign(net, par, allow_loading, seed);
}

Substrate::Substrate() {
	NetworkArray mynet;
	ParameterObj myparameters;
	std::uint64_t sd {1000};
	assign(mynet, myparameters, sd, false);
}

void Substrate::main() {
	if(!mult_loading){
		complexes.at(0).main(std::vector<std::vector<int>>());
		nick1 = complexes.at(0).nick1;
		nick2 = complexes.at(0).nick2;
		return;
	}

	float x;
	float dt = complexes.at(0).dt_react;
	float bindingchance = network.transitions.at(1).at(7);
	currenttime = dt;
	int numcomplexes = 1;
	std::array<std::vector<float>, 2> nicks;
	std::vector<std::vector<int>> positions;
	std::vector<int> single_positions;

	while(currenttime < complexes.at(0).totaltime) {
		x = dist(gen);
		//binding moment
		if (x < bindingchance) {
			complexes.emplace_back(network, parameters, gen, currenttime);
			numcomplexes += 1;
		}
		currenttime += dt;
	}
	std::cout << numcomplexes << std::endl;
	currenttime = 0.0;

	for (size_t i = 0; i < complexes.size(); i++){
			single_positions = complexes.at(i).main(positions);
			positions.push_back(single_positions);
		}

	for(ModelInstance protein : complexes){
		if (protein.nick1 != -1) {
			nicks.at(0).push_back(protein.nick1);
		}
		if (protein.nick2 != -1) {
			nicks.at(1).push_back(protein.nick2);
		}
	}
	if (!nicks.at(0).empty()) { //if never nicked, leave at -1
		nick1 = *std::min_element(nicks.at(0).begin(), nicks.at(0).end());
	}
	if (!nicks.at(1).empty()) {
		nick2 = *std::min_element(nicks.at(1).begin(), nicks.at(1).end());
	}

}