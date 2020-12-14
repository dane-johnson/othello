#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include <vector>

#include "board.hpp"
#include "attacks.hpp"

std::vector<int> GenerateMoves(Board curr);

#endif /* MOVE_GENERATION_H */
