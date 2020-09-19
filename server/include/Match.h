#ifndef CHESS_MATCH_H_
#define CHESS_MATCH_H_

#include <optional>

#include "WebSocket.h"
#include "Board.h"

class Match {
	public:
		const std::string id;
		Connection host;
		std::optional<Connection> guest;
		Color currentTurn = Color::White;
		Color hostColor;
		Board board;
		std::optional<Color> winner;
		int column = -1;

		Match(const std::string &id_, Connection host_, Color host_color);

		bool active() const;
		void roll();
		void end(Connection *winner);

		void makeMove(Connection, Square from, Square to);
		void checkPawns();
		bool canMove();
		void sendBoth(const std::string &);
		void sendBoard();
		Connection getWhite() const;
		Connection getBlack() const;
		Connection get(Color) const;
};

#endif