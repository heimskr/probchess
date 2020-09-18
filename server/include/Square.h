#ifndef CHESS_SQUARE_H_
#define CHESS_SQUARE_H_

#include "Color.h"

class Square {
	public:
		int row, column;

		Square(int row_, int column_): row(row_), column(column_) {}

		Color getColor() const {
			return row % 2 == column % 2? Color::White : Color::Black;
		}

		bool operator==(const Square &other) const {
			return row == other.row && column == other.column;
		}
};

#endif
