#include <string>
#include <iostream>

#include "board.hpp"

int main() {
  std::string standard_game("8/8/8/3WB3/3BW3/8/8/8 W");
  Board board1; // Standard board
  Board board2(standard_game); // From the fen of a standard game

  if (board1 == board2) {
    std::cout << "Same board!" << std::endl;
  } else {
    std::cout << "Wrong board!" << std::endl;
  }

  std::cout << board1.GetFen() << std::endl;
}
