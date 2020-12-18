#include "alphabeta.hpp"

AlphaBeta::AlphaBeta() {
}

int alphabeta(Board board, int depth, int alpha, int beta, bool maximizing) {
  int value;
  if (depth == 0) {
    return board.evaluate();
  }
  std::vector<int> moves = GenerateMoves(board);
  if (moves.empty()) {
    return board.evaluate();
  }
  if (maximizing) {
    value = -MAX_VALUE;
    for (int move : moves) {
      value = std::max(value, alphabeta(MakeMove(board, move), depth -1, alpha, beta, false));
      alpha = std::max(value, alpha);
      if (alpha >= beta) {
	// Beta Cutoff
	break;
      }
    }
  } else {
    value = MAX_VALUE;
    for (int move : moves) {
      value = std::min(value, alphabeta(MakeMove(board, move), depth -1, alpha, beta, true));
      beta = std::min(value, beta);
      if (beta <= alpha) {
	// Alpha cutoff
	break;
      }
    }
  }
  return value;
}

int AlphaBeta::findMove(Board board, int depth) {
  int best_value = -MAX_VALUE - 1;
  int best_move = -1;
  std::vector<int> moves = GenerateMoves(board);
  for (int move : moves) {
    int value = alphabeta(MakeMove(board, move), depth - 1, -MAX_VALUE, MAX_VALUE, true);
    if (value > best_value) {
      best_value = value;
      best_move = move;
    }
  }
  return best_move;
}

std::string AlphaBeta::getName() {
  return "AlphaBeta";
}

std::string AlphaBeta::getDescription() {
  return "Basic serial alpha-beta search.";
}
