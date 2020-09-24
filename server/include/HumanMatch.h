#ifndef CHESS_HUMANMATCH_H_
#define CHESS_HUMANMATCH_H_

#include "HumanPlayer.h"
#include "Match.h"

class HumanMatch: public Match {
	public:
		using Match::Match;
		~HumanMatch() {}

		bool isActive() const override;
		bool hasConnection(Connection) const override;
		bool hasBoth() const;
		bool sendGuest(const std::string &);
		void sendAll(const std::string &) override;
		bool isReady() const override;
		void afterMove(Player &, Square from, Square to) override;
};

#endif
