// The bulk of the algorithm is based on code by Tom Murphy VII, aka suckerpinch.
// https://sourceforge.net/p/tom7misc/svn/HEAD/tree/trunk/chess/

#pragma once

#include "CCCPPlayer.h"

class CCCP2Player: public CCCPPlayer {
	protected:
		/** Checks whether the second given move is better than the first given move. */
		bool checkSuperior(Match &, const CCCPPlayer::LabeledMove &check, const CCCPPlayer::LabeledMove &alternative)
			const override;

	public:
		using CCCPPlayer::CCCPPlayer;
};
