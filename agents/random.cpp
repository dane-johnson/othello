#include "random.hpp"
#include <iostream>
#include <algorithm>

RandomAgent::RandomAgent() {
}

int RandomAgent::findMove(Board board, int depth) {
	std::vector<int> moves = GenerateMoves(board);
	random_shuffle(moves.begin(), moves.end());
	return *moves.begin();
}

std::string RandomAgent::getName() {
	return "Random";
}

std::string RandomAgent::getDescription() {
	return "This just makes a random move from the available options.  Mostly for testing purposes.";
}