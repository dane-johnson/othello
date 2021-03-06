#include "attacks.hpp"
#include <iostream>

void printBitboard(U64 board) {
  for(int i = 0; i < 64; i++) {
    std::cout << GETBIT(board, i);
    if ((i%8) == 7) std::cout << std::endl;
  }
}

U64 rayAttacks[64][8];

const int index64[64] = {
  0,  1, 48,  2, 57, 49, 28,  3,
  61, 58, 50, 42, 38, 29, 17,  4,
  62, 55, 59, 36, 53, 51, 43, 22,
  45, 39, 33, 30, 24, 18, 12,  5,
  63, 47, 56, 27, 60, 41, 37, 16,
  54, 35, 52, 21, 44, 32, 23, 11,
  46, 26, 40, 15, 34, 20, 31, 10,
  25, 14, 19,  9, 13,  8,  7,  6
};

/**
 * bitScanForward
 * @author Martin Läuter (1997)
 *         Charles E. Leiserson
 *         Harald Prokop
 *         Keith H. Randall
 * "Using de Bruijn Sequences to Index a 1 in a Computer Word"
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */

int BitscanForward(U64 bb) {
  const U64 debruijn64 = C64(0x03f79d71b4cb0a89);
  return index64[((bb & -bb) * debruijn64) >> 58];
}

const int index64r[64] = {
  0, 47,  1, 56, 48, 27,  2, 60,
  57, 49, 41, 37, 28, 16,  3, 61,
  54, 58, 35, 52, 50, 42, 21, 44,
  38, 32, 29, 23, 17, 11,  4, 62,
  46, 55, 26, 59, 40, 36, 15, 53,
  34, 51, 20, 43, 31, 22, 10, 45,
  25, 39, 14, 33, 19, 30,  9, 24,
  13, 18,  8, 12,  7,  6,  5, 63
};

/**
 * bitScanReverse
 * @authors Kim Walisch, Mark Dickinson
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of most significant one bit
 */
int BitscanReverse(U64 bb) {
  const U64 debruijn64 = C64(0x03f79d71b4cb0a89);
  bb |= bb >> 1;
  bb |= bb >> 2;
  bb |= bb >> 4;
  bb |= bb >> 8;
  bb |= bb >> 16;
  bb |= bb >> 32;
  return index64r[(bb * debruijn64) >> 58];
}

void InitRayAttacks() {
  // Positive attacks
  // NorthWest
  U64 nowe = C64(0x0102040810204000);
  for (int f = 7; f >= 0; f--, nowe = westOne(nowe)) {
    U64 nw = nowe;
    for (int r8 = 0; r8 < 8 * 8; r8 += 8, nw <<= 8) {
      rayAttacks[r8 + f][NoWe] = nw;
    }
  }
  // North
  U64 nort = C64(0x0101010101010100);
  for (int sq = 0; sq < 64; sq++, nort <<= 1)
    rayAttacks[sq][Nort] = nort;
  // NorthEast
  U64 noea = C64(0x8040201008040200);
  for (int f = 0; f < 8; f++, noea = eastOne(noea)) {
    U64 ne = noea;
    for (int r8 = 0; r8 < 8 * 8; r8 += 8, ne <<= 8)
      rayAttacks[r8 + f][NoEa] = ne;
  }
  // East
  U64 east = C64(0x00000000000000FE);
  for (int f = 0; f < 8; f++, east = eastOne(east)) {
    U64 e = east;
    for (int r = 0; r < 8; r++) {
      rayAttacks[r * 8 + f][East] = e;
      e <<= 8;
    }
  }
  // Negative attacks
  // SouthEast
  U64 soea = C64(0x0002040810204080);
  for (int f = 0; f < 8; f++, soea = eastOne(soea)) {
    U64 se = soea;
    for (int r = 7; r >= 0; r--) {
      rayAttacks[r * 8 + f][SoEa] = se;
      se >>= 8;
    }
  }
  // South
  U64 sout = C64(0x0080808080808080);
  for (int sq = 63; sq >= 0; sq--, sout >>= 1) {
    rayAttacks[sq][Sout] = sout;
  }
  // SouthWest
  U64 sowe = C64(0x0040201008040201);
  for (int f = 7; f >= 0; f--, sowe = westOne(sowe)) {
    U64 sw = sowe;
    for (int r = 7; r >= 0; r--) {
      rayAttacks[r * 8 + f][SoWe] = sw;
      sw >>= 8;
    }
  }
  // West
  U64 west = C64(0x000000000000007f);
  for (int f = 7; f >= 0; f--, west = westOne(west)) {
    U64 w = west;
    for (int r = 0; r < 8; r++) {
      rayAttacks[r * 8 + f][West] = w;
      w <<= 8;
    }
  }
}

