#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include <vector>
#include <utility>

#include "board.hpp"
#include "attacks.hpp"

std::vector<int> GenerateMoves(Board curr);
std::vector<std::pair<int, Board>> GenerateMoveBoards(Board curr);
int countMoves(Board curr);
#endif /* MOVE_GENERATION_H */
