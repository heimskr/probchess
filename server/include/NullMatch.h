#ifndef CHESS_NULLMATCH_H_
#define CHESS_NULLMATCH_H_

#include <memory>

#include "Match.h"

class NullMatch: public Match {
	public:
		NullMatch(const std::string &id_, bool hidden_, bool no_skip, int column_count, Color host_color):
			Match(id_, hidden_, no_skip, column_count, host_color) {}

		~NullMatch() {}

		bool isActive() const override;
		bool hasConnection(Connection) const override;
		bool isReady() const override;
		void afterMove() override;
};

#endif
