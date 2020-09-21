function renderJoin(j) {
	j.html("");
	state.view = null;
	const form = $(`<form id="joinForm"></form>`).appendTo(j);
	const div1 = $("<div></div>").appendTo(form);
	const idLabel = $(`<label for="matchID">Match ID:</label>`).appendTo(div1);
	const matchID = $(`<input type="text" autocomplete="off" id="matchID" />`).appendTo(div1);
	const checkTable = $("<table></table>").appendTo(form);
	const whiteRow = $("<tr></tr>").appendTo(checkTable);
	const whiteCheck = $(`<input type="checkbox" checked id="startWhite" />`).appendTo($("<td></td>").appendTo(whiteRow));
	const whiteLabel = $(`<label for="startWhite" />White (if creating)</label>`).appendTo($("<td></td>").appendTo(whiteRow));
	const hiddenRow = $("<tr></tr>").appendTo(checkTable);
	const hiddenCheck = $(`<input type="checkbox" id="hidden" />`).appendTo($("<td></td>").appendTo(hiddenRow));
	const hiddenLabel = $(`<label for="hidden">Hidden</label>`).appendTo($("<td></td>").appendTo(hiddenRow));
	const div2 = $("<div></div>").appendTo(form);
	const joinButton = $(`<button id="join">Join or Create</button>`).appendTo(div2);

	send(":GetMatches");

	form.on("submit", ev => ev.preventDefault());
	matchID.on("submit", ev => ev.preventDefault());
	joinButton.on("click", () => {
		if (matchID.val())
			ws.send(`:CreateOrJoin ${matchID.val()} ${whiteCheck.is(":checked")? "white" : "black"} ${hiddenCheck.is(":checked")? "hidden" : "public"}`);
	});

	renderMatchTable($(`<table id="matches"></table>`).appendTo(j));
	state.view = "join";
}

function renderMatchTable(table) {
	table.text("");
	for (const key of Object.keys(state.matches).sort()) {
		const open = state.matches[key] == "open";
		const row = $("<tr></tr>").attr("id", "match-" + key).appendTo(table);
		$("<button>Join</button>").attr({disabled: !open}).appendTo($("<td></td>").appendTo(row)).on("click", ev =>
			send(`:Join ${key}`));
		$("<button>Spectate</button>").appendTo($("<td></td>").appendTo(row)).on("click", ev =>
			send(`:Spectate ${key}`));
		$("<td></td>").text(key).appendTo(row);
		$("<td></td>").text(open? "Open" : "Closed").css({color: open? "green" : "red"}).appendTo(row);
	}
}

function renderBoard(board, j) {
	if (!board) {
		alert("Error: unable to render board");
		return;
	}

	state.view = null;
	const container = $(`<div id="table-container"></div>`).appendTo(j.text(""));
	board.renderTo(container).find("td").on("click", ev => {
		ev.preventDefault();
		if (state.turn != state.color || state.spectator)
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
	$(`<div id="before"></div>`).prependTo(container);
	$(`<div id="after"></div>`).appendTo(container);
	state.view = "board";
}
