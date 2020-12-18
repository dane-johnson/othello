#include "agent.hpp"
#include "agents/human.hpp"
#include "agents/serial_pvs.hpp"
#include "agents/alphabeta.hpp"
#include "agents/sharedhashtable.hpp"
#include <sstream>

int numAgents = 2;
Agent* agents[] = { new Human(),  new SerialPVS(), new AlphaBeta(), new SharedHashtable()};

Agent* getAgentOfType(std::string string) {
	for (int i = 0; i < numAgents; i++) {
		if (string == agents[i]->getName()) {
			return agents[i];
		}
	}
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
