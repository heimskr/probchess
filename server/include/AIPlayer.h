#ifndef CHESS_AIPLAYER_H_
#define CHESS_AIPLAYER_H_

#include <set>

#include "Board.h"
#include "Move.h"
#include "Player.h"

struct AIPlayer: public Player {
	using Player::Player;

	void send(const std::string &) override;
	Move chooseMove(const Board &, const std::set<int> &columns);
};

#endif
