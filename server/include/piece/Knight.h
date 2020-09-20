#ifndef CHESS_PIECE_KNIGHT_H_
#define CHESS_PIECE_KNIGHT_H_

#include "Piece.h"

class Knight: public Piece {
	public:
		using Piece::Piece;
		virtual std::list<Square> canMoveTo() const override;
		virtual std::string name() const override { return "knight"; }
		virtual std::string toString(Color color = Color::Black) const override { return color == Color::White? "♘" : "♞"; }
};

#endif
