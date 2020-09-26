#include "AIMatch.h"
#include "HumanPlayer.h"
#include "NoKingError.h"
#include "Options.h"
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
#ifdef DEBUG_SKIPS
	std::cout << "\e[1mSkip-checking loop started.\e[0m\n";
#endif

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

#ifdef DEBUG_SKIPS
	std::cout << "\e[1mSkip-checking loop ended.\e[0m\n";
#endif

	if (currentTurn != hostColor && !over) {
		Move move = dynamic_cast<AIPlayer *>(guest->get())->chooseMove(*this, columns);
		makeMove(**guest, move);
	}
}
