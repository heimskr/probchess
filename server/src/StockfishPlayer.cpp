#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include "Match.h"
#include "StockfishPlayer.h"
#include "fdstream.h"

Move StockfishPlayer::chooseMove(Match &match, const std::set<int> &columns) {
	std::list<Move> possibilities;
	for (const int column: columns) {
		for (int row = 0; row < match.board.height; ++row) {
			std::shared_ptr<Piece> piece = match.board.at(row, column);
			if (!piece || piece->color != color)
				continue;
			for (const Square &square: piece->canMoveTo())
				possibilities.push_back({piece->square, square});
		}
	}

	if (possibilities.empty())
		throw std::runtime_error("StockfishBot has no moves to select from.");
	
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
		ifdstream in(child_to_parent[0]);
		ofdstream out(parent_to_child[1]);

		out << "uci\n";
		std::string line;
		std::getline(in, line);
		std::cout << "[line: \"" << line << "\"]\n";
		std::getline(in, line);
		std::cout << "[line: \"" << line << "\"]\n";

		close(parent_to_child[0]);
		close(child_to_parent[1]);
	} else {
		std::cerr << "\e[31mfork() failed\e[39m\n";
	}

	for(;;);

	return {{0, 0}, {0, 0}};
}
