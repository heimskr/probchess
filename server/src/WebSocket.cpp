#include "WebSocket.h"

void send(websocketpp::connection_hdl connection, const std::string &message) {
	server->send(connection, message, websocketpp::frame::opcode::text);
}
