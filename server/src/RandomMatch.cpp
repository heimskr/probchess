#include "RandomMatch.h"
#include "HumanPlayer.h"
#include "main.h"

RandomMatch::RandomMatch(const std::string &id_, bool hidden_, bool no_skip, int column_count, Color host_color):
Match(id_, hidden_, no_skip, column_count, host_color) {
	guest = std::make_unique<RandomPlayer>(otherColor(host_color), Player::Role::Guest);
}

bool RandomMatch::isActive() const {
	return host.has_value();
}

bool RandomMatch::hasConnection(Connection connection) const {
	if (host.has_value())
		if (HumanPlayer *human_host = dynamic_cast<HumanPlayer *>(host->get()))
			if (human_host->connection.lock().get() == connection.lock().get())
				return true;
	return false;
}

bool RandomMatch::isReady() const {
	return host.has_value();
}

void RandomMatch::afterMove() {
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

	if (currentTurn != hostColor) {
		try {
			makeMove(**guest, dynamic_cast<RandomPlayer *>(guest->get())->chooseMove(board, columns));
		} catch (std::exception &err) {
			std::cerr << "RandomBot couldn't make move: " << err.what() << "\n";
			throw;
		}
	}
}