#ifndef CHESS_PIECE_PIECE_H_
#define CHESS_PIECE_PIECE_H_

#include <list>
#include <string>

#include "../Color.h"
#include "../Square.h"

class Board;

class Piece {
	protected:
		std::list<Square> & filter(std::list<Square> &) const;

	public:
		Board *parent;
		Square square;
		Color color;

		Piece(Board *parent_, Color color_, const Square &square_): parent(parent_), square(square_), color(color_) {}
		Piece(Board *parent_, Color color_, int row, int column): Piece(parent_, color_, {row, column}) {}

		virtual std::list<Square> canMoveTo() const = 0;
		virtual std::string toString(Color) const = 0;

		operator std::string() const { return toString(color); }
};

#endif
