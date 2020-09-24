#ifndef CHESS_AIMATCH_H_
#define CHESS_AIMATCH_H_

#include "AIPlayer.h"
#include "Match.h"

class AIMatch: public Match {
	public:
		AIMatch(const std::string &id_, bool hidden_, bool no_skip, int column_count, Color host_color);
		~AIMatch() {}

		bool isActive() const override;
		bool hasConnection(Connection) const override;
		bool isReady() const override;
		void afterMove() override;
};

#endif
