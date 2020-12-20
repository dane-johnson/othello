#pragma once
#include "../agent.hpp"

class ParallelPVS: public Agent
{
public:
	ParallelPVS();
	int findMove(Board board, int depth);
	std::string getName();
	std::string getDescription();
};