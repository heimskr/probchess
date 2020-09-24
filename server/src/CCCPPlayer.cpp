// The bulk of the algorithm is based on code by Tom Murphy VII, aka suckerpinch.
// https://sourceforge.net/p/tom7misc/svn/HEAD/tree/trunk/chess/

#include "CCCPPlayer.h"
#include "Match.h"

bool CCCPPlayer::checkSuperior(const Move &check, const Move &alternative) const {

	return true;
}

int CCCPPlayer::centerDistance(int column) const {
	switch (column) {
		case 3:
		case 4:
			return 0;
		case 2:
		case 5:
			return 1;
		case 1:
		case 6:
			return 2;
		default:
			return 3;
	}
}

int CCCPPlayer::moveCode(const Move &move) const {
	const int source      = move.from.row * 8 + move.from.column;
	const int destination = move.to.row   * 8 + move.to.column;
	return (source * 64 + destination) * 8;
}

Move CCCPPlayer::chooseMove(const Match &match, const std::set<int> &columns) {
	const bool isBlack = match.currentTurn == Color::Black;
	std::vector<LabeledMove> labeled_moves;

	std::list<Move> legal = match.board.allMoves(match.currentTurn);
	legal.remove_if([&](const Move &move) {
		return columns.count(move.from.column) == 0;
	});

	for (const Move &move: legal) {
		LabeledMove labeled_move;
	}



	return {{0, 0}, {0, 0}};
}
