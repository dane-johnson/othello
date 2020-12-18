#include "serial_pvs.hpp"

SerialPVS::SerialPVS() {
}

int runPVS(Board board, int depth, int a, int b, bool stalled DEBUG(, int* nodes)) {
	DEBUG(printf("\r%d", (*nodes)++);)
	DEBUG(std::cout << std::flush;)
	if (depth == 0) return board.evaluate();  //if we've hit bottom depth
	std::vector<int> moves = GenerateMoves(board);  //Not really PVS yet because we should be sorting the move list
	if (moves.empty()) { //if we stall out then we check if we're a terminal node and otherwise stall.
		if (stalled) {
			int eval = board.evaluate();
			if (eval > 0) return MAX_VALUE;
			if (eval < 0) return -MAX_VALUE;
			return 0;
		}
		return -runPVS(MakeMove(board, -1), depth-1, -b, -a, true DEBUG(, nodes));
	}
	std::vector<int>::iterator it = moves.begin();
	int score = -runPVS(MakeMove(board, *it), depth-1, -b, -a, false DEBUG(, nodes));
	it++;
	a = (a < score ? score : a);
	for (; it != moves.end(); it++) {
		if (a > b) return a;
		score = -runPVS(MakeMove(board, *it), depth-1, -a - 1, -a, false DEBUG(, nodes));
		if (score > a && score < b) {
			score = -runPVS(MakeMove(board, *it), depth-1, -b, -a, false DEBUG(, nodes));
		}
	}
	return a;


}

int SerialPVS::findMove(Board board, int depth) {
	int best_move = -1;
	int best_value = -MAX_VALUE - 1;
	DEBUG(int checked_nodes = 0;)
	std::vector<int> moves = GenerateMoves(board);
	for (int move : moves) {
		int score = -runPVS(MakeMove(board, move), depth-1, best_value, MAX_VALUE + 1, false DEBUG(, &checked_nodes));
		if (score > best_value) {
			best_value = score;
			best_move = move;
		}
	}
	return best_move;
}

std::string SerialPVS::getName() {
	return "SerialPVS";
}

std::string SerialPVS::getDescription() {
	return "A serial version of a basic PVS search";
}