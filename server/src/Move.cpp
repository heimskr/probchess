#include "Move.h"

std::string Move::pseudoalgebraic() const {
	return from.algebraic() + to.algebraic();
}
