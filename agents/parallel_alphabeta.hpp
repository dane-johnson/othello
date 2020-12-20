#pragma once
#include "../agent.hpp"

class ParallelAlphaBeta: public Agent {
public:
  ParallelAlphaBeta();
  int findMove(Board board, int depth);
  std::string getName();
  std::string getDescription();
};