#pragma once
#include "../agent.hpp"

class SerialPVS: public Agent
{
public:
	SerialPVS();
	int findMove(Board board, int depth);
	std::string getName();
	std::string getDescription();
};