#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <time.h>

#include "AIMatch.h"
#include "Board.h"
#include "ChessError.h"
#include "HumanMatch.h"
#include "HumanPlayer.h"
#include "RandomPlayer.h"
#include "Square.h"
#include "Util.h"
#include "WebSocket.h"
#include "main.h"
#include "piece/all.h"

asio_server *server;
std::unordered_map<std::string, std::shared_ptr<Match>> matchesByID;
std::unordered_map<void *, std::shared_ptr<Match>> matchesByConnection;

int main(int argc, char **argv) {
	srand(time(NULL));

	if (argc < 2) {
		std::cerr << "Usage: chess <port>\n";
		exit(1);
	}

	long port;
	try {
		port = parseLong(argv[1], 10);
	} catch (std::invalid_argument &) {
		std::cerr << "Not a valid port: " << argv[1] << "\n";
	}

	signal(SIGINT, signal_handler);

	server = new asio_server;
	server->set_error_channels(websocketpp::log::elevel::all);
	// server->set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload ^ websocketpp::log::alevel::frame_header);
	server->set_access_channels(websocketpp::log::alevel::none);
	server->init_asio();
	server->set_message_handler(std::bind(&echo_handler, std::placeholders::_1, std::placeholders::_2));
	server->set_open_handler(std::bind(&open_handler, std::placeholders::_1));
	server->set_close_handler(std::bind(&close_handler, std::placeholders::_1));
	server->listen(port);
	server->start_accept();
	server->run();
	delete server;
}

void signal_handler(int) {
	std::cout << "Goodbye.\n";
	server->stop_listening();
	for (Connection hdl: connections) {
		server->pause_reading(hdl);
		server->close(hdl, websocketpp::close::status::going_away, "Server shutting down.");
	}
	server->stop();
}

void open_handler(Connection hdl) {
	connections.push_back(hdl);
}

void close_handler(Connection hdl) {
	connections.remove_if([&](Connection connection) { return connection.lock().get() == hdl.lock().get(); });
	std::shared_ptr<Match> match;
	try {
		match = matchesByConnection.at(hdl.lock().get());
	} catch (std::out_of_range &) {
		std::cerr << "Connection lost from client not in a match.\n";
		return;
	}

	std::cerr << "Disconnecting client from match \e[33m" << match->id << "\e[39m.\n";
	match->disconnect(hdl);
	matchesByConnection.erase(hdl.lock().get());
}

void echo_handler(Connection hdl, asio_server::message_ptr msg_ptr) {
	const std::string &msg = msg_ptr->get_payload();
	if (msg.empty() || msg[0] != ':') {
		send(hdl, ":Error Invalid message.");
		return;
	}

	const std::vector<std::string> words = split(msg.substr(1), " ");
	const std::string &verb = words[0];

	if (verb == "Create") { // :Create <id> <column-count> <black/white> <hidden/public> <skip/noskip> <type>
		if (words.size() != 7 || words[1].empty() || words[2].size() != 1) {
			send(hdl, ":Error Invalid message.");
			return;
		}

		createMatch(hdl, words[1], words[2][0] - '0', words[3] == "black"? Color::Black : Color::White,
			words[4] == "hidden", words[5] == "noskip", words[6]);
		return;
	}

	if (verb == "Join") { // :Join <id>
		if (words.size() != 2 || words[1].empty()) {
			send(hdl, ":Error Invalid message.");
			return;
		}

		joinMatch(hdl, words[1], false);
		return;
	}

	if (verb == "Spectate") { // :Spectate <id>
		if (words.size() != 2 || words[1].empty()) {
			send(hdl, ":Error Invalid message.");
			return;
		}

		joinMatch(hdl, words[1], true);
		return;
	}

	if (verb == "CreateOrJoin") { // :CreateOrJoin <id> <column-count> <black/white> <hidden/public> <skip/noskip> <type>
		if (words.size() != 7 || words[1].empty() || words[2].size() != 1) {
			send(hdl, ":Error Invalid message.");
			return;
		}

		if (matchesByID.count(words[1]) > 0)
			joinMatch(hdl, words[1], false);
		else
			createMatch(hdl, words[1], words[2][0] - '0', words[3] == "black"? Color::Black : Color::White,
				words[4] == "hidden", words[5] == "noskip", words[6]);
		return;
	}

	if (verb == "Move") { // :Move <from-square> <to-square>
		if (words.size() != 3) {
			send(hdl, ":Error Invalid message.");
			return;
		}

		for (const std::string &str: {words[1], words[2]})
			if (str.size() != 2 || str.find_first_not_of("01234567") != std::string::npos) {
				send(hdl, ":Error Invalid message.");
				return;
			}

		if (matchesByConnection.count(hdl.lock().get()) == 0) {
			send(hdl, ":Error Not in a match.");
			return;
		}

		std::shared_ptr<Match> match = matchesByConnection.at(hdl.lock().get());
		Square from {words[1][0] - '0', words[1][1] - '0'};
		Square to   {words[2][0] - '0', words[2][1] - '0'};

		try {
			match->makeMove(match->getPlayer(hdl), {from, to});
		} catch (ChessError &err) {
			send(hdl, ":Error " + std::string(err.what()));
			return;
		}

		return;
	}

	if (verb == "Show") { // :Show <column>
		if (words.size() != 2 || words[1].size() != 2 || words[1].find_first_not_of("01234567") != std::string::npos) {
			send(hdl, ":Error Invalid message.");
			return;
		}

		std::shared_ptr<Match> match;
		try {
			match = matchesByConnection.at(hdl.lock().get());
		} catch (std::out_of_range &) {
			send(hdl, ":Error Not in a match.");
			return;
		}

		std::cout << "Show[" << (words[1][0] - '0') << "][" << (words[1][1] - '0') << "]\n";
		std::cout << match->board.toString(match->board.at(words[1][0] - '0', words[1][1] - '0')) << "\n";
		return;
	}

	if (verb == "GetMatches") { // :GetMatches
		send(hdl, ":ClearMatches");
		for (const std::pair<const std::string, std::shared_ptr<Match>> &pair: matchesByID) {
			const std::shared_ptr<Match> match = pair.second;
			if (!match->hidden)
				send(hdl, ":Match " + pair.first + " " + (match->isReady()? "closed" : "open"));
		}

		return;
	}

	send(hdl, ":Error Unknown message type");
}

