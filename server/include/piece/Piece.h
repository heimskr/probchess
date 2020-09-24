#ifndef CHESS_PIECE_PIECE_H_
#define CHESS_PIECE_PIECE_H_

#include <list>
#include <ostream>
#include <string>

#include "../Color.h"
#include "../Square.h"

class Board;

class Piece {
	protected:
		std::list<Square> & filter(std::list<Square> &) const;

	public:
		enum class Type {Bishop, King, Knight, Pawn, Queen, Rook};

		Board *parent;
		Square square;
		Color color;

		virtual ~Piece() {}

		Piece(Board *parent_, Color color_, const Square &square_): parent(parent_), square(square_), color(color_) {}
		Piece(Board *parent_, Color color_, int row, int column): Piece(parent_, color_, {row, column}) {}

		virtual std::list<Square> canMoveTo() const = 0;
		virtual std::string name() const = 0;
		virtual std::string toString(Color) const = 0;
		virtual Piece * clone(Board *) const = 0;
		virtual Type getType() const = 0;
		virtual int typeValue() const = 0;

		operator std::string() const { return toString(color); }
};

std::ostream & operator<<(std::ostream &, const Piece &);

#endif
