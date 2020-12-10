#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <sstream>
#include <ctype.h>
#include <string.h>

typedef long long int U64;

#define WHITE_TURN 0
#define BLACK_TURN 1

#define GETBIT(bb, x) ((bb >> x) & 0x1)

class Board {
public:
  Board();
  Board(U64 white, U64 black, int turn);
  Board(std::string fen);
  std::string GetFen();
  Board PlaceDisk(U64 loc);
  U64 GetOccupied();
  U64 GetVacant();
  U64 white, black;
  int turn;
};

static inline bool operator==(const Board &b1, const Board &b2) {
  return b1.white == b2.white and b1.black == b2.black and b1.turn == b2.turn;
}

static inline bool operator!=(const Board &b1, const Board &b2) {
  return not (b1 == b2);
}


#endif /* BOARD_H */
