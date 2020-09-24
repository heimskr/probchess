#ifndef CHESS_RANDOMPLAYER_H_
#define CHESS_RANDOMPLAYER_H_

#include "AIPlayer.h"

struct RandomPlayer: public AIPlayer {
	using AIPlayer::AIPlayer;

	Move chooseMove(Match &, const std::set<int> &columns) override;
};

#endif
