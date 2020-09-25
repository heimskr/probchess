#ifndef CHESS_STOCKFISHPLAYER_H_
#define CHESS_STOCKFISHPLAYER_H_

#include "AIPlayer.h"

struct StockfishPlayer: public AIPlayer {
	using AIPlayer::AIPlayer;

	Move chooseMove(Match &, const std::set<int> &columns) override;
};

#endif
