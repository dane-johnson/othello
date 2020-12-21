#include "jamboree.hpp"

#include <cilk/cilk.h>

#include <algorithm>

Jamboree::Jamboree() {
}

std::string Jamboree::getName() {
  return "Jamboree";
}

std::string Jamboree::getDescription() {
  return "PVS with the Young Brothers Wait concept.";
}

bool jambcompare(std::pair<int, Board> l, std::pair<int, Board> r) {
  return l.second.evaluate() < l.second.evaluate();
}

int jamboree(Board board, int depth, int alpha, int beta) {
  if (depth <= 0) return board.evaluate();
  std::vector<std::pair<int, Board>> moves = GenerateMoveBoards(board);
  if (moves.empty()) {
    int eval = board.isWinning();
    if (eval > 0) return MAX_VALUE;
    if (eval < 0) return -MAX_VALUE;
    return 0;
  }
  std::sort(moves.begin(), moves.end(), jambcompare);
  std::pair<int, Board> primary_variation = moves[0];
  // Evaluate the primary variation, younger brothers wait.
  int value;
  value = -jamboree(primary_variation.second, depth -1, -beta, -alpha);
  if (value >= beta) { // Beta cutoff
    return value;
  }
  if (value > alpha) {
    alpha = value;
  }
  // Scout the remaining variations
  bool failed = false;
  cilk_for (int i = 1; i < moves.size(); i++) {
    int s = -jamboree(moves[i].second, depth -1, -alpha -1, -alpha);
    if (s > value) {
      failed = true;
      //abort;
    }
  }

  // If there was a failed scout, evaluate all variations in full
  if (failed) {
    for (int i = 1; i < moves.size(); i++) {
      std::pair<int, Board> move = moves[i];
      value = std::max(value, -jamboree(move.second, depth -1, -beta, -alpha));
      alpha = std::max(alpha, value);
      if (alpha >= beta) { // Beta cutoff
	return value;
      }
    }
  }
  
  return value;
}

int Jamboree::findMove(Board board, int depth) {
  int best_value = -MAX_VALUE -1;
  int best_move = -1;
  std::vector<std::pair<int, Board>> moves = GenerateMoveBoards(board);
  for (std::pair<int, Board> move_pair : moves) {
    int value = -jamboree(move_pair.second, depth -1, best_value, MAX_VALUE);
    if (value > best_value) {
      best_value = value;
      best_move = move_pair.first;
    }
  }
  return best_move;
}
