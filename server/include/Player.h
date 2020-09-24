#ifndef CHESS_PLAYER_H_
#define CHESS_PLAYER_H_

#include <string>

#include "Color.h"

struct Player {
	enum class Role {Host, Guest};

	Color color;
	Role role;

	Player(Color color_, Role role_): color(color_), role(role_) {}

	virtual ~Player() {}
	virtual void send(const std::string &) = 0;
};

#endif