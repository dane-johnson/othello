#include "parallel_alphabeta.hpp"
#include <cilk/cilk.h>
#include <cilk/reducer_max.h>

ParallelAlphaBeta::ParallelAlphaBeta() {
}

int parallelalphabeta(Board board, int depth, int alpha, int beta DEBUG(, int* nodes)) {
  DEBUG((*nodes)++;)
  int value;
  if (depth == 0) {
    return board.evaluate();
  }
  std::vector<std::pair<int, Board>> moves = GenerateMoveBoards(board);
  if (moves.empty()) {
    int eval = board.isWinning();
    if (eval > 0) return MAX_VALUE;
    if (eval < 0) return -MAX_VALUE;
    return 0;
  }
  value = -MAX_VALUE;
  for (std::pair<int, Board> move_pair : moves) {
    value = std::max(value, -parallelalphabeta(std::get<Board>(move_pair), depth - 1, -beta, -alpha DEBUG(, nodes)));
    alpha = std::max(value, alpha);
    if (alpha >= beta) {
      // Beta Cutoff
      break;
    }
  }
  return value;
}

int ParallelAlphaBeta::findMove(Board board, int depth) {
  std::vector<std::pair<int, Board>> moves = GenerateMoveBoards(board);
  DEBUG(int nodes = 0;)
  cilk::reducer< cilk::op_max_index<int, int> > best_move;
  cilk_for (int i = 0; i < moves.size(); i++) {
    std::pair<int, Board> move_pair = moves[i];
    int score = -parallelalphabeta(std::get<Board>(move_pair), depth - 1, -MAX_VALUE - 1, MAX_VALUE DEBUG(, &nodes));
    best_move->calc_max(std::get<int>(move_pair), score);
  }
  DEBUG(printf("%d nodes searched.\n", nodes);)
  DEBUG(std::cout << std::flush;)
  return best_move->get_index_reference();
}

std::string ParallelAlphaBeta::getName() {
  return "ParallelAlphaBeta";
}

std::string ParallelAlphaBeta::getDescription() {
  return "Basic parallel alpha-beta search.";
}
