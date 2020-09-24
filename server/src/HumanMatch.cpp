#include "HumanMatch.h"
#include "HumanPlayer.h"
#include "main.h"

void HumanMatch::end(Player *winner) {
	try {
		if (host.has_value() && winner->role == Player::Role::Host) {
			sendHost(":Win");
			sendGuest(":Lose");
		} else if (guest.has_value() && winner->role == Player::Role::Guest) {
			sendHost(":Lose");
			sendGuest(":Win");
		} else sendBoth(":End");
		sendSpectators(":Win " + colorName(winner->role == Player::Role::Host? hostColor : otherColor(hostColor)));
	} catch (websocketpp::exception &) {}

	if (guest.has_value())
		matchesByConnection.erase((*guest)->connection.lock().get());

	Match::end(winner);
}

void HumanMatch::disconnect(Player &player) {
	HumanPlayer *human = dynamic_cast<HumanPlayer *>(&player);
	if (!human)
		return;

	if (guest.has_value() && (*guest)->connection.lock().get() == human->connection.lock().get())
		guest.reset();

	Match::disconnect(player);
}

bool HumanMatch::isActive() const {
	return host.has_value() || guest.has_value();
}

bool HumanMatch::hasConnection(Connection connection) const {
	if (host.has_value())
		if (HumanPlayer *human_host = dynamic_cast<HumanPlayer *>(host->get()))
			if (human_host->connection.lock().get() == connection.lock().get())
				return true;
	if (guest.has_value())
		if (HumanPlayer *human_guest = dynamic_cast<HumanPlayer *>(guest->get()))
			if (human_guest->connection.lock().get() == connection.lock().get())
				return true;
	return false;
}

Player & HumanMatch::currentPlayer() {
	
}

bool HumanMatch::hasBoth() const {
	return host.has_value() && guest.has_value();
}

bool HumanMatch::sendGuest(const std::string &message) {
	if (guest.has_value()) {
		(*guest)->send(message);
		return true;
	}

	return false;
}

void HumanMatch::sendBoth(const std::string &message) {
	if (host.has_value())
		(*host)->send(message);
	if (guest.has_value())
		(*guest)->send(message);
}

void HumanMatch::sendAll(const std::string &message) {
	sendBoth(message);
	sendSpectators(message);
}

bool HumanMatch::isReady() const {
	return hasBoth();
}

Player & HumanMatch::getWhite() {
	return hostColor == Color::White? **host : **guest;
}

Player & HumanMatch::getBlack() {
	return hostColor == Color::Black? **host : **guest;
}

Player & HumanMatch::get(Color color) {
	return color == Color::White? getWhite() : getBlack();
}
