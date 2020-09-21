#include "WebSocket.h"

std::list<Connection> connections;

void send(websocketpp::connection_hdl connection, const std::string &message) {
	server->send(connection, message, websocketpp::frame::opcode::text);
}

void broadcast(const std::string &message) {
	for (Connection connection: connections)
		send(connection, message);
}
