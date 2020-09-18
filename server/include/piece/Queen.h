#ifndef CHESS_PIECE_QUEEN_H_
#define CHESS_PIECE_QUEEN_H_

#include "Piece.h"

class Queen: public Piece {
	public:
		using Piece::Piece;
		virtual std::list<Square> canMoveTo() override;
		virtual std::string toString(Color color = Color::Black) const override { return color == Color::White? "♕" : "♛"; }
};

#endif