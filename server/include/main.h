#ifndef CHESS_MAIN_H_
#define CHESS_MAIN_H_

#include <memory>
#include <string>
#include <unordered_map>

class Match;

extern std::unordered_map<std::string, std::shared_ptr<Match>> matches;

#endif