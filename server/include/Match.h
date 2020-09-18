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
		std::optional<Color> winner;

		Match(websocketpp::connection_hdl host_, Color host_color);

		bool active() const;

		void makeMove(websocketpp::connection_hdl, Square from, Square to);
		websocketpp::connection_hdl getWhite() const;
		websocketpp::connection_hdl getBlack() const;
		websocketpp::connection_hdl get(Color) const;
};

#endif