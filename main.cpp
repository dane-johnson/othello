#include <string>
#include <iostream>

#include "board.hpp"
#include "attacks.hpp"

int main() {
  std::string standard_game("8/8/8/3BW3/3WB3/8/8/8 B");
  Board board1; // Standard board
  Board board2(standard_game); // From the fen of a standard game

  if (board1 == board2) {
    std::cout << "Same board!" << std::endl;
  } else {
    std::cout << "Wrong board!" << std::endl;
  }

  std::cout << board1.GetFen() << std::endl;

  InitRayAttacks();
  U64 attacks_from_d5 = C64(0x925438ef38549211);
  U64 attacks_in_table = 0x0;
  attacks_in_table |= rayAttacks[36][Nort];
  attacks_in_table |= rayAttacks[36][NoEa];
  attacks_in_table |= rayAttacks[36][East];
  attacks_in_table |= rayAttacks[36][SoEa];
  attacks_in_table |= rayAttacks[36][Sout];
  attacks_in_table |= rayAttacks[36][SoWe];
  attacks_in_table |= rayAttacks[36][West];
  attacks_in_table |= rayAttacks[36][NoWe];
  if (attacks_from_d5 == attacks_in_table) {
    std::cout << "Attacks are correct!" << std::endl;
  } else {
    std::cout << "Attacks are wrong!" << std::endl;
    Board good(attacks_from_d5, 0x0, 0);
    Board bad(attacks_in_table, 0x0, 0);
    std::cout << good.GetFen() << std::endl;
    std::cout << bad.GetFen() << std::endl;
  }
}
