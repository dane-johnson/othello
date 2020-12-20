#ifndef ALPHABETA_H
#define ALPHABETA_H

#include "../agent.hpp"

class AlphaBeta: public Agent {
public:
  AlphaBeta();
  int findMove(Board board, int depth);
  std::string getName();
  std::string getDescription();
};

#endif /* ALPHABETA_H */
