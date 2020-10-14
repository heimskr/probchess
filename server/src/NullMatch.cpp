#include "NullMatch.h"
#include "HumanPlayer.h"
#include "NoKingError.h"
#include "Options.h"
#include "main.h"

bool NullMatch::isActive() const {
	return host.has_value();
}

bool NullMatch::hasConnection(Connection connection) const {
	if (host.has_value())
		if (HumanPlayer *human_host = dynamic_cast<HumanPlayer *>(host->get()))
			if (human_host->connection.lock().get() == connection.lock().get())
				return true;
	return false;
}

bool NullMatch::isReady() const {
	return host.has_value();
}

void NullMatch::afterMove() {
	sendAll(":Turn " + colorName(currentTurn));
	do roll(); while (!canMove());
}
