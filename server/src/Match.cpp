#include <cstdlib>
#include <unistd.h>

#include "Match.h"
#include "ChessError.h"
#include "main.h"
#include "piece/all.h"

Match::Match(const std::string &id_, websocketpp::connection_hdl host_, Color host_color):
id(id_), host(host_), hostColor(host_color) {
	board.placePieces();
}

bool Match::active() const {
	return guest.has_value();
}

void Match::roll() {
	column = ((rand() % 6 + 1) + (rand() % 6 + 1));
	if (8 < column)
		column -= 8;
	--column;
	sendBoth(":Column " + std::to_string(column));
}

void Match::end(Connection *winner) {
	if (winner == &host) {
		send(host, ":Win");
		if (guest.has_value())
			send(*guest, ":Lose");
	} else if (guest.has_value() && winner == &*guest) {
		send(host, ":Lose");
		send(*guest, ":Win");
	} else sendBoth(":End");
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

	if (from.column != column)
		throw ChessError("Incorrect column");

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
	checkPawns();
	sendBoard();
	while (true) {
		currentTurn = currentTurn == Color::White? Color::Black : Color::White;
		const std::string turn_str = currentTurn == Color::White? "white" : "black";
		sendBoth(":Turn " + turn_str);
		roll();
		if (!canMove()) {
			sendBoth(":Skip");
		} else break;
	}
}

void Match::checkPawns() {
	for (int col = 0; col < 8; ++col) {
		std::shared_ptr<Piece> piece = board.at(0, col);
		if (piece && dynamic_cast<Pawn *>(piece.get()) && piece->color == Color::White) {
			board.erase(piece);
			board.whitePieces.push_back(board.set<Queen>(Color::White, 0, col));
		}
	}

	for (int col = 0; col < 8; ++col) {
		std::shared_ptr<Piece> piece = board.at(7, col);
		if (piece && dynamic_cast<Pawn *>(piece.get()) && piece->color == Color::Black) {
			board.erase(piece);
			board.blackPieces.push_back(board.set<Queen>(Color::Black, 7, col));
		}
	}
}

bool Match::canMove() {
	for (int row = 0; row < 8; ++row) {
		std::shared_ptr<Piece> piece = board.at(row, column);
		if (piece && piece->color == currentTurn && !piece->canMoveTo().empty())
			return true;
	}

	return false;
}

void Match::sendBoth(const std::string &message) {
	send(host, message);
	if (guest.has_value())
		send(*guest, message);
}

void Match::sendBoard() {
	std::string encoded;
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			std::shared_ptr<Piece> piece = board.at(row, col);
			if (!piece) {
				encoded += "__";
			} else {
				if (dynamic_cast<Bishop *>(piece.get())) {
					encoded += "b";
				} else if (dynamic_cast<King *>(piece.get())) {
					encoded += "k";
				} else if (dynamic_cast<Knight *>(piece.get())) {
					encoded += "h";
				} else if (dynamic_cast<Pawn *>(piece.get())) {
					encoded += "p";
				} else if (dynamic_cast<Queen *>(piece.get())) {
					encoded += "q";
				} else if (dynamic_cast<Rook *>(piece.get())) {
					encoded += "r";
				} else {
					throw std::runtime_error("Invalid piece at row " + std::to_string(row) + ", column " +
						std::to_string(col));
				}

				encoded += piece->color == Color::White? "W" : "B";
			}
		}
	}
	sendBoth(":Board " + encoded);
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
