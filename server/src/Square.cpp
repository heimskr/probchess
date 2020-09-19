#include "Square.h"

std::ostream & operator<<(std::ostream &os, const Square &square) {
	return os << square.row << square.column;
}
