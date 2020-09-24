#ifndef CHESS_RandomMATCH_H_
#define CHESS_RandomMATCH_H_

#include "RandomPlayer.h"
#include "Match.h"

class RandomMatch: public Match {
	public:
		RandomMatch(const std::string &id_, bool hidden_, bool no_skip, int column_count, Color host_color);
		~RandomMatch() {}

		bool isActive() const override;
		bool hasConnection(Connection) const override;
		bool isReady() const override;
		void afterMove() override;
};

#endif
