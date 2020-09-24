#ifndef CHESS_AIPLAYER_H_
#define CHESS_AIPLAYER_H_

#include "Player.h"

struct AIPlayer: public Player {
	void send(const std::string &) override;
};

#endif
