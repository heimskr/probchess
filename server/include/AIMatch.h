#ifndef CHESS_AIMATCH_H_
#define CHESS_AIMATCH_H_

#include <memory>

#include "Match.h"

class AIMatch: public Match {
	private:
		AIMatch(const std::string &id_, bool hidden_, bool no_skip, int column_count, Color host_color):
			Match(id_, hidden_, no_skip, column_count, host_color) {}

	public:
		template <typename P>
		static std::shared_ptr<AIMatch> create(const std::string &id_, bool hidden_, bool no_skip, int column_count,
		Color host_color) {
			AIMatch *match = new AIMatch(id_, hidden_, no_skip, column_count, host_color);
			match->guest = std::make_unique<P>(otherColor(host_color), Player::Role::Guest);
			return std::shared_ptr<AIMatch>(match);
		}

		~AIMatch() {}

		bool isActive() const override;
		bool hasConnection(Connection) const override;
		bool isReady() const override;
		void afterMove() override;
};

#endif
