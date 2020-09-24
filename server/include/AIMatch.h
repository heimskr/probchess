// #ifndef CHESS_AIMATCH_H_
// #define CHESS_AIMATCH_H_

// #include "AIPlayer.h"
// #include "Match.h"

// class AIMatch: public Match {
// 	public:
// 		std::AIPlayer guest;

// 		using Match::Match;
// 		~AIMatch() {}

// 		void end(Player *winner) override;
// 		void disconnect(Player &) override;
// 		bool isActive() const override;
// 		bool hasConnection(Connection) const override;
// 		Player & currentPlayer() override;
// 		bool hasBoth() const;
// 		bool sendGuest(const std::string &);
// 		void sendBoth(const std::string &);
// 		void sendAll(const std::string &) override;
// 		bool isReady() const override;
// 		Player & getWhite();
// 		Player & getBlack();
// 		Player & get(Color);
// };

// #endif
