#include <iostream>
#include <cstdlib>
#include <signal.h>

#include "websocketpp/server.hpp"
#include "websocketpp/config/asio_no_tls.hpp"

#include "Board.h"
#include "Util.h"
#include "Square.h"
#include "piece/all.h"

using asio_server = websocketpp::server<websocketpp::config::asio>;

void echo_handler(websocketpp::connection_hdl, asio_server::message_ptr);
void signal_handler(int signum);

asio_server *server;

int main(int argc, char **argv) {
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
	server->set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload ^ websocketpp::log::alevel::frame_header);
	server->init_asio();
	server->set_message_handler(std::bind(&echo_handler, std::placeholders::_1, std::placeholders::_2));
	server->listen(port);
	server->start_accept();
	server->run();
}

void signal_handler(int signum) {
	server->stop();
	server->stop_listening();
	delete server;
}

void echo_handler(websocketpp::connection_hdl hdl, asio_server::message_ptr msg_ptr) {
	const std::string &msg = msg_ptr->get_payload();
	if (msg.empty() || msg[0] != ':') {
		server->send(hdl, "Invalid message.", msg_ptr->get_opcode());
		return;
	}
}
