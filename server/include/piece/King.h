#ifndef CHESS_PIECE_KING_H_
#define CHESS_PIECE_KING_H_

#include "Piece.h"

class King: public Piece {
	public:
		using Piece::Piece;
		virtual std::list<Square> canMoveTo() const override;
		virtual Piece * clone(Board *new_board) const override { return new King(new_board, color, square); }
		virtual Type getType() const override { return Type::King; }
		virtual int typeValue() const override { return 0; }
		virtual std::string name() const override { return "king"; }
		virtual std::string toString(Color color = Color::Black) const override { return color == Color::White? "♔" : "♚"; }
};

#endif
