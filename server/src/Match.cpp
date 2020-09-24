#include <cstdlib>
#include <unistd.h>

#include "ChessError.h"
#include "HumanPlayer.h"
#include "Match.h"
#include "main.h"
#include "piece/all.h"

Match::Match(const std::string &id_, bool hidden_, bool no_skip, int column_count, Color host_color):
id(id_), hidden(hidden_), noSkip(no_skip), hostColor(host_color), columnCount(column_count) {
	board.placePieces();
}

void Match::roll() {
	columns.clear();
	for (int i = 0; i < columnCount; ++i) {
		int column;
		do {
			column = 0;
			for (int i = 0; i < board.width; i += 6)
				column += rand() % 6 + 1;
			while (board.width < column)
				column -= board.width;
			--column;
		} while (columns.count(column) != 0);
		columns.insert(column);
	}

	sendAll(columnMessage());
}

void Match::end(Player *winner) {
	try {
		if (!winner) {
			sendAll(":End");
		} else {
			if (host.has_value() && winner->role == Player::Role::Host) {
				sendHost(":Win");
				sendGuest(":Lose");
			} else if (guest.has_value() && winner->role == Player::Role::Guest) {
				sendHost(":Lose");
				sendGuest(":Win");
			} else sendAll(":End");
			sendSpectators(":Win " + colorName(winner->role == Player::Role::Host? hostColor : otherColor(hostColor)));
		}
	} catch (websocketpp::exception &) {}

	matchesByID.erase(id);

	if (guest.has_value())
		if (HumanPlayer *human_guest = dynamic_cast<HumanPlayer *>(guest->get()))
			matchesByConnection.erase(human_guest->connection.lock().get());

	if (host.has_value())
		if (HumanPlayer *human_host = dynamic_cast<HumanPlayer *>(host->get()))
			matchesByConnection.erase(human_host->connection.lock().get());

	for (Connection spectator: spectators)
		matchesByConnection.erase(spectator.lock().get());

	if (!hidden)
		broadcast(":RemoveMatch " + id);
}

void Match::disconnect(Connection connection) {
	const void *vptr = connection.lock().get();

	spectators.remove_if([&](Connection hdl) { return hdl.lock().get() == vptr; });

	if (host.has_value())
		if (HumanPlayer *human_host = dynamic_cast<HumanPlayer *>(host->get()))
			if (human_host->connection.lock().get() == vptr)
				host.reset();

	if (guest.has_value())
		if (HumanPlayer *human_guest = dynamic_cast<HumanPlayer *>(guest->get()))
			if (human_guest->connection.lock().get() == vptr)
				guest.reset();

	if (!isActive()) {
		std::cout << "Ending match \e[31m" << id << "\e[39m: all players disconnected.\n";
		end(nullptr);
	} else if (!hidden)
		broadcast(":Match " + id + " open");
}

Player & Match::getPlayer(Connection connection) {
	if (host.has_value())
		if (HumanPlayer *human_host = dynamic_cast<HumanPlayer *>(host->get()))
			if (human_host->connection.lock().get() == connection.lock().get())
				return *human_host;
	if (guest.has_value())
		if (HumanPlayer *human_guest = dynamic_cast<HumanPlayer *>(guest->get()))
			if (human_guest->connection.lock().get() == connection.lock().get())
				return *human_guest;
	throw std::runtime_error("Couldn't find player for connection");
}

Player & Match::currentPlayer() {
	return currentTurn == hostColor? **host : **guest;
}

