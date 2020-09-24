#ifndef CHESS_MATCH_H_
#define CHESS_MATCH_H_

#include <list>
#include <optional>
#include <set>

#include "Board.h"
#include "Player.h"
#include "WebSocket.h"

class Match {
	public:
		const std::string id;
		bool hidden, noSkip;
		std::optional<std::unique_ptr<Player>> host;
		Color currentTurn = Color::White;
		Color hostColor;
		Board board;
		std::optional<Color> winner;
		int columnCount;
		std::set<int> columns;
		bool started = false;
		std::list<std::shared_ptr<Piece>> captured;
		std::list<Connection> spectators;

		Match(const std::string &id_, bool hidden_, bool no_skip, int column_count, Color host_color);

		virtual ~Match() = 0;

		void roll();
		virtual void end(Player *winner);
		virtual void disconnect(Player &) = 0;
		virtual bool isActive() const = 0;
		virtual bool hasConnection(Connection) const = 0;
		virtual Player & currentPlayer();
		void makeMove(Player &, Square from, Square to);
		void checkPawns();
		bool canMove() const;
		bool anyCanMove() const;
		bool sendHost(const std::string &);
		virtual void sendAll(const std::string &);
		virtual bool isReady() const = 0;
		void sendSpectators(const std::string &);
		void sendCaptured(Player &, std::shared_ptr<Piece>);
		void sendBoard();
		std::string columnMessage();
};

#endif