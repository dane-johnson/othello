#include "move_generation.hpp"
#include <iostream>

int countMoves(Board curr) {
  if (curr.turn == WHITE_TURN) {
    return __builtin_popcountl(valid_attacks(curr.white, curr.black, ~(curr.white | curr.black)));
  } else {
    return __builtin_popcountl(valid_attacks(curr.black, curr.white, ~(curr.white | curr.black)));
  }
}

int countOpponentMoves(Board curr) {
  if (curr.turn == BLACK_TURN) {
    return __builtin_popcountl(valid_attacks(curr.white, curr.black, ~(curr.white | curr.black)));
  } else {
    return __builtin_popcountl(valid_attacks(curr.black, curr.white, ~(curr.white | curr.black)));
  }
}

std::vector<int> GenerateMoves(Board curr) {
  // This is super dumb
  U64 legal_spots;
  std::vector<int> moves;
  if (curr.turn == WHITE_TURN) {
    legal_spots = valid_attacks(curr.white, curr.black, ~(curr.white | curr.black));
    if (!legal_spots) {
      if (valid_attacks(curr.black, curr.white, ~(curr.white | curr.black))) moves.push_back(-1);  //if our opponent has moves give us a skip move, otherwise return empty to end game
      return moves;
    }
  } else {
    legal_spots = valid_attacks(curr.black, curr.white, ~(curr.white | curr.black));
    if (!legal_spots) {
      if (valid_attacks(curr.white, curr.black, ~(curr.white | curr.black))) moves.push_back(-1);  //if our opponent has moves give us a skip move, otherwise return empty to end game
      return moves;
    }
  }
  for (int sp = 0; sp < 64; sp++) {
    if (GETBIT(legal_spots, sp)) moves.push_back(sp);
  }
  return moves;
}

std::vector<std::pair<int, Board>> GenerateMoveBoards(Board curr) {
  // This is super dumb
  U64 legal_spots;
  std::vector<std::pair<int, Board>> moves;
  if (curr.turn == WHITE_TURN) {
    legal_spots = valid_attacks(curr.white, curr.black, ~(curr.white | curr.black));
    if (!legal_spots) {
      if (valid_attacks(curr.black, curr.white, ~(curr.white | curr.black))) moves.push_back(std::pair<int, Board>(-1, MakeMove(curr, -1))); //if our opponent has moves give us a skip move, otherwise return empty to end game
      return moves;
    }
  } else {
    legal_spots = valid_attacks(curr.black, curr.white, ~(curr.white | curr.black));
    if (!legal_spots) {
      if (valid_attacks(curr.white, curr.black, ~(curr.white | curr.black))) moves.push_back(std::pair<int, Board>(-1, MakeMove(curr, -1))); //if our opponent has moves give us a skip move, otherwise return empty to end game
      return moves;
    }
  }
  for (int sp = 0; sp < 64; sp++) {
    if (GETBIT(legal_spots, sp)) moves.push_back(std::pair<int, Board>(sp, MakeMove(curr, sp)));
  }
  return moves;
}
