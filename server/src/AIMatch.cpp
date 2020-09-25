#include "AIMatch.h"
#include "HumanPlayer.h"
#include "NoKingError.h"
#include "RandomPlayer.h"
#include "main.h"

bool AIMatch::isActive() const {
	return host.has_value();
}

bool AIMatch::hasConnection(Connection connection) const {
	if (host.has_value())
		if (HumanPlayer *human_host = dynamic_cast<HumanPlayer *>(host->get()))
			if (human_host->connection.lock().get() == connection.lock().get())
				return true;
	return false;
}

bool AIMatch::isReady() const {
	return host.has_value();
}

void AIMatch::afterMove() {
	std::cout << "\e[1mSkip-checking loop started.\e[0m\n";

	if (noSkip) {
		invertTurn();
		sendAll(":Turn " + colorName(currentTurn));
		do roll(); while (!canMove());
	} else {
		while (true) {
			invertTurn();
			sendAll(":Turn " + colorName(currentTurn));
			roll();
			if (!canMove()) {
				sendAll(":Skip");
			} else break;
		}
	}

	std::cout << "\e[1mSkip-checking loop ended.\e[0m\n";

	if (currentTurn != hostColor && !over)
		makeMove(**guest, dynamic_cast<AIPlayer *>(guest->get())->chooseMove(*this, columns));
}
