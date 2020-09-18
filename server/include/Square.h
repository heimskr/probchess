#ifndef CHESS_SQUARE_H_
#define CHESS_SQUARE_H_

#include <utility>

#include "Color.h"

class Square {
	public:
		int row, column;

		Square(int row_, int column_): row(row_), column(column_) {}

		Color getColor() const {
			return row % 2 == column % 2? Color::White : Color::Black;
		}

		bool operator==(const Square &other) const { return row == other.row && column == other.column; }
		bool operator!=(const Square &other) const { return row != other.row || column != other.column; }
		bool operator!() const { return row < 0 || 7 < row || column < 0 || 7 < column; }
		Square operator+(int row_shift) const { return Square(row + row_shift, column); }
		Square operator-(int row_shift) const { return Square(row - row_shift, column); }
		Square operator<<(int column_shift) const { return Square(row, column - column_shift); }
		Square operator>>(int column_shift) const { return Square(row, column + column_shift); }
		Square operator+=(int row_shift) { row += row_shift; return *this; }
		Square operator-=(int row_shift) { row -= row_shift; return *this; }
		Square operator<<=(int column_shift) { column -= column_shift; return *this; }
		Square operator>>=(int column_shift) { column += column_shift; return *this; }
		operator bool() const { return 0 <= row && row < 8 && 0 <= column && column < 8; }

		Square operator+(const std::pair<int, int> &offsets) const {
			return Square(row + offsets.first, column + offsets.second);
		}

		Square operator+=(const std::pair<int, int> &offsets) {
			row += offsets.first;
			column += offsets.second;
			return *this;
		}
};

#endif