U64 inline valid_neg_attack(U64 friendly, U64 opposed, U64 empty, U64 direction, U64 eliminate)
{
  U64 flood = friendly;  //start with our own pieces
  flood |= ((flood << direction) & ~eliminate & opposed);
  flood |= ((flood << direction) & ~eliminate & opposed);
  flood |= ((flood << direction) & ~eliminate & opposed);
  flood |= ((flood << direction) & ~eliminate & opposed);
  flood |= ((flood << direction) & ~eliminate & opposed);
  flood |= ((flood << direction) & ~eliminate & opposed); //travel over up to 6 enemy pieces
  flood ^= friendly; //remove out own pieces from the flood since they need to pass at least 1 enemy piece
  flood = ((flood << direction) & ~eliminate & empty); //only look at the values that result in an empty square.
  return flood;
}

U64 inline valid_pos_attack(U64 friendly, U64 opposed, U64 empty, U64 direction, U64 eliminate)
{
  U64 flood = friendly;  //start with our own pieces
  flood |= ((flood >> direction) & ~eliminate & opposed);
  flood |= ((flood >> direction) & ~eliminate & opposed);
  flood |= ((flood >> direction) & ~eliminate & opposed);
  flood |= ((flood >> direction) & ~eliminate & opposed);
  flood |= ((flood >> direction) & ~eliminate & opposed);
  flood |= ((flood >> direction) & ~eliminate & opposed); //travel over up to 6 enemy pieces
  flood ^= friendly; //remove out own pieces from the flood since they need to pass at least 1 enemy piece
  flood = ((flood >> direction) & ~eliminate & empty); //only look at the values that result in an empty square.
  return flood;
}

U64 valid_attacks(U64 friendly, U64 opposed, U64 empty) {
  // std::cout << "Friendly" << std::endl;
  // printBitboard(friendly);
  // std::cout << "Opposed" << std::endl;
  // printBitboard(opposed);
  // std::cout << "Empty" << std::endl;
  // printBitboard(empty);
  U64 attack = C64(0x0);
  attack |= valid_pos_attack(friendly, opposed, empty, _NORTH, 0);
  attack |= valid_pos_attack(friendly, opposed, empty, _NORTHEAST, H_FILE);
  attack |= valid_pos_attack(friendly, opposed, empty, _NORTHWEST, A_FILE);
  attack |= valid_pos_attack(friendly, opposed, empty, _EAST, H_FILE);
  attack |= valid_neg_attack(friendly, opposed, empty, _SOUTH, 0);
  attack |= valid_neg_attack(friendly, opposed, empty, _SOUTHWEST, A_FILE);
  attack |= valid_neg_attack(friendly, opposed, empty, _SOUTHEAST, H_FILE);
  attack |= valid_neg_attack(friendly, opposed, empty, _WEST, A_FILE);
  // std::cout << "Attacks" << std::endl;
  // printBitboard(attack);
  return attack;
}

U64 attack(int space, Dir dir, U64 friendly, U64 opposed) {
  U64 blocker, flipped;
  bool positive;
  switch (dir) {
  case NoWe:
  case Nort:
  case NoEa:
  case East:
    positive = true;
    break;
  case SoEa:
  case Sout:
  case SoWe:
  case West:
    positive = false;
    break;
  }
  U64 ray = rayAttacks[space][dir];
  if ((ray & friendly) == 0) {
    return C64(0x0);
  }
  if (positive) {
    blocker = BitscanForward(ray & friendly);
  } else {
    blocker = BitscanReverse(ray & friendly);
  }
  flipped = ray & ~(rayAttacks[blocker][dir] | C64(1) << blocker);
  if ((flipped & opposed) == flipped) {
    return flipped;
  } else {
    return 0x0;
  }
}

Board MakeMove(Board curr, int space) {
  Board next;
  U64 friendly;
  U64 opposed;

  if (space == -1) {
    next.white = curr.white;
    next.black = curr.black;
    next.turn = (curr.turn == WHITE_TURN ? BLACK_TURN : WHITE_TURN);
    return next;
  }

  if (curr.turn == WHITE_TURN) {
    friendly = curr.white;
    opposed = curr.black;
    next.turn = BLACK_TURN;
  } else {
    friendly = curr.black;
    opposed = curr.white;
    next.turn = WHITE_TURN;
  }

  // Do all attacks
  U64 flipped = 0x0;
  flipped |= attack(space, Nort, friendly, opposed);
  flipped |= attack(space, NoEa, friendly, opposed);
  flipped |= attack(space, East, friendly, opposed);
  flipped |= attack(space, SoEa, friendly, opposed);
  flipped |= attack(space, Sout, friendly, opposed);
  flipped |= attack(space, SoWe, friendly, opposed);
  flipped |= attack(space, West, friendly, opposed);
  flipped |= attack(space, NoWe, friendly, opposed);

  opposed &= ~(flipped);
  friendly |= flipped | C64(1) << space;

  if (curr.turn == WHITE_TURN) {
    next.white = friendly;
    next.black = opposed;
  } else {
    next.black = friendly;
    next.white = opposed;
  }

  return next;
}
