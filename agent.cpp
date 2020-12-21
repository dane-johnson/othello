#include "agent.hpp"
#include "agents/human.hpp"
#include "agents/serial_pvs.hpp"
#include "agents/alphabeta.hpp"
#include "agents/sharedhashtable.hpp"
#include "agents/random.hpp"
#include "agents/parallel_pvs.hpp"
#include "agents/parallel_alphabeta.hpp"
#include "agents/jamboree.hpp"
#include <sstream>

int numAgents = 8;
Agent* agents[] = { new Human(),  new SerialPVS(), new AlphaBeta(), new SharedHashtable(), new RandomAgent(), new ParallelPVS(), new ParallelAlphaBeta(), new Jamboree()};

Agent* getAgentOfType(std::string string) {
	for (int i = 0; i < numAgents; i++) {
		if (string == agents[i]->getName()) {
			return agents[i];
		}
	}
	return agents[0];
}

std::string getAgentTypesDescriptions() {
	std::stringstream ss;
	for (int i = 0; i < numAgents; i++) {
		ss << agents[i]->getName() << " :: " << agents[i]->getDescription() << "\n";
	}
	return ss.str();
}

int Agent::findMove(Board board, int depth) {
	return 0;
}

std::string Agent::getName() {
	return "Raw Agent -- If you see this something went wrong";
}

std::string Agent::getDescription() {
	return "This is a virtual class for inheritance -- If you see this something went wrong";
}

Agent::Agent() {

}
