#include <string>
#include <iostream>

#include <string.h>

#include "board.hpp"
#include "attacks.hpp"

void test() {
  // Test board setup and FEN
  std::string standard_game("8/8/8/3BW3/3WB3/8/8/8 B");
  Board board1; // Standard board
  Board board2(standard_game); // From the fen of a standard game

  if (board1 == board2) {
    std::cout << "Same board!" << std::endl;
  } else {
    std::cout << "Wrong board!" << std::endl;
  }

  std::cout << board1.GetFen() << std::endl;

  // Test ray attacks
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

  // Test move resolution
  std::string pre_move("8/8/2W1B1B1/3BBB2/3WBB2/5W2/8/8 W");
  std::string post_move("8/8/2W1BWB1/3BWW2/3WBW2/5W2/8/8 B");
  board1 = Board(pre_move);
  board2 = Board(post_move);
  // place f2
  if (MakeMove(board1, 21) == board2) {
    std::cout << "Move resolution correct!" << std::endl;
  } else {
    std::cout << "Move resolution not correct!" << std::endl;
    std::cout << MakeMove(board1, 21).GetFen() << std::endl;
  }
}

int main(int argc, char* argv[]) {
  InitRayAttacks();
  if (argc == 2 && strcmp(argv[1], "-t") == 0) {
    test();
    return 0;
  }
  // Play a game
  Board board; // Standard board
  while(true) {
    std::cout << board.GetFen() << std::endl;
    int space;
    std::cin >> space;
    board = MakeMove(board, space);
  }
}
