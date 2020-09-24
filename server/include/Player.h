#ifndef CHESS_PLAYER_H_
#define CHESS_PLAYER_H_

#include <string>

struct Player {
	enum class Role {Host, Guest};

	Role role;

	Player(Role role_): role(role_) {}

	virtual ~Player() {}
	virtual void send(const std::string &) = 0;
};

#endif