#include "move_generation.hpp"

U64 GenerateMoves(Board board) {
  U64 self, oppn;
  if (board.turn == WHITE_TURN) {
    self = board.white;
    oppn = board.black;
  } else {
    self = board.black;
    oppn = board.white;
  }

  return 0x0;
}
