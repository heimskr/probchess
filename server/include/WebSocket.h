#ifndef CHESS_WEBSOCKET_H_
#define CHESS_WEBSOCKET_H_

#include <list>

#include "websocketpp/server.hpp"
#include "websocketpp/config/asio_no_tls.hpp"

using asio_server = websocketpp::server<websocketpp::config::asio>;
using Connection  = websocketpp::connection_hdl;

extern asio_server *server;
extern std::list<Connection> connections;

void send(Connection, const std::string &);
void broadcast(const std::string &);

#endif
