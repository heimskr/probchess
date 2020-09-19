#ifndef CHESS_MAIN_H_
#define CHESS_MAIN_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "WebSocket.h"

class Match;

extern std::unordered_map<std::string, std::shared_ptr<Match>> matchesByID;
extern std::unordered_map<void *, std::shared_ptr<Match>> matchesByConnection;

#endif
