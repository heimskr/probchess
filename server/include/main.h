#ifndef CHESS_MAIN_H_
#define CHESS_MAIN_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "WebSocket.h"

class Match;

extern std::unordered_map<std::string, std::shared_ptr<Match>> matchesByID;
extern std::unordered_map<void *, std::shared_ptr<Match>> matchesByConnection;

void echo_handler(Connection, asio_server::message_ptr);
void open_handler(Connection);
void close_handler(Connection);
void signal_handler(int);

void createMatch(Connection, const std::string &id, Color, bool hidden);
void joinMatch(Connection, const std::string &id, bool as_spectator);

#endif
