#ifndef JAMBOREE_H
#define JAMBOREE_H

#include "../agent.hpp"

class Jamboree: public Agent {
public :
  Jamboree();
  int findMove(Board board, int depth);
  std::string getName();
  std::string getDescription();
};

#endif /* JAMBOREE_H */
