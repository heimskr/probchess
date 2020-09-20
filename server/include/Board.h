#ifndef CHESS_BOARD_H_
#define CHESS_BOARD_H_

#include <array>
#include <memory>

#include "piece/Piece.h"

// Assumption: black starts at the top.

class Board {
	public:
		std::array<std::array<std::shared_ptr<Piece>, 8>, 8> pieces;
		std::list<std::shared_ptr<Piece>> whitePieces, blackPieces;

		Board() {}

		std::shared_ptr<Piece> at(int row, int column) const;
		std::shared_ptr<Piece> at(Square) const;
		void placePieces();
		void erase(std::shared_ptr<Piece>);
		void erase(int row, int column);
		bool move(std::shared_ptr<Piece>, int new_row, int new_column);
		bool move(std::shared_ptr<Piece>, Square);
		std::string toString(std::shared_ptr<Piece>) const;
		operator std::string() const;

		template <typename P>
		std::shared_ptr<Piece> set(Color color, int row_, int column_) {
			return pieces[row_][column_] = std::make_shared<P>(this, color, row_, column_);
		}
};

std::ostream & operator<<(std::ostream &, const Board &);

#endif