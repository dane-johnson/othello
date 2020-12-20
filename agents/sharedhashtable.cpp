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

int SharedHashtable::sht(Board board, int depth, int alpha, int beta, bool maximizing) {
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
      value = std::max(value, sht(MakeMove(board, move), depth - 1, alpha, beta, false));
      alpha = std::max(value, alpha);
      if (alpha >= beta) { // Beta cutoff
	break;
      }
    }
  } else {
    value = MAX_VALUE;
    for (int move : moves) {
      value = std::min(value, sht(MakeMove(board, move), depth - 1, alpha, beta, true));
      beta = std::min(value, beta);
      if (beta <= alpha) { // Alpha cutoff
	break;
      }
    }
  }
  update(key, value);
  return value;
}

int SharedHashtable::findMove(Board board, int depth) {
  std::vector<int> moves = GenerateMoves(board);
  // Send off other threads to fill out the table
  cilk_for (int i = 0; i < moves.size(); i++) {
    sht(MakeMove(board, moves[i]), depth - 1, -MAX_VALUE, MAX_VALUE, true);
  }
  int best_value = -MAX_VALUE - 1;
  int best_move = -1;
  cilk_for (int i = 0; i < moves.size(); i++) {
    int value = sht(MakeMove(board, moves[i]), depth - 1, -MAX_VALUE, MAX_VALUE, true);
    if (value > best_value) {
      best_value = value;
      best_move = moves[i];
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