void createMatch(Connection hdl, const std::string &id, int column_count, Color color, bool hidden, bool noskip,
                 const std::string &type) {
	if (id.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") != std::string::npos) {
		send(hdl, ":Error Invalid match ID.");
		return;
	}

	if (column_count < 1 || 8 < column_count) {
		send(hdl, ":Error Invalid column count.");
		return;
	}

	if (matchesByConnection.count(hdl.lock().get()) > 0) {
		send(hdl, ":Error Already in a match.");
		return;
	}

	if (matchesByID.count(id) > 0) {
		send(hdl, ":Error A match with that ID already exists.");
		return;
	}

	std::shared_ptr<Match> match;
	if (type == "human") {
		match = std::make_shared<HumanMatch>(id, hidden, noskip, column_count, color);
	} else if (type == "random") {
		match = AIMatch::create<RandomPlayer>(id, hidden, noskip, column_count, color);
	} else {
		send(hdl, ":Error Invalid match type.");
		return;
	}

	match->host = std::make_unique<HumanPlayer>(color, Player::Role::Host, hdl);
	matchesByID.insert({id, match});
	matchesByConnection.insert({hdl.lock().get(), match});
	send(hdl, ":Joined " + id + " " + (color == Color::White? "white" : "black"));
	std::cout << "Client created " << (match->hidden? "hidden " : "") << "match \e[32m" << id << "\e[39m.\n";
	if (!match->hidden)
		broadcast(":Match " + match->id + " " + (match->isReady()? "closed" : "open"));

	if (match->isReady()) {
		send(hdl, ":Start");
		match->sendAll(":Turn " + colorName(match->currentTurn));
		match->sendBoard();

		match->started = true;
		if (!match->anyCanMove())
			match->end(&(match->board.whitePieces.empty()? match->getBlack() : match->getWhite()));
		do match->roll(); while (!match->canMove());

		if (!match->hidden)
			broadcast(":Match " + match->id + " " + (match->isReady()? "closed" : "open"));
	}
}

void joinMatch(Connection hdl, const std::string &id, bool as_spectator) {
	if (matchesByConnection.count(hdl.lock().get()) > 0) {
		send(hdl, ":Error Already in a match.");
		return;
	}

	if (matchesByID.count(id) == 0) {
		send(hdl, ":Error No match with that ID exists.");
		return;
	}

	std::shared_ptr<Match> match = matchesByID.at(id);

	if (!as_spectator && match->host.has_value() && match->guest.has_value()) {
		send(hdl, ":Error Match is full.");
		return;
	}

	matchesByConnection.insert({hdl.lock().get(), match});
	if (as_spectator)
		send(hdl, ":Joined " + id + " spectator");
	else if (!match->host.has_value())
		send(hdl, ":Joined " + id + " " + colorName(match->hostColor));
	else
		send(hdl, ":Joined " + id + " " + colorName(otherColor(match->hostColor)));

	if (as_spectator) {
		if (match->started)
			send(hdl, ":Start");
	} else if (!match->started)
		match->sendAll(":Start");

	const char *as = "unknown";
	if (as_spectator) {
		match->spectators.push_back(hdl);
		as = "spectator";
	} else if (!match->host.has_value()) {
		match->host = std::make_unique<HumanPlayer>(match->hostColor, Player::Role::Host, hdl);
		as = "host";
	} else if (!match->guest.has_value()) {
		match->guest = std::make_unique<HumanPlayer>(otherColor(match->hostColor), Player::Role::Guest, hdl);
		as = "guest";
	}

	if (!as_spectator)
		send(hdl, ":Start");

	std::cout << "Joining as " << as << ": match is " << (match->isReady()? "ready" : "not ready") << ".\n";
	if (match->isReady())
		match->sendAll(":Turn " + colorName(match->currentTurn));

	match->sendBoard();

	if (!as_spectator && !match->started) {
		match->started = true;
		if (!match->anyCanMove())
			match->end(&(match->board.whitePieces.empty()? match->getBlack() : match->getWhite()));
		do match->roll(); while (!match->canMove());
	} else {
		send(hdl, match->columnMessage());
		for (const std::shared_ptr<Piece> &piece: match->captured)
			send(hdl, match->capturedMessage(piece));
	}

	if (!as_spectator && !match->hidden)
		broadcast(":Match " + match->id + " " + (match->isReady()? "closed" : "open"));

	std::cout << "Client joined match \e[32m" << id << "\e[39m as \e[1m" << as << "\e[22m.\n";
}
