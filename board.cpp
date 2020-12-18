#include "board.hpp"

Board::Board() {
  black = 0x0000001008000000;
  white = 0x0000000810000000;
  turn = BLACK_TURN;
}

Board::Board(U64 white, U64 black, int turn) {
  this->white = white;
  this->black = black;
  this->turn = turn;
}

// 8/8/8/3BW3/3WB3/8/8/8 B
Board::Board(std::string fen) {
  int i = 0;

  // Read board
  white = 0;
  black = 0;
  int position = 0;
  while (fen[i] != ' ') {
    int row = position / 8;
    int col = (position % 8);

    if (isdigit(fen[i])) {
      position += (fen[i] - '0');
    } else if (fen[i] == 'W') {
      white |= 1ULL << (row * 8 + col);
      position++;
    } else if (fen[i] == 'B') {
      black |= 1ULL << (row * 8 + col);
      position++;
    }
    i++;
  }

  // Read turn
  i++;
  if (fen[i] == 'W') {
    turn = WHITE_TURN;
  } else {
    turn = BLACK_TURN;
  }
}

std::string Board::GetFen() {
  std::stringstream ss;
  for (int i = 0; i < 8; i++) {
    int blanks = 0;
    for (int j = 0; j < 8; j++) {
      if (not GETBIT(white, (i * 8 + j)) and not GETBIT(black, (i * 8 + j))) {
        blanks++;
      } else {
        if (blanks > 0) {
          ss << (char)('0' + blanks);
          blanks = 0;
        }
        if (GETBIT(white, (i * 8 + j))) {
          ss << 'W';
        } else {
          ss << 'B';
        }
      }
    }
    if (blanks > 0) {
      ss << (char)('0' + blanks);
    }
    if (i < 7) {
      ss << '/';
    }
  }

  ss << ' ';
  if (turn == WHITE_TURN) {
    ss << 'W';
  } else {
    ss << 'B';
  }

  return ss.str();
}

U64 Board::GetOccupied() {
  return white | black;
}

U64 Board::GetVacant() {
  return ~(white | black);
}

std::string Board::toOutputString() {
  std::stringstream ss;
  for (int i = 0; i < 8; i++) {
    ss << '|';
    for (int j = 0; j < 8; j++) {
      if (not GETBIT(white, (i * 8 + j)) and not GETBIT(black, (i * 8 + j))) {
        ss << "\u001b[42;1m";
        if (i * 8 + j < 10) ss << '0';
        ss << i * 8 + j;
      } else {
        if (GETBIT(white, (i * 8 + j))) {
          ss << "\u001b[47;1m" << "WW";
        } else {
          ss << "\u001b[40;1m" << "BB";
        }
      }
      ss << "\u001b[0m";
      ss << '|';
    }
    if (i < 7) {
      ss << '\n';
    }
  }
  return ss.str();
}

int Board::evaluate() {
  if (this->turn == BLACK_TURN) {
    return (this->blackPieces() - this->whitePieces());
  } else {
    return (this->blackPieces() - this->whitePieces());
  }
}



int Board::whitePieces() {
  return __builtin_popcountl(this->white);
}


int Board::blackPieces() {
  return __builtin_popcountl(this->black);
}