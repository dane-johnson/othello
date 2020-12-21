#include "jamboree.hpp"

#include <cilk/cilk.h>
#include <cilk/reducer_max.h>

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
  return l.second.evaluate() > r.second.evaluate();
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
  int scouts[moves.size() - 1];
  int cutoff = -1;
  cilk_for (int i = 1; i < moves.size(); i++) {
    int s = -jamboree(moves[i].second, depth -1, -alpha -1, -alpha);
    if (s >= beta) {
      // Beta cutoff
      cutoff = s;
      abort;
    }
    scouts[i - 1] = s;
  }

  if (cutoff > 0) {
    return cutoff;
  }

  // If there was a failed scout, evaluate the window in full
  for (int i = 0; i < moves.size() - 1; i++) {
    std::pair<int, Board> move = moves[i + 1];
    if (scouts[i] > value) {
      value = scouts[i];
    }
    if (scouts[i] > alpha) {
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
  DEBUG(int failed_scouts = 0;)
    DEBUG(int scouts = 0;)
    DEBUG(int nodes = 0;)
    std::vector<std::pair<int, Board>> moves = GenerateMoveBoards(board);
  cilk::reducer< cilk::op_max_index<int, int> > best_move; //soooo this is being misused a bit, but basically we do a max reducer in order to find the best move.  This is supposed to be for an index, but no reason it can't store the move instead
  cilk_for (int i = 0; i < moves.size(); i++) {
    std::pair<int, Board> move_pair = moves[i];
    int score = -jamboree(std::get<Board>(move_pair), depth - 1, -MAX_VALUE - 1, MAX_VALUE + 1);
    best_move->calc_max(std::get<int>(move_pair), score);
  }
  DEBUG(printf("%d nodes searched.  Failed %d scouts out of %d scouts\n", nodes, failed_scouts, scouts);)
    DEBUG(std::cout << std::flush;)
    return best_move->get_index_reference();
}
