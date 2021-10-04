function renderJoin(j) {
	j.html("");
	state.view = null;
	const form = $(`<form id="joinForm"></form>`).appendTo(j);
	const div1 = $("<div></div>").appendTo(form);
	const idLabel = $(`<label for="matchID">Match ID:</label>`).appendTo(div1);
	const matchID = $(`<input type="text" autocomplete="off" id="matchID" />`).appendTo(div1);
	const div2 = $("<div></div>").appendTo(form);
	const columnSelect = $(`<select id="columnCount"></select>`).appendTo(div2);
	for (let i = 1; i <= 8; ++i) columnSelect.append($(`<option value="${i}">${i} column${i == 1? "" : "s"}</option>`));
	const div3 = $("<div></div>").appendTo(form);
	const typeSelect = $(`<select id="type"></select>`).appendTo(div3);
	$(`<option value="human">Human vs. Human</option>`).appendTo(typeSelect);
	$(`<option value="random">Human vs. RandomBot</option>`).appendTo(typeSelect);
	$(`<option value="cccp">Human vs. CCCPBot</option>`).appendTo(typeSelect);
	$(`<option value="cccp2">Human vs. CCCPBot2</option>`).appendTo(typeSelect);
	$(`<option value="null">Human vs. No One</option>`).appendTo(typeSelect);
	$(`<option value="stockfish">Human vs. Stockfish</option>`).appendTo(typeSelect);
	const checkTable = $("<table></table>").appendTo(form);
	const whiteRow = $("<tr></tr>").appendTo(checkTable);
	const whiteCheck = $(`<input type="checkbox" checked id="startWhite" />`).appendTo($("<td></td>").appendTo(whiteRow));
	const whiteLabel = $(`<label for="startWhite" />White (if creating)</label>`).appendTo($("<td></td>").appendTo(whiteRow));
	const hiddenRow = $("<tr></tr>").appendTo(checkTable);
	const hiddenCheck = $(`<input type="checkbox" id="hidden" />`).appendTo($("<td></td>").appendTo(hiddenRow));
	const hiddenLabel = $(`<label for="hidden">Hidden</label>`).appendTo($("<td></td>").appendTo(hiddenRow));
	const noskipRow = $("<tr></tr>").appendTo(checkTable);
	const noskipCheck = $(`<input type="checkbox" id="noskip" />`).appendTo($("<td></td>").appendTo(noskipRow));
	const noskipLabel = $(`<label for="noskip">Skipn't</label>`).appendTo($("<td></td>").appendTo(noskipRow));
	const div4 = $("<div></div>").appendTo(form);
	const joinButton = $(`<button id="join">Join or Create</button>`).appendTo(div4);

	send(":GetMatches");

	form.on("submit", ev => ev.preventDefault());
	matchID.on("submit", ev => ev.preventDefault());
	joinButton.on("click", () => {
		if (matchID.val())
			ws.send(`:CreateOrJoin ${matchID.val()} ${columnSelect.val()} ${whiteCheck.is(":checked")? "white" : "black"} ` +
				`${hiddenCheck.is(":checked")? "hidden" : "public"} ${noskipCheck.is(":checked")? "noskip" : "skip"} ` +
				typeSelect.val());
	});

	$(`<div id="rules"></div>`).appendTo(j).append($("<div></div>").html(`
		<h3>Rules</h3>
		Probability chess is like normal chess, except you can move only from the column that's randomly selected for
		you. When you're in check, you're not required to immediately get out of check. This is because you won't always
		be given a chance to get yourself out of check. However, the other player won't necessarily be able to checkmate
		you right away either.
		<h4>Skipping</h4>
		In the standard configuration, if the column chosen doesn't contain any moveable pieces, the player's turn is
		skipped. With skipn't mode, rerolls will be made until a column with moveable pieces is chosen.
		<h4>Probability</h4>
		The probabilities are uneven due to the dice rolling mechanic. In order from column 1 to column 8, they are
		<span class="hidden">
			11.11%<span class="sep"></span>
			11.11%<span class="sep"></span>
			11.11%<span class="sep"></span>
			11.11%<span class="sep"></span>
			11.11%<span class="sep"></span>
			13.89%<span class="sep"></span>
			16.67%<span class="sep"></span>
			13.89%
		</span> (hover to view).
		Equivalently, the chances out of 36 are
		<span class="hidden">
			4<span class="sep"></span>
			4<span class="sep"></span>
			4<span class="sep"></span>
			4<span class="sep"></span>
			4<span class="sep"></span>
			5<span class="sep"></span>
			6<span class="sep"></span>
			5
		</span>.
		Note that the columns are always chosen from white's perspective; if you're playing as black, the probabilities
		will be in the reverse order.
	`));

	renderMatchTable($(`<table id="matches"></table>`).appendTo(j));
	if (!window.navigator.userAgent.match(/iPhone/))
		$("#matchID").focus();
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
			if (!pair || pair[1] != state.color || state.columns.indexOf(col) == -1)
				return;
			state.selected = square;
			cell.addClass("selected-piece");
			const piece = Piece.create(state.board, pair, square);
			$(".square").removeClass(["possible", "only-move"]);
			for (const move of piece.canMoveTo())
				$(`#cell${move.row}${move.column}`).addClass("possible");
			forceUpdate();
		} else {
			if (row != state.selected.row || col != state.selected.column)
			move(`${state.selected.row}${state.selected.column}`, `${row}${col}`);
			state.selected = null;
			$("td.square").removeClass(["possible", "selected-piece", "only-move"]);
			forceUpdate();
		}
	});
	$(`<div id="before"></div>`).prependTo(container);
	$(`<div id="after"></div>`).appendTo(container);
	state.view = "board";
}
