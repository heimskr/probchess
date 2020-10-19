#ifndef CHESS_MAIN_H_
#define CHESS_MAIN_H_

#include <memory>
#include <string>
#include <sys/wait.h>
#include <unordered_map>
#include <unordered_set>

#include "WebSocket.h"

class Match;

extern std::unordered_map<std::string, std::shared_ptr<Match>> matchesByID;
extern std::unordered_map<void *, std::shared_ptr<Match>> matchesByConnection;
extern std::unordered_set<pid_t> segfault_set;

void echo_handler(Connection, asio_server::message_ptr);
void open_handler(Connection);
void close_handler(Connection);
void sigint_handler(int);
void sigchld_handler(int);

void createMatch(Connection, const std::string &id, int column_count, Color, bool hidden, bool noskip,
                 const std::string &type);
void joinMatch(Connection, const std::string &id, bool as_spectator);
void leaveMatch(Connection);

#endif
