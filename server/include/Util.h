#ifndef CHESS_UTIL_H_
#define CHESS_UTIL_H_

#include <cstdlib>
#include <stdexcept>
#include <string>

long parseLong(const std::string &str, int base = 10) {
	const char *c_str = str.c_str();
	char *end;
	long parsed = strtoll(c_str, &end, base);
	if (c_str + str.length() != end)
		throw std::invalid_argument("Not an integer: \"" + str + "\"");
	return parsed;
}

#endif