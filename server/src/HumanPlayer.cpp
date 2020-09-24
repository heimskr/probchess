#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(Color color_, Role role_, Connection connection_):
	Player(color_, role_), connection(connection_) {}

void HumanPlayer::send(const std::string &message) {
	::send(connection, message);
}
