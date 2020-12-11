#ifndef ATTACKS_H
#define ATTACKS_H

#include <string.h>

#include "board.hpp"

U64 rayAttacks[64][8];

void InitRayAttacks();

int BitscanForward(U64 bb);
int BitscanReverse(U64 bb);


#endif /* ATTACKS_H */
