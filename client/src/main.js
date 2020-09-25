const WS_HOST = window.location.host || "localhost";

let ws;

const state = {
	view: null,
	connected: false,
	turn: null,
	joined: false,
	color: null,
	matchID: null,
	board: null,
	started: false,
	selected: null,
	over: false,
	columns: [],
	spectator: false,
	matches: [],
};

function connect() {
	ws = new WebSocket(`ws://${WS_HOST}:38555`);

	ws.onopen = ev => {
		state.connected = true;
		renderJoin($("main"));
	};

	ws.onmessage = ev => {
		if (ev.data.length < 2 || ev.data[0] != ":") {
			console.error("Invalid message:", ev.data);
			return;
		}

		const split = ev.data.split(" ");
		const verb = split[0].substr(1);
		const rest = split.slice(1).join(" ");

		if (verb == "Joined") {
			state.matchID = split[1];
			state.color = split[2];
			state.turn = "white";
			state.board = new Chessboard();
			state.board.placePieces();
			renderBoard(state.board, $("main"));
			$("#status").text("Waiting for another player to join.");
			if (split[2] == "black")
				state.board.invert();
			state.spectator = split[2] == "spectator";
			return;
		}

		if (verb == "Columns") {
			state.columns = rest.split(" ").map(x => parseInt(x));
			$("td.square").removeClass("active-column");
			if (state.columns.length < 8)
				for (const column of state.columns)
					$(`.col${column}`).addClass("active-column");
			return;
		}

		if (verb == "Board") { // :Board <encoded>
			state.board.updatePieces(rest);
			return;
		}

		if (verb == "Start") { // :Start
			state.started = true;
			$("#status").text("Connected.");
			return;
		}

		if (verb == "Error") { // :Error <text...>
			$("#error").addClass("visible").text(rest);
			setTimeout(() => $("#error").removeClass("visible"), 2000);
			return;
		}

		if (verb == "Turn") {
			if (state.spectator) {
				if (state.started)
					$("#status").text(`It's ${rest}'s turn.`);
			} else
				$("#status").text(rest == state.color? "It's your turn." : "It's the opponent's turn.");
			state.turn = rest;
			if (rest != state.color) {
				state.selected = null;
				$("td.square").removeClass("selected-piece");
			}
			return;
		}

		if (verb == "Win") {
			if (state.spectator)
				$("#status").text(`Winner: ${rest}`);
			else
				$("#status").text("You win!");
			state.over = true;
			return;
		}

		if (verb == "Lose") {
			$("#status").text("You lose :(");
			state.over = true;
			return;
		}

		if (verb == "End") {
			$("#status").text("Match ended unexpectedly.");
			state.over = true;
			return;
		}

		if (verb == "MoveMade") {
			$(".last-move").removeClass("last-move");
			$(`#cell${split[1]}, #cell${split[2]}`).addClass("last-move");
			return;
		}

		if (verb == "Capture") {
			const span = $("<span></span>").html(pieceMap[split[2]]).addClass(split[3]);
			if (split[3] == state.color)
				$("#table-container #before").append(span);
			else
				$("#table-container #after").append(span);
			return;
		}

		if (verb == "ClearMatches") {
			state.matches = [];
			if (state.view == "join")
				$("#matches").text("");
			return;
		}

		if (verb == "Match") {
			state.matches[split[1]] = split[2];
			if (state.view == "join")
				renderMatchTable($("#matches"));
			return;
		}

		if (verb == "RemoveMatch") {
			delete state.matches[split[1]];
			if (state.view == "join")
				renderMatchTable($("#matches"));
			return;
		}

		if (verb == "FEN") {
			console.log("FEN:", rest);
			return;
		}

		console.warn("Unknown verb:", verb);
	};

	ws.onclose = ev => {
		console.warn("Closing connection.");
		$("main").text("");
		$("#error").text("The server is offline.").addClass("visible");
	};

	ws.onerror = ev => {
		$("#error").text("The server doesn't seem to be online.").addClass("visible");
	};
}

function send(...a) {
	ws.send(...a);
}

function create(id, black) {
	send(black? `:Create ${id} black` : `:Create ${id}`);
}

function join(id) {
	send(":Join " + id);
}

function move(from, to) {
	ws.send(":Move " + from + " " + to);
}

function show(pos) {
	ws.send(":Show " + pos);
}

connect();
