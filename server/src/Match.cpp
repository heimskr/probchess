#include "Match.h"
#include "ChessError.h"

Match::Match(websocketpp::connection_hdl host_, Color host_color): host(host_), hostColor(host_color) {
	board.placePieces();
}

bool Match::active() const {
	return guest.has_value();
}

void Match::makeMove(websocketpp::connection_hdl connection, Square from, Square to) {
	if (!active())
		throw ChessError("Match not started");
	
	const void *address = connection.lock().get();
	if (address != host.lock().get() && address != guest->lock().get())
		throw ChessError("Invalid connection");

	const bool isHostTurn = currentTurn == hostColor;
	if ((address == host.lock().get() && !isHostTurn) || (address == guest->lock().get() && isHostTurn))
		throw ChessError("Invalid turn");
	
	if (!board.at(from))
		throw ChessError("No source piece");
}
