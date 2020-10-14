#ifndef CHESS_MATCH_H_
#define CHESS_MATCH_H_

#include <list>
#include <optional>
#include <set>

#include "Board.h"
#include "Move.h"
#include "Player.h"
#include "WebSocket.h"

class Match {
	public:
		const std::string id;
		bool hidden, noSkip;
		std::optional<std::unique_ptr<Player>> host, guest;
		Color currentTurn = Color::White;
		Color hostColor;
		Board board;
		std::optional<Color> winnerColor;
		int columnCount;
		std::set<int> columns;
		bool started = false, over = false;
		std::list<std::shared_ptr<Piece>> captured;
		std::list<Connection> spectators;

		Match(const std::string &id_, bool hidden_, bool no_skip, int column_count, Color host_color);
		Match(const Match &) = delete;
		Match(Match &&) = delete;

		Match & operator=(const Match &) = delete;
		Match & operator=(Match &&) = delete;

		virtual ~Match() {}

		void roll();
		virtual void end(Player *winner);
		virtual void disconnect(Connection);
		Player & getPlayer(Connection);
		virtual bool isActive() const = 0;
		virtual bool hasConnection(Connection) const = 0;
		Player & currentPlayer();
		void makeMove(Player &, const Move &);
		virtual void afterMove() = 0;
		std::list<Square> checkPawns();
		bool canMove() const;
		bool anyCanMove() const;
		bool sendHost(const std::string &);
		bool sendGuest(const std::string &);
		virtual void sendBoth(const std::string &);
		virtual void sendAll(const std::string &);
		virtual bool isReady() const = 0;
		void sendSpectators(const std::string &);
		void sendBoard();
		void invertTurn();
		std::string capturedMessage(std::shared_ptr<Piece>);
		std::string columnMessage();
		Player & getWhite();
		Player & getBlack();
		Player & get(Color);
};

#endif