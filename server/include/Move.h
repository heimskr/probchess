#ifndef CHESS_MOVE_H_
#define CHESS_MOVE_H_

#include "Square.h"

struct Move {
	Square from, to;
	Move(Square from_, Square to_): from(from_), to(to_) {}
};

#endif
