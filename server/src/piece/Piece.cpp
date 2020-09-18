#include "piece/Piece.h"

void Piece::filter(std::list<Square> &squares) {
	for (auto iter = squares.begin(); iter != squares.end();) {
		if (iter->column < 0 || 7 < iter->column || iter->row < 0 || 7 < iter->row)
			squares.erase(iter++);
		else
			++iter;
	}
}
