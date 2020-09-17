#ifndef CHESS_BOARD_H_
#define CHESS_BOARD_H_

#include <array>
#include <memory>

#include "piece/Piece.h"

// Assumption: black starts at the top.

class Board {
	public:
		std::array<std::array<std::shared_ptr<Piece>, 8>, 8> pieces;

		Board() {}

		std::shared_ptr<Piece> at(int row, int column) const;
		std::shared_ptr<Piece> at(Square) const;
		void placePieces();
		operator std::string() const;

		template <typename P>
		std::shared_ptr<Piece> set(Color color, int row_, int column_) {
			return pieces[row_][column_] = std::make_shared<P>(this, color, row_, column_);
		}
};

#endif