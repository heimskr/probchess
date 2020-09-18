#include "Match.h"
#include "ChessError.h"
#include "piece/King.h"

Match::Match(websocketpp::connection_hdl host_, Color host_color): host(host_), hostColor(host_color) {
	board.placePieces();
}

bool Match::active() const {
	return guest.has_value();
}

void Match::makeMove(websocketpp::connection_hdl connection, Square from, Square to) {
	if (!active())
		throw ChessError("Match not started");

	if (winner.has_value())
		throw ChessError("Match already over");
	
	const void *address = connection.lock().get();
	if (address != host.lock().get() && address != guest->lock().get())
		throw ChessError("Invalid connection");

	const bool isHostTurn = currentTurn == hostColor;
	if ((address == host.lock().get() && !isHostTurn) || (address == guest->lock().get() && isHostTurn))
		throw ChessError("Invalid turn");

	if (!board.at(from))
		throw ChessError("No source piece");

	std::shared_ptr<Piece> to_piece = board.at(to);
	if (to_piece) {
		if (to_piece->color == currentTurn)
			throw ChessError("Can't capture own piece");
		board.erase(to_piece);
		if (dynamic_cast<King *>(to_piece.get()))
			winner = currentTurn;
	}
}

websocketpp::connection_hdl Match::getWhite() const {
	return hostColor == Color::White? host : *guest;
}

websocketpp::connection_hdl Match::getBlack() const {
	return hostColor == Color::Black? host : *guest;
}

websocketpp::connection_hdl Match::get(Color color) const {
	return color == Color::White? getWhite() : getBlack();
}
