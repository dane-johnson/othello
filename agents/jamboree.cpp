#include "jamboree.hpp"
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
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

int serialJamboree(Board board, int depth, int a, int b) {
  if (depth <= 0) return board.evaluate();  //if we've hit bottom depth
  std::vector<std::pair<int, Board>> moves = GenerateMoveBoards(board);
  //for (std::pair<int, Board> move_pair : moves) { std::cout << std::get<int>(move_pair) << ", ";} std::cout << std::endl;
  if (moves.empty()) { //if we stall out then we check if we're a terminal node and otherwise stall.
    int eval = board.isWinning();
    if (eval > 0) return MAX_VALUE;
    if (eval < 0) return -MAX_VALUE;
    return 0;
  }
  std::pair<int, Board> primary_variation = moves[0];
  int best_value = -MAX_VALUE - 1;
  for (std::pair<int, Board> move_pair : moves) {
    int score = -std::get<Board>(move_pair).evaluate();
    if (score > best_value) {
      best_value = score;
      primary_variation = move_pair;
    }
  }
  int score = -serialJamboree(std::get<Board>(primary_variation), depth - 1, -b, -a);
  a = std::max(a, score);
  for (std::pair<int, Board> move_pair : moves) {
    if (a >= b) return score;
    if (move_pair == primary_variation) continue;
    score = std::max(score, -serialJamboree(std::get<Board>(move_pair), depth - 1, -a - 1, -a));
    if ((score > a) && (score < b)) {
      score = std::max(score, -serialJamboree(std::get<Board>(move_pair), depth - 1, -b, -score));
    }
    a = std::max(a, score);
  }
  return score;
}


int jamboree(Board board, int depth, int alpha, int beta, int serial_watcher) {
  if (depth <= 0) return board.evaluate();
  if (serial_watcher <= 0) return serialJamboree(board, depth, alpha, beta);
  std::vector<std::pair<int, Board>> moves = GenerateMoveBoards(board);
  if (moves.empty()) {
    int eval = board.isWinning();
    if (eval > 0) return MAX_VALUE;
    if (eval < 0) return -MAX_VALUE;
    return 0;
  }
  int size = moves.size();
  int pv_index = 0;
  std::pair<int, Board> primary_variation = moves[0];
  int best_value = -std::get<Board>(primary_variation).evaluate();
  for (int i = 1; i < size; i++) {
    int score = -std::get<Board>(moves[i]).evaluate();
    if (score > best_value) {
      best_value = score;
      pv_index = i;
    }
  }
  std::swap(moves[moves.size() - 1], moves[pv_index]);
  moves.pop_back();
  // Evaluate the primary variation, younger brothers wait.
  int value;
  value = -jamboree(primary_variation.second, depth - 1, -beta, -alpha, serial_watcher - 1);
  if (value >= beta) { // Beta cutoff
    return value;
  }
  if (value > alpha) {
    alpha = value;
  }
  // Scout the remaining variations
  int scouts[moves.size()];
  int cutoff = -1;
  cilk_for (int i = 0; i < moves.size(); i++) {
    int s = -jamboree(moves[i].second, depth - 1, -alpha - 1, -alpha, serial_watcher - 1);
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
      value = std::max(value, -jamboree(move.second, depth - 1, -beta, -alpha, serial_watcher - 1));
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
  int spawn_lvls = (32-__builtin_clz(__cilkrts_get_nworkers()))/2;
  cilk::reducer< cilk::op_max_index<int, int> > best_move; //soooo this is being misused a bit, but basically we do a max reducer in order to find the best move.  This is supposed to be for an index, but no reason it can't store the move instead
  cilk_for (int i = 0; i < moves.size(); i++) {
    std::pair<int, Board> move_pair = moves[i];
    int score = -jamboree(std::get<Board>(move_pair), depth - 1, -MAX_VALUE - 1, MAX_VALUE + 1, spawn_lvls);
    best_move->calc_max(std::get<int>(move_pair), score);
  }
  DEBUG(printf("%d nodes searched.  Failed %d scouts out of %d scouts\n", nodes, failed_scouts, scouts);)
  DEBUG(std::cout << std::flush;)
  return best_move->get_index_reference();
}
