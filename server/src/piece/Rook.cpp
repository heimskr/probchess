#include "Board.h"
#include "piece/Rook.h"

std::list<Square> Rook::canMoveTo() {
	std::list<Square> out;

	Square next = square;
	for (int i = 0; i < 7; ++i) { // up
		next -= 1;
		if (!next)
			break;
		out.push_back(square);
		if (parent->at(square))
			break;
	}

	next = square;
	for (int i = 0; i < 7; ++i) { // down
		next += 1;
		if (!next)
			break;
		out.push_back(square);
		if (parent->at(square))
			break;
	}

	next = square;
	for (int i = 0; i < 7; ++i) { // left
		next <<= 1;
		if (!next)
			break;
		out.push_back(square);
		if (parent->at(square))
			break;
	}

	next = square;
	for (int i = 0; i < 7; ++i) { // right
		next >>= 1;
		if (!next)
			break;
		out.push_back(square);
		if (parent->at(square))
			break;
	}

	filter(out);
	return out;
}
