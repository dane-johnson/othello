#ifndef SHAREDHASHTABLE_H
#define SHAREDHASHTABLE_H

#include <tbb/concurrent_hash_map.h>
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

struct PlyCompare {
  static size_t hash(const Ply &p) {
    return std::hash<U64>()(p.board.white) ^
      std::hash<U64>()(p.board.black) ^
      std::hash<U64>()(p.depth) ^
      p.maximizing;
  }
  static bool equal(const Ply &l, const Ply &r) {
    return l.board == r.board && l.depth == r.depth && l.maximizing == r.maximizing;
  }
};

typedef tbb::concurrent_hash_map<Ply, int, PlyCompare> Movetable;

class SharedHashtable: public Agent {
public:
  SharedHashtable();
  int findMove(Board board, int depth);
  std::string getName();
  std::string getDescription();
private:
  void update(const Ply &key, const int &val);
  std::mutex write_lock;
  Movetable table;
  int sht(Board board, int depth, int alpha, int beta, bool maximizing);
};

#endif /* SHAREDHASHTABLE_H */
