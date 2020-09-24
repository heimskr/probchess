#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(Connection connection_): connection(connection_) {}

void HumanPlayer::send(const std::string &message) {
	::send(connection, message);
}
