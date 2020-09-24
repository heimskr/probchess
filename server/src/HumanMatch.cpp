#include "HumanMatch.h"
#include "HumanPlayer.h"
#include "main.h"

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

void HumanMatch::sendAll(const std::string &message) {
	sendBoth(message);
	sendSpectators(message);
}

bool HumanMatch::isReady() const {
	return hasBoth();
}

void HumanMatch::afterMove(Player &player, Square from, Square to) {
	std::cout << "\e[1mSkip-checking loop started.\e[0m\n";

	if (noSkip) {
		currentTurn = currentTurn == Color::White? Color::Black : Color::White;
		sendAll(":Turn " + colorName(currentTurn));
		do roll(); while (!canMove());
	} else {
		while (true) {
			currentTurn = currentTurn == Color::White? Color::Black : Color::White;
			sendAll(":Turn " + colorName(currentTurn));
			roll();
			if (!canMove()) {
				sendAll(":Skip");
			} else break;
		}
	}

	std::cout << "\e[1mSkip-checking loop ended.\e[0m\n";
}
