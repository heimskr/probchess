#include "Board.h"
#include "piece/Pawn.h"

std::list<Square> Pawn::canMoveTo() const {
	std::list<Square> out;
	Square next = square + (color == Color::Black? 1 : -1);

	if (!parent->at(next))
		out.push_back(next);

	next = next << 1;
	if (parent->at(next))
		out.push_back(next);

	next = next >> 2;
	if (parent->at(next))
		out.push_back(next);

	filter(out);
	return out;
}
