#ifndef CHESS_PIECE_ROOK_H_
#define CHESS_PIECE_ROOK_H_

#include "Piece.h"

class Rook: public Piece {
	public:
		using Piece::Piece;
		virtual std::list<Square> canMoveTo() const override;
		virtual Piece * clone(Board *new_board) const override { return new Rook(new_board, color, square); }
		virtual Type getType() const override { return Type::Rook; }
		virtual int typeValue() const override { return 5; }
		virtual std::string name() const override { return "rook"; }
		virtual std::string toString(Color color = Color::Black) const override { return color == Color::White? "♖" : "♜"; }
};

#endif
