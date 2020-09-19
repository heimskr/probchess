#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <time.h>

#include "Board.h"
#include "ChessError.h"
#include "Match.h"
#include "Square.h"
#include "Util.h"
#include "WebSocket.h"
#include "main.h"
#include "piece/all.h"

void echo_handler(websocketpp::connection_hdl, asio_server::message_ptr);
void signal_handler(int signum);

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

	Board board;
	board.placePieces();
	std::cout << std::string(board);

	server = new asio_server;
	server->set_error_channels(websocketpp::log::elevel::all);
	server->set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload ^ websocketpp::log::alevel::frame_header);
	server->init_asio();
	server->set_message_handler(std::bind(&echo_handler, std::placeholders::_1, std::placeholders::_2));
	server->listen(port);
	server->start_accept();
	server->run();
}

void signal_handler(int signum) {
	std::cout << "Goodbye.\n";
	server->stop();
	server->stop_listening();
	delete server;
}

void echo_handler(websocketpp::connection_hdl hdl, asio_server::message_ptr msg_ptr) {
	const std::string &msg = msg_ptr->get_payload();
	if (msg.empty() || msg[0] != ':') {
		send(hdl, ":Error Invalid message");
		return;
	}

	const std::vector<std::string> words = split(msg.substr(1), " ");
	const std::string &verb = words[0];

	if (verb == "Create") {
		if (words.size() != 2 && words.size() != 3) {
			send(hdl, ":Error Invalid message");
			return;
		}

		if (words[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_")
		    != std::string::npos) {
			send(hdl, ":Error Invalid match ID");
			return;
		}

		if (matchesByConnection.count(hdl.lock().get()) > 0) {
			send(hdl, ":Error Already in a match");
			return;
		}

		if (matchesByID.count(words[1]) > 0) {
			send(hdl, ":Error A match with that ID already exists");
			return;
		}

		Color color = words.size() == 3 && words[2] == "black"? Color::Black : Color::White;
		std::shared_ptr<Match> match = std::make_shared<Match>(words[1], hdl, color);
		matchesByID.insert({words[1], match});
		matchesByConnection.insert({hdl.lock().get(), match});
		send(hdl, ":MatchCreated " + words[1] + " " + (color == Color::White? "white" : "black"));
		return;
	}

	if (verb == "Join") {
		if (words.size() != 2) {
			send(hdl, ":Error Invalid message");
			return;
		}

		if (matchesByConnection.count(hdl.lock().get()) > 0) {
			send(hdl, ":Error Already in a match");
			return;
		}

		if (matchesByID.count(words[1]) == 0) {
			send(hdl, ":Error No match with that ID exists");
			return;
		}

		std::shared_ptr<Match> match = matchesByID.at(words[1]);
		if (match->guest.has_value()) {
			send(hdl, ":Error Match is full");
			return;
		}

		matchesByConnection.insert({hdl.lock().get(), match});
		match->guest = hdl;
		send(hdl, ":Joined " + words[1] + " " + (match->hostColor == Color::White? "black" : "white"));
		match->roll();
		return;
	}

	if (verb == "Move") {
		if (words.size() != 3) {
			send(hdl, ":Error Invalid message");
			return;
		}

		for (const std::string &str: {words[1], words[2]})
			if (str.size() != 2 || str.find_first_not_of("01234567") != std::string::npos) {
				send(hdl, ":Error Invalid message");
				return;
			}

		if (matchesByConnection.count(hdl.lock().get()) == 0) {
			send(hdl, ":Error Not in a match");
			return;
		}

		std::shared_ptr<Match> match = matchesByConnection.at(hdl.lock().get());

		Square from {words[1][0] - '0', words[1][1] - '0'};
		Square to   {words[2][0] - '0', words[2][1] - '0'};

		try {
			match->makeMove(hdl, from, to);
		} catch (ChessError &err) {
			send(hdl, ":Error " + std::string(err.what()));
			return;
		}

		send(hdl, ":MoveMade");
		std::cout << std::string(match->board);
		return;
	}

	if (verb == "Show") {
		if (words.size() != 2 || words[1].size() != 2 || words[1].find_first_not_of("01234567") != std::string::npos) {
			send(hdl, ":Error Invalid message");
			return;
		}

		std::shared_ptr<Match> match = matchesByConnection.at(hdl.lock().get());
		if (!match) {
			send(hdl, ":Error Not in a match");
			return;
		}

		std::cout << match->board.toString(match->board.at(words[1][0] - '0', words[1][1] - '0')) << "\n";
		return;
	}

	send(hdl, ":Error Unknown message type");
}
