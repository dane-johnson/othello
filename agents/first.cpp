#include "first.hpp"
#include <algorithm>

FirstAgent::FirstAgent() {
}

int FirstAgent::findMove(Board board, int depth) {
  std::vector<int> moves = GenerateMoves(board);
  return *std::min_element(moves.begin(), moves.end());
}

std::string FirstAgent::getName() {
  return "FirstAgent";
}

std::string FirstAgent::getDescription() {
  return "Agent that always takes first available move.";
}
