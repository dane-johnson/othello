#include <string>
#include <iostream>

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "board.hpp"
#include "attacks.hpp"
#include "move_generation.hpp"
#include "agent.hpp"
#include <chrono>

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
  InitRayAttacks();
  U64 attacks_from_e5 = C64(0x925438ef38549211);
  U64 attacks_in_table = 0x0;
  attacks_in_table |= rayAttacks[36][Nort];
  attacks_in_table |= rayAttacks[36][NoEa];
  attacks_in_table |= rayAttacks[36][East];
  attacks_in_table |= rayAttacks[36][SoEa];
  attacks_in_table |= rayAttacks[36][Sout];
  attacks_in_table |= rayAttacks[36][SoWe];
  attacks_in_table |= rayAttacks[36][West];
  attacks_in_table |= rayAttacks[36][NoWe];
  if (attacks_from_e5 == attacks_in_table) {
    std::cout << "Attacks are correct!" << std::endl;
  } else {
    std::cout << "Attacks are wrong!" << std::endl;
    Board good(attacks_from_e5, 0x0, 0);
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

  // Test top corner
  board1 = Board("8/8/8/8/8/7W/7B/8 W");
  auto moves = GenerateMoves(board1);
  if (moves.size() == 1 and moves[0] == 63) {
    std::cout << "Top corner working" << std::endl;
  } else {
    std::cout << "Top corner not working" << std::endl;
  }
}

void printHelpMessage(char* progam_name) {
  std::cout << "Either you gave an invalid option or requested the help message.  This is an othello program made by Dane Johnson and Aaron Handleman.\n";
  std::cout << "Usage: " << progam_name << " [-t] [-h] [-w <agent_name>] [-b <agent_name>] [-d <depth=10>]\n";
  std::cout << "where -w sets the white agent and -b sets the black agent.  Both default to human.  Valid agents are as follows: \n";
  std::cout << getAgentTypesDescriptions() << std::endl;
}

int main(int argc, char* argv[]) {
  int opt;
  int depth = 10;
  bool intermediary_boards = false;
  Agent* whiteAgent = getAgentOfType("Human");
  Agent* blackAgent = getAgentOfType("Human");
  while ( (opt = getopt(argc, argv, "thw:b:d:i")) > 0) {
    switch (opt) {
    case 'i':
      intermediary_boards = true;
      break;
    case 't':
      test();
      return 0;
    case 'w':
      whiteAgent = getAgentOfType(optarg);
      break;
    case 'b':
      blackAgent = getAgentOfType(optarg);
      break;
    case 'd':
      depth = atoi(optarg);
      break;
    case 'h':
    default:
      printHelpMessage(argv[0]);
      return 0;
    }
  }
  // Play a game
  InitRayAttacks();
  Board board; // Standard board
  bool stall = false;
  auto init_time = std::chrono::high_resolution_clock::now();
  auto black_time_total = std::chrono::duration_cast<std::chrono::microseconds>(init_time - init_time);
  auto white_time_total = std::chrono::duration_cast<std::chrono::microseconds>(init_time - init_time);
  int white_moves = 0;
  int black_moves = 0;

  while (true) {
    if (intermediary_boards) std::cout << "intermediary_board: \n" << board.toOutputString() << std::endl;
    if (GenerateMoves(board).empty()) {  //if we can't find any legal moves, check if we're already stalled.  If so, game over.
      if (stall) {
        break;
      }
      stall = true; //Otherwise mark that we stalled one players turn and go to the next player's turn
      board = MakeMove(board, -1);
      continue;
    }
    if (board.turn == BLACK_TURN) {
      auto start = std::chrono::high_resolution_clock::now();
      int move = blackAgent->findMove(board, depth);
      auto end = std::chrono::high_resolution_clock::now();
      black_time_total += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
      black_moves++;
      board = MakeMove(board, move);
    } else {
      auto start = std::chrono::high_resolution_clock::now();
      int move = whiteAgent->findMove(board, depth);
      auto end = std::chrono::high_resolution_clock::now();
      white_time_total += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
      white_moves++;
      board = MakeMove(board, move);
    }
  }
  if (board.whitePieces() > board.blackPieces()) {
    //white wins
    std::cout << "White played by " << whiteAgent->getName() << " beat Black played by " << blackAgent->getName() << std::endl;
  } else if (board.whitePieces() < board.blackPieces()) {
    //black wins
    std::cout << "Black played by " << blackAgent->getName() << " beat White played by " << whiteAgent->getName() << std::endl;
  } else {
    //draw
    std::cout << "Black played by " << blackAgent->getName() << " drew White played by " << whiteAgent->getName() << std::endl;
  }
  std::cout << "Black played by " << blackAgent->getName() << " had an average move time of " << black_time_total.count() / black_moves << " microseconds over " << black_moves << " moves" << std::endl;
  std::cout << "White played by " << whiteAgent->getName() << " had an average move time of " << white_time_total.count() / white_moves << " microseconds over " << white_moves << " moves" << std::endl;
  std::cout << "Final board: \n" << board.toOutputString() << std::endl;
}
