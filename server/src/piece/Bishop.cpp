#include "Board.h"
#include "piece/Bishop.h"

std::list<Square> Bishop::canMoveTo() const {
	std::list<Square> out;

	Square next = square;
	for (int i = 0; i < 7; ++i) { // down and to the right
		next += {1, 1};
		if (!next)
			break;
		out.push_back(next);
		if (parent->at(next))
			break;
	}

	next = square;
	for (int i = 0; i < 7; ++i) { // down and to the left
		next += {1, -1};
		if (!next)
			break;
		out.push_back(next);
		if (parent->at(next))
			break;
	}

	next = square;
	for (int i = 0; i < 7; ++i) { // up and to the left
		next += {-1, -1};
		if (!next)
			break;
		out.push_back(next);
		if (parent->at(next))
			break;
	}

	next = square;
	for (int i = 0; i < 7; ++i) { // up and to the right
		next += {-1, 1};
		if (!next)
			break;
		out.push_back(next);
		if (parent->at(next))
			break;
	}

	filter(out);
	return out;
}
