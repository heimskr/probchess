// The bulk of the algorithm is based on code by Tom Murphy VII, aka suckerpinch.
// https://sourceforge.net/p/tom7misc/svn/HEAD/tree/trunk/chess/

#ifndef CHESS_CCCPPLAYER_H_
#define CHESS_CCCPPLAYER_H_

#include <optional>

#include "AIPlayer.h"

class CCCPPlayer: public AIPlayer {
	private:
		struct LabeledMove {
			Move move;
			bool isCheckmated = false, isInCheck = false;
			std::optional<std::shared_ptr<Piece>> captured;
			LabeledMove(const Move &move_): move(move_) {}
			LabeledMove(const Move &move_, bool is_checkmated, bool is_in_check):
				move(move_), isCheckmated(is_checkmated), isInCheck(is_in_check) {}
			LabeledMove(const Move &move_, bool is_checkmated, bool is_in_check, std::shared_ptr<Piece> captured_):
				move(move_), isCheckmated(is_checkmated), isInCheck(is_in_check), captured(captured_) {}
		};

		/** Checks whether the second given move is better than the first given move. */
		bool checkSuperior(Match &, const LabeledMove &check, const LabeledMove &alternative) const;
		
		int centerDistance(int column) const;
		int moveCode(const Move &) const;

	public:
		using AIPlayer::AIPlayer;

		Move chooseMove(Match &, const std::set<int> &columns) override;
};

#endif
