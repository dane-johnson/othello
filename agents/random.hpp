#pragma once
#include "../agent.hpp"

class RandomAgent: public Agent
{
public:
	RandomAgent();
	int findMove(Board board, int depth);
	std::string getName();
	std::string getDescription();
};