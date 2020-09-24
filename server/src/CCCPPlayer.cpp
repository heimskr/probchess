// The bulk of the algorithm is based on code by Tom Murphy VII, aka suckerpinch.
// https://sourceforge.net/p/tom7misc/svn/HEAD/tree/trunk/chess/

#include "CCCPPlayer.h"

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

Move CCCPPlayer::chooseMove(const Board &board, const std::set<int> &columns) {
	std::list<Move> all_legal_moves;

	for (const int column: columns) {
		for (int row = 0; row < board.height; ++row) {
			std::shared_ptr<Piece> piece = board.at(row, column);
			if (piece && piece->color == color) {
				for (const Square &destination: piece->canMoveTo())
					all_legal_moves.push_back({piece->square, destination});
			}
		}
	}



	return {{0, 0}, {0, 0}};
}
