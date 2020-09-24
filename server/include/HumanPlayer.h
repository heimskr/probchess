#ifndef CHESS_HUMANPLAYER_H_
#define CHESS_HUMANPLAYER_H_

#include "Player.h"
#include "WebSocket.h"

struct HumanPlayer: public Player {
	Connection connection;

	HumanPlayer(Color color_, Role role_, Connection connection_);

	void send(const std::string &) override;
};

#endif