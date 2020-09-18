#ifndef CHESS_WEBSOCKET_H_
#define CHESS_WEBSOCKET_H_

#include "websocketpp/server.hpp"
#include "websocketpp/config/asio_no_tls.hpp"

using asio_server = websocketpp::server<websocketpp::config::asio>;
extern asio_server *server;
using Connection = websocketpp::connection_hdl;

void send(Connection, const std::string &);


#endif
