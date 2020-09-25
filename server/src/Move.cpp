#include <iostream>
#include "Move.h"

Move::Move(const std::string &str): from(0, 0), to(0, 0) {
	from.column = str[0] - 'a';
	from.row = str[1] - '0' - 1;
	to.column = str[2] - 'a';
	to.row = str[3] - '0' - 1;
}

std::string Move::pseudoalgebraic() const {
	return from.algebraic() + to.algebraic();
}
