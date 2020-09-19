class Square {
	constructor(row_, column_) {
		if (row_ instanceof Square) {
			this.row = row_.row;
			this.column = row_.column;
		} else {
			this.row = row_;
			this.column = column_;
		}
	}

	valid() {
		return 0 <= this.row && this.row < 8 && 0 <= this.column && this.column < 8;
	}
}

class Chessboard {
	constructor(str) {
		this.rows = [];
		for (let i = 0; i < 8; ++i)
			this.rows.push([]);
		if (str) {
			for (let i = 0; i < str.length; i += 2) {
				const chunk = str.substr(i, 2);
				const row = Math.floor(i / 16);
				const col = (i / 2) % 8;
				this.rows[row][col] = [abbreviations[chunk[0]], {B: "black", W: "white"}[chunk[1]]];
			}
		}
	}

	at(square, col) {
		if (typeof col != "undefined") {
			if (!this.rows[square])
				return undefined;
			return this.rows[square][col];
		}

		if (!this.rows[square.row])
			return undefined;
		return this.rows[square.row][square.column];
	}

	placePieces() {
		this.rows[0][0] = ["rook",   "black"];
		this.rows[0][1] = ["knight", "black"];
		this.rows[0][2] = ["bishop", "black"];
		this.rows[0][3] = ["queen",  "black"];
		this.rows[0][4] = ["king",   "black"];
		this.rows[0][5] = ["bishop", "black"];
		this.rows[0][6] = ["knight", "black"];
		this.rows[0][7] = ["rook",   "black"];
		this.rows[7][0] = ["rook",   "white"];
		this.rows[7][1] = ["knight", "white"];
		this.rows[7][2] = ["bishop", "white"];
		this.rows[7][3] = ["queen",  "white"];
		this.rows[7][4] = ["king",   "white"];
		this.rows[7][5] = ["bishop", "white"];
		this.rows[7][6] = ["knight", "white"];
		this.rows[7][7] = ["rook",   "white"];
		for (let i = 0; i < 8; ++i) {
			this.rows[1][i] = ["pawn", "black"];
			this.rows[6][i] = ["pawn", "white"];
		}
	}

	renderTo(j) {
		j.html("");
		let table = $("<table></table>").appendTo(j);
		let tbody = $("<tbody></tbody>").appendTo(table);
		for (let row = 0; row < 8; ++row) {
			let tr = $("<tr></tr>").appendTo(tbody);
			for (let col = 0; col < 8; ++col) {
				let td = $("<td></td>").addClass("square").addClass((row % 2 == col % 2)? "white" : "black").appendTo(tr);
				const item = this.rows[row][col];
				if (item) {
					td.addClass(item[1] + "-piece").text(pieceMap[item[0]]);
				}
			}
		}
	}
}