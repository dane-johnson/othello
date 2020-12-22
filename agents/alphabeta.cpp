#include "alphabeta.hpp"

AlphaBeta::AlphaBeta() {
}

int alphabeta(Board board, int depth, int alpha, int beta DEBUG(, int* nodes)) {
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
    value = std::max(value, -alphabeta(std::get<Board>(move_pair), depth - 1, -beta, -alpha DEBUG(, nodes)));
    alpha = std::max(value, alpha);
    if (alpha >= beta) {
      // Beta Cutoff
      break;
    }
  }
  return value;
}

int AlphaBeta::findMove(Board board, int depth) {
  int best_value = -MAX_VALUE - 1;
  int best_move = -1;
  std::vector<std::pair<int, Board>> moves = GenerateMoveBoards(board);
  DEBUG(int nodes = 0;)
  for (std::pair<int, Board> move_pair : moves) {
    int value = -alphabeta(std::get<Board>(move_pair), depth - 1, best_value, MAX_VALUE DEBUG(, &nodes));
    if (value > best_value) {
      best_value = value;
      best_move = std::get<int>(move_pair);
    }
  }
  DEBUG(printf("%d\n", nodes);)
  DEBUG(std::cout << std::flush;)
  return best_move;
}

std::string AlphaBeta::getName() {
  return "AlphaBeta";
}

std::string AlphaBeta::getDescription() {
  return "Basic serial alpha-beta search.";
}
