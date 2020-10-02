#include <cerrno>
#include <condition_variable>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>

#include "Match.h"
#include "StockfishPlayer.h"
#include "Util.h"
#include "fdstream.h"
#include "main.h"

Move StockfishPlayer::chooseMove(Match &match, const std::set<int> &columns) {
	std::list<Move> possibilities = getPossibleMoves(match, columns);

	// First, see if the player can capture the king. This tends to cause Stockfish to segfault for unknown reasons.
	// A manual check for checkmate can prevent us having to use Stockfish in that case.
	for (const Move &move: possibilities) {
		std::shared_ptr<Piece> piece = match.board.at(move.to);
		if (piece && piece->color != match.currentTurn && piece->getType() == Piece::Type::King) {
			std::cerr << "Returning checkmate.\n";
			return move;
		}
	}

	int child_to_parent[2], parent_to_child[2];

	if (pipe(child_to_parent) == -1)
		std::cerr << "pipe(c2p) failed: " << strerror(errno) << "\n";

	if (pipe(parent_to_child) == -1)
		std::cerr << "pipe(c2p) failed: " << strerror(errno) << "\n";

	pid_t child_pid = fork();

	if (child_pid == 0) { // child
		dup2(parent_to_child[0], STDIN_FILENO);
		dup2(child_to_parent[1], STDOUT_FILENO);
		close(parent_to_child[1]);
		close(child_to_parent[0]);
		execlp("stockfish", "stockfish", 0);
	} else if (child_pid > 0) { // parent
		std::condition_variable condition;
		std::mutex mutex;
		ifdstream in(child_to_parent[0]);
		ofdstream out(parent_to_child[1]);

		std::string line;
		std::getline(in, line);

		out << "ucinewgame\n";
		out << "position fen " << match.board.toFEN(match.currentTurn) << "\n";
		std::cout << "position fen " << match.board.toFEN(match.currentTurn) << "\n";

		out << "go searchmoves";
		std::cout << "go searchmoves";
		for (const Move &move: possibilities) {
			out << " " << move.pseudoalgebraic();
			std::cout << " " << move.pseudoalgebraic();
		}
		out << "\n";
		std::cout << "\n";

		Move bestmove {{0, 0}, {0, 0}};
		bool done = false;

		std::thread segfault_watch = std::thread([&]() {
			while (!done) {
				std::unique_lock<std::mutex> lock(mutex);
				condition.wait_for(lock, std::chrono::milliseconds(321), [&] { return done; });
				int stat_loc;
				waitpid(child_pid, &stat_loc, WNOHANG);

				const bool was_segfault = WTERMSIG(stat_loc) == SIGSEGV;
				const bool in_segfault_set = segfault_set.count(child_pid) != 0;
				if (in_segfault_set)
					segfault_set.erase(child_pid);

				if (was_segfault || in_segfault_set) {
					close(child_to_parent[0]);
					bestmove = *std::next(possibilities.begin(), rand() % possibilities.size());
					break;
				}
			}
		});

		while (std::getline(in, line)) {
			// std::cout << "[line: \"" << line << "\"]\n";
			if (line.substr(0, 9) == "bestmove ") {
				std::string beststring;
				for (int i = 9; line[i] != ' '; ++i)
					beststring.push_back(line[i]);
				// std::cout << "Beststring: [" << beststring << "]\n";
				if (beststring.find("none") != std::string::npos) {
					bestmove = *std::next(possibilities.begin(), rand() % possibilities.size());
					warn() << "Stockfish couldn't choose a move. Choosing \e[1m" << bestmove.pseudoalgebraic()
					       << "\e[22m\n";
				} else {
					bestmove = beststring;
				}

				break;
			}
		}

		out << "quit\n";
		close(parent_to_child[0]);
		close(child_to_parent[1]);
		{
			std::unique_lock<std::mutex> lock(mutex);
			done = true;
			lock.unlock();
			condition.notify_all();
			segfault_watch.join();
		}
		return bestmove;
	} else std::cerr << "\e[31mfork() failed\e[39m\n";

	match.sendAll(":Error Stockfish failed to find a move.");
	return *std::next(possibilities.begin(), rand() % possibilities.size());
}
