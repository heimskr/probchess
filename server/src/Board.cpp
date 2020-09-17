#include <functional>
#include <sstream>

#include "Board.h"
#include "piece/all.h"

std::shared_ptr<Piece> Board::at(int row, int column) {
	return pieces[row][column];
}

void Board::placePieces() {
	for (int row = 0; row < 8; ++row)
		for (int column = 0; column < 8; ++column)
			pieces[row][column].reset();
	
	for (const std::pair<Color, int> &pair: std::initializer_list<std::pair<Color, int>> {{Color::Black, 0}, {Color::White, 7}}) {
		set<Rook>(pair.first,   pair.second, 0);
		set<Knight>(pair.first, pair.second, 1);
		set<Bishop>(pair.first, pair.second, 2);
		set<Queen>(pair.first,  pair.second, 3);
		set<King>(pair.first,   pair.second, 4);
		set<Bishop>(pair.first, pair.second, 5);
		set<Knight>(pair.first, pair.second, 6);
		set<Rook>(pair.first,   pair.second, 7);
	}

	for (int column = 0; column < 8; ++column) {
		set<Pawn>(Color::Black, 1, column);
		set<Pawn>(Color::White, 6, column);
	}
}

Board::operator std::string() {
	static std::function<const char * (int, int)> bg = [](int row, int column) {
		return row % 2 == column % 2? "\e[47m" : "\e[100m";
	};

	static std::function<const char * (std::shared_ptr<Piece>)> pieceColor = [](std::shared_ptr<Piece> piece) {
		return piece->color == Color::White? "\e[37m" : "\e[30m";
	};

	std::stringstream out;

	for (int row = 0; row < 8; ++row) {
		for (int column = 0; column < 8; ++column)
			out << bg(row, column) << "     \e[0m";
		out << "\n";
		for (int column = 0; column < 8; ++column) {
			std::shared_ptr<Piece> piece = at(row, column);
			out << bg(row, column) << "  ";
			if (piece)
				out << pieceColor(piece) << piece->toString(Color::Black);
			else
				out << " ";
			out << "  \e[0m";
		}
		out << "\n";
		for (int column = 0; column < 8; ++column)
			out << bg(row, column) << "     \e[0m";
		out << "\n";
	}

	return out.str();
}
