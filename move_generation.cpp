#include "move_generation.hpp"

std::vector<int> GenerateMoves(Board curr) {
  // This is super dumb
  std::vector<int> moves;
  for(int sp = 0; sp < 63; sp++) {
    Board next = MakeMove(curr, sp);
    if (next.white != curr.white and next.black != curr.black) {
      // Both sides changed, a legal move
      moves.push_back(sp);
    }
  }
  return moves;
}
