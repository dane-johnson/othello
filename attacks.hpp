#ifndef ATTACKS_H
#define ATTACKS_H

#include <string.h>

#include "board.hpp"

extern U64 rayAttacks[64][8];

void InitRayAttacks();

int BitscanForward(U64 bb);
int BitscanReverse(U64 bb);

inline U64 eastOne(U64 bb) {
  return (bb << 1) & ~A_FILE;
}

inline U64 westOne(U64 bb) {
  return (bb >> 1) & ~H_FILE;
}

enum Dir{Nort, NoEa, East, SoEa, Sout, SoWe, West, NoWe};


#endif /* ATTACKS_H */
