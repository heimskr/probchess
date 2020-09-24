#ifndef CHESS_HUMANMATCH_H_
#define CHESS_HUMANMATCH_H_

#include "HumanPlayer.h"
#include "Match.h"

class HumanMatch: public Match {
	public:
		std::optional<std::unique_ptr<HumanPlayer>> guest;

		using Match::Match;
		~HumanMatch() {}

		void end(Player *winner) override;
		void disconnect(Player &) override;
		bool isActive() const override;
		bool hasConnection(Connection) const override;
		Player & currentPlayer() override;
		bool hasBoth() const;
		bool sendGuest(const std::string &);
		void sendBoth(const std::string &);
		void sendAll(const std::string &) override;
		bool isReady() const override;
		Player & getWhite();
		Player & getBlack();
		Player & get(Color);
};

#endif
