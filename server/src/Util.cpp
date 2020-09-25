#include <iostream>

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

std::vector<std::string> split(const std::string &str, const std::string &delimeter, bool condense) {
	if (str.empty())
		return {};

	size_t next = str.find(delimeter);
	if (next == std::string::npos)
		return {str};

	std::vector<std::string> out;
	const size_t delimeter_length = delimeter.size();
	size_t last = 0;

	out.push_back(str.substr(0, next));

	while (next != std::string::npos) {
		last = next;
		next = str.find(delimeter, last + delimeter_length);
		std::string sub = str.substr(last + delimeter_length, next - last - delimeter_length);
		if (!sub.empty() || !condense)
			out.push_back(std::move(sub));
	}

	return out;
}

std::ostream & warn() {
	return std::cerr << "\e[2m[\e[22;33m!\e[39;2m]\e[22m ";
}
