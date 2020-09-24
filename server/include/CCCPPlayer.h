// The bulk of the algorithm is based on code by Tom Murphy VII, aka suckerpinch.
// https://sourceforge.net/p/tom7misc/svn/HEAD/tree/trunk/chess/

#ifndef CHESS_CCCPPLAYER_H_
#define CHESS_CCCPPLAYER_H_

#include <optional>

#include "AIPlayer.h"

class CCCPPlayer: public AIPlayer {
	private:
		struct LabeledMove: public Move {
			bool isCheckmate = false, isCheck = false;
			std::optional<Square> captured;
			LabeledMove(): Move({0, 0}, {0, 0}) {}
			LabeledMove(const Move &move, bool is_checkmate, bool is_check):
				Move(move), isCheckmate(is_checkmate), isCheck(is_check) {}
			LabeledMove(const Move &move, bool is_checkmate, bool is_check, const Square &captured_):
				Move(move), isCheckmate(is_checkmate), isCheck(is_check), captured(captured_) {}
		};

		/** Checks whether the second given move is better than the first given move. */
		bool checkSuperior(const Move &check, const Move &alternative) const;
		
		int centerDistance(int column) const;
		int moveCode(const Move &) const;

	public:
		using AIPlayer::AIPlayer;

		Move chooseMove(const Match &, const std::set<int> &columns) override;
};

#endif
