#ifndef ATTACKS_H
#define ATTACKS_H

#include <string.h>

#include "board.hpp"

extern U64 rayAttacks[64][8];

void InitRayAttacks();
U64 valid_attacks(U64 friendly, U64 opposed, U64 empty);

int BitscanForward(U64 bb);
int BitscanReverse(U64 bb);

inline U64 eastOne(U64 bb) {
  return (bb << 1) & ~A_FILE;
}

inline U64 westOne(U64 bb) {
  return (bb >> 1) & ~H_FILE;
}

enum Dir{Nort, NoEa, East, SoEa, Sout, SoWe, West, NoWe};

#define _NORTH 8
#define _NORTHEAST 9
#define _NORTHWEST 7
#define _EAST 1

#define _SOUTH 8
#define _SOUTHWEST 9
#define _SOUTHEAST 7
#define _WEST 1


Board MakeMove(Board curr, int space);

#endif /* ATTACKS_H */
