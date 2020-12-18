#include "sharedhashtable.hpp"

SharedHashtable::SharedHashtable() {
}

bool operator<(const Ply &l, const Ply &r) {
  if (l.board != r.board) {
    return l.board < r.board;
  } else if (l.depth != r.depth)  {
    return l.depth < r.depth;
  } else {
    return l.maximizing < r.maximizing;
  }
}

void SharedHashtable::update(const Ply &key, const int &value) {
  write_lock.lock();
  table[key] = value;
  write_lock.unlock();
}

int SharedHashtable::sht(Board board, int depth, bool maximizing) {
  int value;
  Ply key(board, depth, maximizing);
  // See if someone has done this computation already;
  std::map<Ply, int>::iterator it = table.find(key);
  if (it != table.end()) {
    return it->second;
  }
  // See if we're bottomed out
  if (depth == 0) {
    value = board.evaluate();
    update(key, value);
    return value;
  }

  std::vector<int> moves = GenerateMoves(board);
  if (moves.empty()) {
    value = board.evaluate();
    update(key, value);
    return value;
  }
  if (maximizing) {
    value = -MAX_VALUE;
    for (int move : moves) {
      value = std::max(value, sht(MakeMove(board, move), depth -1, false));
    }
  } else {
    value = MAX_VALUE;
    for (int move : moves) {
      value = std::min(value, sht(MakeMove(board, move), depth -1, false));
    }
  }
  update(key, value);
  return value;
}

int SharedHashtable::findMove(Board board, int depth) {
  std::vector<int> moves = GenerateMoves(board);
  // Send off other threads to fill out the table
  #pragma omp parallel
  {
    for (int move : moves) {
      sht(MakeMove(board, move), depth -1, true);
    }
  }
  int best_value = -MAX_VALUE -1;
  int best_move = -1;
  for (int move : moves) {
    int value = sht(MakeMove(board, move), depth -1, true);
    if (value > best_value) {
      best_value = value;
      best_move = move;
    }
  }
  return best_move;
}
std::string SharedHashtable::getName() {
  return "SharedHashtable";
}
std::string SharedHashtable::getDescription() {
  return "Minimax with a shared lookup table.";
}
