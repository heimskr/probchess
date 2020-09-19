class Piece {
	constructor(board_, color_, square_) {
		this.board = board_;
		this.color = color_;
		this.square = square_;
	}

	filterSquares(squares) {
		const out = [];
		console.log("Pre filter:", squares);
		for (const square of squares) {
			if (!square.valid()) {
				console.log("Square invalid:", square);
				continue;
			}
			const piece = this.board.at(square);
			if (piece && piece[1] == this.color) {
				console.log("Piece is same color:", piece);
				continue;
			}
			out.push(square);
		}
		console.log("Post filter:", out);
		return out;
	}

	canMoveTo() {
		return [];
	}

	static create(board, pair, square) {
		if (pair[0] == "bishop") return new Bishop(board, pair[1], square);
		if (pair[0] == "king")   return new King(board, pair[1], square);
		if (pair[0] == "knight") return new Knight(board, pair[1], square);
		if (pair[0] == "pawn")   return new Pawn(board, pair[1], square);
		if (pair[0] == "queen")  return new Queen(board, pair[1], square);
		if (pair[0] == "rook")   return new Rook(board, pair[1], square);
		throw "Unknown piece type: " + pair[0];
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
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // down and to the left
			++next.row;
			--next.column;
			if (!next.valid())
				break;
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // up and to the left
			--next.row;
			--next.column;
			if (!next.valid())
				break;
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // up and to the right
			--next.row;
			++next.column;
			if (!next.valid())
				break;
			out.push(new Square(next));
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
		console.log("col:", this.square.column);
		let next = new Square(this.square.row + (this.color == "black"? 1 : -1), this.square.column);
		if (!this.board.at(next)) {
			out.push(new Square(next));
			if ((this.square.row == 1 && this.color == "black") || (this.square.row == 6 && this.color == "white")) {
				const ahead = new Square(next.row + (this.color == "black"? 1 : -1), next.column);
				if (!this.board.at(ahead))
					out.push(ahead);
			}
		}
		next = new Square(next);
		--next.column;
		if (this.board.at(next))
			out.push(new Square(next));
		next = new Square(next);
		next.column += 2;
		if (this.board.at(next))
			out.push(new Square(next));
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
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // down
			++next.row;
			if (!next.valid())
				break;
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // left
			--next.column;
			if (!next.valid())
				break;
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // right
			++next.column;
			if (!next.valid())
				break;
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // down and to the right
			++next.row;
			++next.column;
			if (!next.valid())
				break;
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // down and to the left
			++next.row;
			--next.column;
			if (!next.valid())
				break;
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // up and to the left
			--next.row;
			--next.column;
			if (!next.valid())
				break;
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // up and to the right
			--next.row;
			++next.column;
			if (!next.valid())
				break;
			out.push(new Square(next));
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
			if (!next.valid()) {
				console.log("up:", next, "isn't valid; breaking");
				break;
			}
			console.log("up: adding", next);
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // down
			++next.row;
			if (!next.valid()) {
				console.log("down:", next, "isn't valid; breaking");
				break;
			}
			console.log("down: adding", next);
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // left
			--next.column;
			if (!next.valid()) {
				console.log("left:", next, "isn't valid; breaking");
				break;
			}
			console.log("left: adding", next);
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		next = new Square(this.square);
		for (let i = 0; i < 7; ++i) { // right
			++next.column;
			if (!next.valid()) {
				console.log("right:", next, "isn't valid; breaking");
				break;
			}
			console.log("right: adding", next);
			out.push(new Square(next));
			if (this.board.at(next))
				break;
		}

		return this.filterSquares(out);
	}
}
