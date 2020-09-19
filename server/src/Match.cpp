#include <cstdlib>

#include "Match.h"
#include "ChessError.h"
#include "main.h"
#include "piece/King.h"

Match::Match(const std::string &id_, websocketpp::connection_hdl host_, Color host_color):
id(id_), host(host_), hostColor(host_color) {
	board.placePieces();
}

bool Match::active() const {
	return guest.has_value();
}

void Match::roll() {
	column = ((rand() % 6) + (rand() % 6)) % 8;
	const std::string message = ":Column " + std::to_string(column);
	send(host, message);
	send(*guest, message);
}

void Match::end(Connection *winner) {
	if (winner == &host) {
		send(host, ":Win");
		if (guest.has_value())
			send(*guest, ":Lose");
	} else if (guest.has_value() && winner == &*guest) {
		send(host, ":Lose");
		send(*guest, ":Win");
	} else {
		send(host, ":End");
		if (guest.has_value())
			send(*guest, ":End");
	}
	matchesByID.erase(id);
	matchesByConnection.erase(host.lock().get());
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

	std::shared_ptr<Piece> from_piece = board.at(from);
	if (!from_piece)
		throw ChessError("No source piece");

	if (from_piece->color != currentTurn)
		throw ChessError("Not your piece");

	bool can_move = false;
	for (const Square &possibility: from_piece->canMoveTo()) {
		if (possibility == to) {
			can_move = true;
			break;
		}
	}

	if (!can_move)
		throw ChessError("Invalid move");

	std::shared_ptr<Piece> to_piece = board.at(to);
	if (to_piece) {
		if (to_piece->color == currentTurn)
			throw ChessError("Can't capture own piece");
		if (dynamic_cast<King *>(to_piece.get())) {
			winner = currentTurn;
			end(hostColor == currentTurn? &host : &*guest);
			return;
		}

		board.erase(to_piece);
	}

	board.move(from_piece, to);
	// Check for pawns to promote
	currentTurn = currentTurn == Color::White? Color::Black : Color::White;
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
