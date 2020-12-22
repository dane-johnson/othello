#pragma once
#include "../agent.hpp"

class FirstAgent: public Agent {
public:
  FirstAgent();
  int findMove(Board board, int depth);
  std::string getName();
  std::string getDescription();
};