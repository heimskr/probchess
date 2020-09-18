#ifndef CHESS_UTIL_H_
#define CHESS_UTIL_H_

#include <cstdlib>
#include <stdexcept>
#include <string>

long parseLong(const std::string &, int base = 10);
bool isNumeric(char);
bool isNumeric(const std::string &);

#endif