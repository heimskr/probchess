#include <functional>
#include <iostream>
#include <sstream>

#include "Board.h"
#include "piece/all.h"

std::shared_ptr<Piece> Board::at(int row, int column) const {
	if (row < 0 || 7 < row || column < 0 || 7 < column)
		return nullptr;
	return pieces[row][column];
}

std::shared_ptr<Piece> Board::at(Square square) const {
	return at(square.row, square.column);
}

void Board::placePieces() {
	for (int row = 0; row < 8; ++row)
		for (int column = 0; column < 8; ++column)
			pieces[row][column].reset();
	
	blackPieces.push_back(set<Rook>(Color::Black,   0, 0));
	blackPieces.push_back(set<Knight>(Color::Black, 0, 1));
	blackPieces.push_back(set<Bishop>(Color::Black, 0, 2));
	blackPieces.push_back(set<Queen>(Color::Black,  0, 3));
	blackPieces.push_back(set<King>(Color::Black,   0, 4));
	blackPieces.push_back(set<Bishop>(Color::Black, 0, 5));
	blackPieces.push_back(set<Knight>(Color::Black, 0, 6));
	blackPieces.push_back(set<Rook>(Color::Black,   0, 7));

	whitePieces.push_back(set<Rook>(Color::White,   7, 0));
	whitePieces.push_back(set<Knight>(Color::White, 7, 1));
	whitePieces.push_back(set<Bishop>(Color::White, 7, 2));
	whitePieces.push_back(set<Queen>(Color::White,  7, 3));
	whitePieces.push_back(set<King>(Color::White,   7, 4));
	whitePieces.push_back(set<Bishop>(Color::White, 7, 5));
	whitePieces.push_back(set<Knight>(Color::White, 7, 6));
	whitePieces.push_back(set<Rook>(Color::White,   7, 7));

	for (int column = 0; column < 8; ++column) {
		blackPieces.push_back(set<Pawn>(Color::Black, 1, column));
		whitePieces.push_back(set<Pawn>(Color::White, 6, column));
	}
}

void Board::erase(std::shared_ptr<Piece> piece) {
	if (!piece)
		return;
	if (piece->color == Color::Black)
		blackPieces.remove(piece);
	else
		whitePieces.remove(piece);
	pieces[piece->square.row][piece->square.column].reset();
}

void Board::erase(int row, int column) {
	erase(at(row, column));
}

bool Board::move(std::shared_ptr<Piece> piece, int new_row, int new_column) {
	if (!piece)
		return false;
	pieces[new_row][new_column] = piece;
	pieces[piece->square.row][piece->square.column] = nullptr;
	piece->square.row = new_row;
	piece->square.column = new_column;
	return true;
}

bool Board::move(std::shared_ptr<Piece> piece, Square square) {
	return move(piece, square.row, square.column);
}

std::string Board::toString(std::shared_ptr<Piece> show_moves) const {
	const std::list<Square> moves = show_moves? show_moves->canMoveTo() : std::list<Square>();

	std::function<const char * (int, int)> bg = [&](int row, int column) {
		for (const Square &square: moves)
			if (square.row == row && square.column == column)
				return "\e[43m";
		return row % 2 == column % 2? "\e[47m" : "\e[100m";
	};

	static std::function<const char * (std::shared_ptr<Piece>)> pieceColor = [](std::shared_ptr<Piece> piece) {
		return piece->color == Color::White? "\e[37m" : "\e[30m";
	};

	std::stringstream out;

	for (int row = 0; row < 8; ++row) {
		for (int column = 0; column < 8; ++column)
			out << bg(row, column) << row << column << "   \e[0m";
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

Board::operator std::string() const {
	return toString(nullptr);
}

std::ostream & operator<<(std::ostream &os, const Board &board) {
	return os << std::string(board);
}
