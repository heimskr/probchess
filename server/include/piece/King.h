#ifndef CHESS_PIECE_KING_H_
#define CHESS_PIECE_KING_H_

#include "Piece.h"

class King: public Piece {
	public:
		using Piece::Piece;
		virtual std::list<Square> canMoveTo() const override;
		virtual std::string name() const override { return "king"; }
		virtual std::string toString(Color color = Color::Black) const override { return color == Color::White? "♔" : "♚"; }
};

#endif
