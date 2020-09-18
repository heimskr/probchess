#include "Board.h"
#include "piece/Bishop.h"

std::list<Square> Bishop::canMoveTo() {
	std::list<Square> out;

	Square next = square;
	for (int i = 0; i < 7; ++i) { // down and to the right
		next += 1;
		next >>= 1;
		if (!next || parent->at(square))
			break;
		out.push_back(square);
	}

	next = square;
	for (int i = 0; i < 7; ++i) { // down and to the left
		next += 1;
		next <<= 1;
		if (!next || parent->at(square))
			break;
		out.push_back(square);
	}

	next = square;
	for (int i = 0; i < 7; ++i) { // up and to the left
		next -= 1;
		next <<= 1;
		if (!next || parent->at(square))
			break;
		out.push_back(square);
	}

	next = square;
	for (int i = 0; i < 7; ++i) { // up and to the right
		next -= 1;
		next >>= 1;
		if (!next || parent->at(square))
			break;
		out.push_back(square);
	}

	filter(out);
	return out;
}
