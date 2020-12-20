#include "parallel_pvs.hpp"
#include <utility>
#include <cilk/cilk.h>
#include <cilk/reducer_max.h>

ParallelPVS::ParallelPVS() {
}

int runParallelPVS(Board board, int depth, int a, int b DEBUG(, int* failed_scouts, int* scouts, int* nodes)) {
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
	int score = -runParallelPVS(std::get<Board>(primary_variation), depth - 1, -b, -a DEBUG(, failed_scouts, scouts, nodes));
	a = std::max(a, score);
	for (std::pair<int, Board> move_pair : moves) {
		if (a >= b) return score;
		if (move_pair == primary_variation) continue;
		DEBUG((*scouts)++;)
		score = std::max(score, -runParallelPVS(std::get<Board>(move_pair), depth - 1, -a - 1, -a DEBUG(, failed_scouts, scouts, nodes)));
		if ((score > a) && (score < b)) {
			DEBUG((*failed_scouts)++;)
			score = std::max(score, -runParallelPVS(std::get<Board>(move_pair), depth - 1, -b, -score DEBUG(, failed_scouts, scouts, nodes)));
		}
		a = std::max(a, score);
	}
	return score;
}

int ParallelPVS::findMove(Board board, int depth) {
	DEBUG(int failed_scouts = 0;)
	DEBUG(int scouts = 0;)
	DEBUG(int nodes = 0;)
	std::vector<std::pair<int, Board>> moves = GenerateMoveBoards(board);
	cilk::reducer< cilk::op_max_index<int, int> > best_move; //soooo this is being misused a bit, but basically we do a max reducer in order to find the best move.  This is supposed to be for an index, but no reason it can't store the move instead
	cilk_for (int i = 0; i < moves.size(); i++) {
		std::pair<int, Board> move_pair = moves[i];
		int score = -runParallelPVS(std::get<Board>(move_pair), depth - 1, -MAX_VALUE - 1, MAX_VALUE + 1 DEBUG(, &failed_scouts, &scouts, &nodes));
		best_move->calc_max(std::get<int>(move_pair), score);
	}
	DEBUG(printf("%d nodes searched.  Failed %d scouts out of %d scouts\n", nodes, failed_scouts, scouts);)
	DEBUG(std::cout << std::flush;)
	return best_move->get_index_reference();
}

std::string ParallelPVS::getName() {
	return "ParallelPVS";
}

std::string ParallelPVS::getDescription() {
	return "A basic parallel version of a PVS search";
}