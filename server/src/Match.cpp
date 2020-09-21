#include <cstdlib>
#include <unistd.h>

#include "Match.h"
#include "ChessError.h"
#include "main.h"
#include "piece/all.h"

Match::Match(const std::string &id_, bool hidden_, Connection host_, Color host_color):
id(id_), hidden(hidden_), host(host_), hostColor(host_color) {
	board.placePieces();
}

void Match::roll() {
	column = ((rand() % 6 + 1) + (rand() % 6 + 1));
	if (8 < column)
		column -= 8;
	--column;
	sendAll(":Column " + std::to_string(column));
}

void Match::end(Connection *winner) {
	try {
		if (host.has_value() && winner == &*host) {
			sendHost(":Win");
			sendGuest(":Lose");
		} else if (guest.has_value() && winner == &*guest) {
			sendHost(":Lose");
			sendGuest(":Win");
		} else sendBoth(":End");
		sendSpectators(":Win " + colorName(winner == &*host? hostColor : otherColor(hostColor)));
	} catch (websocketpp::exception &) {}

	matchesByID.erase(id);

	if (host.has_value())
		matchesByConnection.erase(host->lock().get());

	if (guest.has_value())
		matchesByConnection.erase(guest->lock().get());

	for (Connection spectator: spectators)
		matchesByConnection.erase(spectator.lock().get());

	if (!hidden)
		broadcast(":RemoveMatch " + id);
}

void Match::disconnect(Connection connection) {
	const void *vptr = connection.lock().get();

	for (Connection spectator: spectators) {
		if (spectator.lock().get() == vptr) {
			spectators.remove_if([&](Connection hdl) { return hdl.lock().get() == vptr; });
			return;
		}
	}

	if (host.has_value() && host->lock().get() == vptr)
		host.reset();
	else if (guest.has_value() && guest->lock().get() == vptr)
		guest.reset();

	if (!host.has_value() && !guest.has_value()) {
		std::cout << "Ending match \e[31m" << id << "\e[39m: all clients disconnected.\n";
		end(nullptr);
	} else if (!hidden)
		broadcast(":Match " + id + " open");
}

bool Match::hasBoth() const {
	return host.has_value() && guest.has_value();
}

void Match::makeMove(Connection connection, Square from, Square to) {
	if (!hasBoth())
		throw ChessError("Match is missing a participant");

	if (winner.has_value())
		throw ChessError("Match already over");
	
	const void *address = connection.lock().get();
	if (address != host->lock().get() && address != guest->lock().get())
		throw ChessError("Invalid connection");

	const bool isHostTurn = currentTurn == hostColor;
	if ((address == host->lock().get() && !isHostTurn) || (address == guest->lock().get() && isHostTurn))
		throw ChessError("Invalid turn");

	std::shared_ptr<Piece> from_piece = board.at(from);
	if (!from_piece)
		throw ChessError("No source piece");

	if (from_piece->color != currentTurn)
		throw ChessError("Not your piece");

	if (from == to)
		throw ChessError("Move must actually move a piece");

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
		sendCaptured(*host, to_piece);
		sendCaptured(*guest, to_piece);
		for (Connection spectator: spectators)
			sendCaptured(spectator, to_piece);
		captured.push_back(to_piece);
		board.erase(to_piece);
		if (dynamic_cast<King *>(to_piece.get())) {
			board.move(from_piece, to);
			checkPawns();
			sendBoard();
			winner = currentTurn;
			end(hostColor == currentTurn? &*host : &*guest);
			return;
		}
	}

	board.move(from_piece, to);
	checkPawns();
	sendBoard();
	std::cout << "\e[1mSkip-checking loop started.\e[0m\n";
	while (true) {
		currentTurn = currentTurn == Color::White? Color::Black : Color::White;
		const std::string turn_str = currentTurn == Color::White? "white" : "black";
		sendAll(":Turn " + turn_str);
		roll();
		if (!canMove()) {
			sendAll(":Skip");
		} else break;
	}
	std::cout << "\e[1mSkip-checking loop ended.\e[0m\n";
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
	std::cout << board << "\n";
	std::cout << "Scanning column \e[1m" << column << "\e[22m for pieces.\n";
	for (int row = 0; row < 8; ++row) {
		std::shared_ptr<Piece> piece = board.at(row, column);
		if (piece) {
			std::cout << "\e[2m-\e[22m Found a piece at " << row << column << ": " << *piece << "\n";
			if (piece->color == currentTurn) {
				std::cout << "\e[2m--\e[22m Correct color.\n";
				std::cout << "\e[2m---\e[22m Moves:";
				for (const Square &move: piece->canMoveTo())
					std::cout << " " << move;
				std::cout << "\n";
				if (piece->canMoveTo().empty())
					std::cout << "\e[2m---\e[22m No moves.\n";
				else {
					std::cout << "\e[2;32m---\e[22m A move was found.\e[0m\n";
					return true;
				}
			} else std::cout << "\e[2m--\e[22m Incorrect color.\n";
		} else std::cout << "\e[2m-\e[22m No piece at " << row << column << ".\n";
		// if (piece && piece->color == currentTurn && !piece->canMoveTo().empty())
		// 	return true;
	}

	std::cout << "\e[31mNo moves were found.\e[0m\n";
	return false;
}

bool Match::sendHost(const std::string &message) {
	if (host.has_value()) {
		send(*host, message);
		return true;
	}

	return false;
}

bool Match::sendGuest(const std::string &message) {
	if (guest.has_value()) {
		send(*guest, message);
		return true;
	}

	return false;
}

void Match::sendBoth(const std::string &message) {
	if (host.has_value())
		send(*host, message);
	if (guest.has_value())
		send(*guest, message);
}

void Match::sendAll(const std::string &message) {
	sendBoth(message);
	sendSpectators(message);
}

void Match::sendSpectators(const std::string &message) {
	for (Connection spectator: spectators)
		send(spectator, message);
}

void Match::sendCaptured(Connection connection, std::shared_ptr<Piece> piece) {
	send(connection, ":Capture " + std::string(piece->square) + " " + piece->name() + " " + colorName(piece->color));
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

	sendAll(":Board " + encoded);
}

Connection Match::getWhite() const {
	return hostColor == Color::White? *host : *guest;
}

Connection Match::getBlack() const {
	return hostColor == Color::Black? *host : *guest;
}

Connection Match::get(Color color) const {
	return color == Color::White? getWhite() : getBlack();
}
