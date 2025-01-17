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

bool HumanMatch::isReady() const {
	return host.has_value() && guest.has_value();
}

void HumanMatch::afterMove() {
#ifdef DEBUG_SKIPS
	std::cout << "\e[1mSkip-checking loop started.\e[0m\n";
#endif

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

#ifdef DEBUG_SKIPS
	std::cout << "\e[1mSkip-checking loop ended.\e[0m\n";
#endif
}
