#include "serial_pvs.hpp"
#include <utility>

SerialPVS::SerialPVS() {
}

int runPVS(Board board, int depth, int a, int b DEBUG(, int* failed_scouts, int* scouts, int* nodes)) {
	DEBUG((*nodes)++;)
	if (depth <= 0) return board.evaluate();  //if we've hit bottom depth
	std::vector<std::pair<int, Board>> moves = GenerateMoveBoards(board);
	//for (std::pair<int, Board> move_pair : moves) { std::cout << std::get<int>(move_pair) << ", ";} std::cout << std::endl;
	if (moves.empty()) { //if we stall out then we check if we're a terminal node and otherwise stall.
		int eval = board.isWinning();
		if (eval > 0) return MAX_VALUE;
		if (eval < 0) return -MAX_VALUE;
		return 0;
	}
	std::pair<int, Board> primary_variation = moves[0];
	int best_value = -MAX_VALUE - 1;
	for (std::pair<int, Board> move_pair : moves) {
		int score = -std::get<Board>(move_pair).evaluate();
		if (score > best_value) {
			best_value = score;
			primary_variation = move_pair;
		}
	}
	int score = -runPVS(std::get<Board>(primary_variation), depth - 1, -b, -a DEBUG(, failed_scouts, scouts, nodes));
	a = std::max(a, score);
	for (std::pair<int, Board> move_pair : moves) {
		if (a >= b) return score;
		if (move_pair == primary_variation) continue;
		DEBUG((*scouts)++;)
		score = std::max(score, -runPVS(std::get<Board>(move_pair), depth - 1, -a - 1, -a DEBUG(, failed_scouts, scouts, nodes)));
		if ((score > a) && (score < b)) {
			DEBUG((*failed_scouts)++;)
			score = std::max(score, -runPVS(std::get<Board>(move_pair), depth - 1, -b, -score DEBUG(, failed_scouts, scouts, nodes)));
		}
		a = std::max(a, score);
	}
	return score;
}

int SerialPVS::findMove(Board board, int depth) {
	int best_move = -1;
	int best_value = -MAX_VALUE - 1;
	DEBUG(int failed_scouts = 0;)
	DEBUG(int scouts = 0;)
	DEBUG(int nodes = 0;)
	std::vector<std::pair<int, Board>> moves = GenerateMoveBoards(board);
	for (std::pair<int, Board> move_pair : moves) {
		int score = -runPVS(std::get<Board>(move_pair), depth - 1, best_value, MAX_VALUE + 1 DEBUG(, &failed_scouts, &scouts, &nodes));
		if (score > best_value) {
			best_value = score;
			best_move = std::get<int>(move_pair);
		}
	}
	DEBUG(printf("%d\n", nodes);)
	DEBUG(std::cout << std::flush;)
	return best_move;
}

std::string SerialPVS::getName() {
	return "SerialPVS";
}

std::string SerialPVS::getDescription() {
	return "A serial version of a basic PVS search";
}