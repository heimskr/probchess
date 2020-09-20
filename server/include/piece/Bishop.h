#ifndef CHESS_PIECE_BISHOP_H_
#define CHESS_PIECE_BISHOP_H_

#include "Piece.h"

class Bishop: public Piece {
	public:
		using Piece::Piece;
		virtual std::list<Square> canMoveTo() const override;
		virtual std::string name() const override { return "bishop"; }
		virtual std::string toString(Color color = Color::Black) const override { return color == Color::White? "♗" : "♝"; }
};

#endif
