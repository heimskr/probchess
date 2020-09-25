#ifndef CHESS_PIECE_PAWN_H_
#define CHESS_PIECE_PAWN_H_

#include "Piece.h"

class Pawn: public Piece {
	public:
		using Piece::Piece;
		virtual std::list<Square> canMoveTo() const override;
		virtual Piece * clone(Board *new_board) const override { return new Pawn(new_board, color, square); }
		virtual Type getType() const override { return Type::Pawn; }
		virtual int typeValue() const override { return 1; }
		virtual std::string name() const override { return "pawn"; }
		virtual std::string toString(Color color = Color::Black) const override { return color == Color::White? "♙" : "♟"; }
		virtual std::string toFEN() const override { return color == Color::White? "P" : "p"; }
};

#endif
