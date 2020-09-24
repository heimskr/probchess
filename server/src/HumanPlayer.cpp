#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(Role role_, Connection connection_): Player(role_), connection(connection_) {}

void HumanPlayer::send(const std::string &message) {
	::send(connection, message);
}
