#include "human.hpp"
#include <iostream>

Human::Human() {
}

int Human::findMove(Board board, int depth) {
	std::cout << std::endl;
	std::cout << board.toOutputString() << std::endl;
	std::cout << "Legal moves: ";
	for (int move : GenerateMoves(board)) std::cout << move << ' ';
	std::cout << std::endl;
	bool legal = false;
	while (true) {
		int space;
		std::cin >> space;
		for (int move : GenerateMoves(board)) {
			if (space == move) {
				legal = true;
				break;
			}
		}
		if (legal) {
			return space;
		} else {
			std::cout << "Illegal move!" << std::endl;
		}
	}
}

std::string Human::getName() {
	return "Human";
}

std::string Human::getDescription() {
	return "This lets a human player play through the command line.";
}