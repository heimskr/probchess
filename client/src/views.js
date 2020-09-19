function renderJoin(j) {
	j.html("");
	const form = $(`<form id="joinForm"></form>`).appendTo(j);
	const div1 = $("<div></div>").appendTo(form);
	const idLabel = $(`<label for="matchID">Match ID:</label>`).appendTo(div1);
	const matchID = $(`<input type="text" autocomplete="off" id="matchID" />`).appendTo(div1);
	const div2 = $("<div></div>").appendTo(form);
	const whiteCheck = $(`<input type="checkbox" checked id="startWhite" />`).appendTo(div2);
	const whiteLabel = $(`<label for="startWhite" />White (create only)</label>`).appendTo(div2);
	const div3 = $("<div></div>").appendTo(form);
	const createButton = $(`<button id="create">Create</button>`).appendTo(div3);
	const joinButton = $(`<button id="join">Join</button>`).appendTo(div3);

	form.on("submit", ev => ev.preventDefault());
	matchID.on("submit", ev => ev.preventDefault());

	createButton.on("click", () => {
		if (matchID.val())
			ws.send(`:Create ${matchID.val()}` + (whiteCheck.is(":checked")? "" : " black"));
	});

	joinButton.on("click", () => {
		if (matchID.val())
			ws.send(":Join " + matchID.val());
	});
}

function renderBoard(board, j) {
	if (!board) {
		alert("Error: unable to render board");
		return;
	}

	board.renderTo(j).find("td").on("click", ev => {
		ev.preventDefault();
		if (state.turn != state.color)
			return;

		const cell = $(ev.target);
		const row = parseInt(cell.attr("data-row")), col = parseInt(cell.attr("data-col"));
		if (state.selected === null) {
			const square = new Square(row, col);
			const pair = board.at(square);
			if (!pair || pair[1] != state.color || col != state.column)
				return;
			state.selected = square;
			cell.addClass("selected-piece");
			const piece = Piece.create(state.board, pair, square);
			$(".square").removeClass("possible");
			for (const move of piece.canMoveTo())
				$(`#cell${move.row}${move.column}`).addClass("possible");
		} else {
			if (row != state.selected.row || col != state.selected.column)
				move(`${state.selected.row}${state.selected.column}`, `${row}${col}`);
			state.selected = null;
			$("td.square").removeClass(["possible", "selected-piece"]);
		}
	});
}
