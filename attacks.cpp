#include "attacks.hpp"

enum Dir{Nort, NoEa, East, SoEa, Sout, SoWe, West, NoWe};

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

/**
 * bitScanReverse
 * @authors Kim Walisch, Mark Dickinson
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of most significant one bit
 */
int bitScanReverse(U64 bb) {
   const U64 debruijn64 = C64(0x03f79d71b4cb0a89);
   assert (bb != 0);
   bb |= bb >> 1; 
   bb |= bb >> 2;
   bb |= bb >> 4;
   bb |= bb >> 8;
   bb |= bb >> 16;
   bb |= bb >> 32;
   return index64[(bb * debruijn64) >> 58];
}

void InitRayAttacks() {
  // Positive attacks
  // NorthWest
  // North
  U64 nort = C64(0x0101010101010100);
  for (int sq=0; sq < 64; sq++, nort <<= 1)
    rayAttacks[sq][Nort] = nort;
  // NorthEast
  U64 noea = C64(0x8040201008040200);
  for (int f=0; f < 8; f++, noea = eastOne(noea)) {
    U64 ne = noea;
    for (int r8 = 0; r8 < 8*8; r8 += 8, ne <<= 8)
      rayAttacks[r8+f][NoEa] = ne;
  }
  // East
  // Negative attacks
}
