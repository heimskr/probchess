#ifndef CHESS_AIPLAYER_H_
#define CHESS_AIPLAYER_H_

#include <set>

#include "Board.h"
#include "Move.h"
#include "Player.h"

class Match;

struct AIPlayer: public Player {
	using Player::Player;

	void send(const std::string &) override;
	virtual Move chooseMove(Match &, const std::set<int> &columns) = 0;
};

#endif
