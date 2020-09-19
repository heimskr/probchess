class Piece {
	constructor(board_, color_, square_) {
		this.board = board_;
		this.color = color_;
		this.square = square_;
	}

	filterSquares(squares) {
		const out = [];
		for (const square of squares) {
			if (!square.valid()) continue;
			const piece = board.at(square);
			if (piece && piece.color == this.color) continue;
			out.push(square);
		}
		return out;
	}

	canMoveTo() {
		return [];
	}
}

class Bishop extends Piece {
	canMoveTo() {
		const out = [];

		let next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // down and to the right
			++next.row;
			++next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // down and to the left
			++next.row;
			--next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // up and to the left
			--next.row;
			--next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // up and to the right
			--next.row;
			++next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		return this.filterSquares(out);
	}
}

class King extends Piece {
	canMoveTo() {
		return filterSquares([
			new Square(this.square.row - 1, this.square.column),
			new Square(this.square.row - 1, this.square.column + 1),
			new Square(this.square.row,     this.square.column + 1),
			new Square(this.square.row + 1, this.square.column + 1),
			new Square(this.square.row + 1, this.square.column),
			new Square(this.square.row + 1, this.square.column - 1),
			new Square(this.square.row,     this.square.column - 1),
			new Square(this.square.row - 1, this.square.column - 1)
		]);
	}
}

class Knight extends Piece {
	canMoveTo() {
		return this.filterSquares([
			new Square(this.square.row - 2, this.square.column - 1),
			new Square(this.square.row - 2, this.square.column + 1),
			new Square(this.square.row - 1, this.square.column + 2),
			new Square(this.square.row + 1, this.square.column + 2),
			new Square(this.square.row + 2, this.square.column + 1),
			new Square(this.square.row + 2, this.square.column - 1),
			new Square(this.square.row + 1, this.square.column - 2),
			new Square(this.square.row - 1, this.square.column - 2)
		]);
	}
}

class Pawn extends Piece {
	canMoveTo() {
		const out = [];
		let next = new Square(this.square.row + (this.color == "black"? 1 : -1), this.square.column);
		if (!this.board.at(next))
			out.push(next);
		--next.column;
		if (this.board.at(next))
			out.push(next);
		next.column += 2;
		if (this.board.at(next))
			out.push(next);
		return this.filterSquares(out);
	}
}

class Queen extends Piece {
	canMoveTo() {
		const out = [];

		let next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // up
			--next.row;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // down
			++next.row;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // left
			--next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // right
			++next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // down and to the right
			++next.row;
			++next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // down and to the left
			++next.row;
			--next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // up and to the left
			--next.row;
			--next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // up and to the right
			--next.row;
			++next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		return this.filterSquares(out);
	}
}

class Rook extends Piece {
	canMoveTo() {
		const out = [];

		let next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // up
			--next.row;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // down
			++next.row;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // left
			--next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // right
			++next.column;
			if (!next.valid())
				break;
			out.push(next);
			if (this.board.at(next))
				break;
		}

		return this.filterSquares(out);
	}
}
