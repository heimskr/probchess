#include "Match.h"
#include "RandomPlayer.h"

std::list<Move> AIPlayer::getPossibleMoves(const Match &match, const std::set<int> &columns) const {
	std::list<Move> possibilities;
	for (const int column: columns)
		for (int row = 0; row < match.board.height; ++row) {
			std::shared_ptr<Piece> piece = match.board.at(row, column);
			if (!piece || piece->color != color)
				continue;
			for (const Square &square: piece->canMoveTo())
				possibilities.push_back({piece->square, square});
		}
	return possibilities;
}

void AIPlayer::send(const std::string &) {}
