#include <cstdlib>
#include <iostream>

#include "RandomPlayer.h"
#include "Board.h"

void RandomPlayer::send(const std::string &) {}

Move RandomPlayer::chooseMove(const Board &board, const std::set<int> &columns) {
	std::list<Move> possibilities;
	for (const int column: columns) {
		for (int row = 0; row < board.height; ++row) {
			std::shared_ptr<Piece> piece = board.at(row, column);
			if (!piece || piece->color != color)
				continue;
			for (const Square &square: piece->canMoveTo())
				possibilities.push_back({piece->square, square});
		}
	}

	if (possibilities.empty())
		throw std::runtime_error("RandomBot has no moves to select from");

	const int index = rand() % possibilities.size();
	std::cout << "Choosing possibility \e[1m" << index << "\e[22m out of \e[1m" << possibilities.size() << "\e[22m.\n";
	return *std::next(possibilities.begin(), index);
}
