#ifndef CHESS_MATCH_H_
#define CHESS_MATCH_H_

#include <optional>

#include "websocketpp/server.hpp"
#include "websocketpp/connection.hpp"
#include "Board.h"

class Match {
	public:
		websocketpp::connection_hdl host;
		std::optional<websocketpp::connection_hdl> guest;
		Color currentTurn = Color::White;
		Color hostColor;
		Board board;

		Match(websocketpp::connection_hdl host_, Color host_color);

		bool active() const;

		void makeMove(websocketpp::connection_hdl, Square from, Square to);
};

#endif