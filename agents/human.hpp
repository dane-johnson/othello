#pragma once
#include "../agent.hpp"

class Human: public Agent
{
public:
	Human();
	int findMove(Board board, int depth);
	std::string getName();
	std::string getDescription();
};