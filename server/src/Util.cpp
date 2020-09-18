#include "Util.h"

long parseLong(const std::string &str, int base) {
	const char *c_str = str.c_str();
	char *end;
	long parsed = strtoll(c_str, &end, base);
	if (c_str + str.length() != end)
		throw std::invalid_argument("Not an integer: \"" + str + "\"");
	return parsed;
}

bool isNumeric(char ch) {
	return '0' <= ch && ch <= '9';
}

bool isNumeric(const std::string &str) {
	if (str.empty())
		return false;
	for (char ch: str)
		if (!isNumeric(ch))
			return false;
	return true;
}
