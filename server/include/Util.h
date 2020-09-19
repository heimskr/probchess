#ifndef CHESS_UTIL_H_
#define CHESS_UTIL_H_

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

long parseLong(const std::string &, int base = 10);
bool isNumeric(char);
bool isNumeric(const std::string &);
std::vector<std::string> split(const std::string &str, const std::string &delimeter, bool condense = false);

#endif