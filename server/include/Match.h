#ifndef CHESS_MATCH_H_
#define CHESS_MATCH_H_

#include <optional>

#include "WebSocket.h"
#include "Board.h"

class Match {
	public:
		const std::string id;
		std::optional<Connection> host;
		std::optional<Connection> guest;
		Color currentTurn = Color::White;
		Color hostColor;
		Board board;
		std::optional<Color> winner;
		int column = -1;
		bool started = false;

		Match(const std::string &id_, Connection host_, Color host_color);

		void roll();
		void end(Connection *winner);
		void disconnect(Connection);
		bool hasBoth() const;
		void makeMove(Connection, Square from, Square to);
		void checkPawns();
		bool canMove();
		bool sendHost(const std::string &);
		bool sendGuest(const std::string &);
		void sendBoth(const std::string &);
		void sendBoard();
		Connection getWhite() const;
		Connection getBlack() const;
		Connection get(Color) const;
};

#endif