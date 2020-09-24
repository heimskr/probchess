#ifndef CHESS_RandomPLAYER_H_
#define CHESS_RandomPLAYER_H_

#include <set>

#include "Board.h"
#include "Move.h"
#include "Player.h"

struct RandomPlayer: public Player {
	using Player::Player;

	void send(const std::string &) override;
	Move chooseMove(const Board &, const std::set<int> &columns);
};

#endif
