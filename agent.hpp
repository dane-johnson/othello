#pragma once
#include "board.hpp"
#include <string>
#include "attacks.hpp"
#include "move_generation.hpp"
#include <iostream>
#define DEBUG(...) 
//__VA_ARGS__

class Agent {
public:
	Agent();
	virtual int findMove(Board board, int depth);
	virtual std::string getName();
	virtual std::string getDescription();
};

Agent* getAgentOfType(std::string string);
std::string getAgentTypesDescriptions();