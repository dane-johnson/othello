#ifndef SHAREDHASHTABLE_H
#define SHAREDHASHTABLE_H

#include <map>
#include <mutex>
#include <cilk/cilk.h>

#include "../agent.hpp"

typedef struct Ply Ply;
struct Ply {
  Board board;
  int depth;
  bool maximizing;
  Ply(Board b, int d, bool m) {
    board = b;
    depth = d;
    maximizing = m;
  }
};

class SharedHashtable: public Agent {
public:
  SharedHashtable();
  int findMove(Board board, int depth);
  std::string getName();
  std::string getDescription();
  private:
  void update(const Ply &key, const int &val);
  std::mutex write_lock;
  std::map<Ply, int> table;
  int sht(Board board, int depth, int alpha, int beta, bool maximizing);
};

#endif /* SHAREDHASHTABLE_H */