void Match::makeMove(Player &player, const Move &move) {
	if (!isReady())
		throw ChessError("Match is missing a participant.");

	if (winnerColor.has_value())
		throw ChessError("Match already over.");

	if (HumanPlayer *human = dynamic_cast<HumanPlayer *>(&player))
		if (!hasConnection(human->connection))
			throw ChessError("Invalid connection.");

	if (&player != &currentPlayer())
		throw ChessError("Invalid turn.");

	std::shared_ptr<Piece> from_piece = board.at(move.from);
	if (!from_piece)
		throw ChessError("No source piece.");

	if (from_piece->color != currentTurn)
		throw ChessError("Not your piece.");

	if (move.from == move.to)
		throw ChessError("Move must actually move a piece.");

	if (columns.count(move.from.column) == 0)
		throw ChessError("Incorrect column.");

	bool can_move = false;
	for (const Square &possibility: from_piece->canMoveTo()) {
		if (possibility == move.to) {
			can_move = true;
			break;
		}
	}

	if (!can_move)
		throw ChessError("Invalid move.");

	std::shared_ptr<Piece> to_piece = board.at(move.to);
	if (to_piece) {
		if (to_piece->color == currentTurn)
			throw ChessError("Can't capture one of your own pieces.");
		sendAll(capturedMessage(to_piece));
		captured.push_back(to_piece);
		board.erase(to_piece);
		if (dynamic_cast<King *>(to_piece.get())) {
			board.move(from_piece, move.to);
			checkPawns();
			sendBoard();
			winnerColor = currentTurn;
			end(hostColor == currentTurn? host->get() : guest->get());
			return;
		}
	}

	board.move(from_piece, move.to);
	checkPawns();
	sendBoard();
	sendAll(":MoveMade " + std::string(move.from) + " " + std::string(move.to));
	afterMove();
}

void Match::checkPawns() {
	for (int column = 0; column < board.width; ++column) {
		std::shared_ptr<Piece> piece = board.at(0, column);
		if (piece && dynamic_cast<Pawn *>(piece.get()) && piece->color == Color::White) {
			board.erase(piece);
			board.whitePieces.push_back(board.set<Queen>(Color::White, 0, column));
		}
	}

	for (int column = 0; column < board.width; ++column) {
		std::shared_ptr<Piece> piece = board.at(board.height - 1, column);
		if (piece && dynamic_cast<Pawn *>(piece.get()) && piece->color == Color::Black) {
			board.erase(piece);
			board.blackPieces.push_back(board.set<Queen>(Color::Black, board.height - 1, column));
		}
	}
}

bool Match::canMove() const {
	std::cout << board << "\n";
	for (const int column: columns) {
		std::cout << "Scanning column \e[1m" << column << "\e[22m for pieces.\n";
		for (int row = 0; row < board.height; ++row) {
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
	}

	std::cout << "\e[31mNo moves were found.\e[0m\n";
	return false;
}

bool Match::anyCanMove() const {
	for (int column = 0; column < board.width; ++column)
		for (int row = 0; row < board.height; ++row) {
			std::shared_ptr<Piece> piece = board.at(row, column);
			if (piece && piece->color == currentTurn && !piece->canMoveTo().empty())
				return true;
		}
	return false;
}

bool Match::sendHost(const std::string &message) {
	if (host.has_value()) {
		(*host)->send(message);
		return true;
	}

	return false;
}

bool Match::sendGuest(const std::string &message) {
	if (guest.has_value()) {
		(*guest)->send(message);
		return true;
	}

	return false;
}

void Match::sendBoth(const std::string &message) {
	if (host.has_value())
		(*host)->send(message);
	if (guest.has_value())
		(*guest)->send(message);
}

void Match::sendAll(const std::string &message) {
	sendBoth(message);
	sendSpectators(message);
}

void Match::sendSpectators(const std::string &message) {
	for (Connection spectator: spectators)
		send(spectator, message);
}

void Match::sendBoard() {
	std::string encoded;
	for (int row = 0; row < board.height; ++row) {
		for (int column = 0; column < board.width; ++column) {
			std::shared_ptr<Piece> piece = board.at(row, column);
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
						std::to_string(column));
				}

				encoded += piece->color == Color::White? "W" : "B";
			}
		}
	}

	sendAll(":Board " + encoded);
}

void Match::invertTurn() {
	currentTurn = currentTurn == Color::White? Color::Black : Color::White;
}

std::string Match::capturedMessage(std::shared_ptr<Piece> piece) {
	return ":Capture " + std::string(piece->square) + " " + piece->name() + " " + colorName(piece->color);
}

std::string Match::columnMessage() {
	std::string message = ":Columns";
	for (const int column: columns)
		message += " " + std::to_string(column);
	return message;
}

Player & Match::getWhite() {
	return hostColor == Color::White? **host : **guest;
}

Player & Match::getBlack() {
	return hostColor == Color::Black? **host : **guest;
}

Player & Match::get(Color color) {
	return color == Color::White? getWhite() : getBlack();
}
