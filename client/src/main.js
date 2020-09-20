const WS_HOST = "192.168.0.8";

let ws;

const state = {
	connected: false,
	turn: null,
	joined: false,
	color: null,
	matchID: null,
	board: null,
	started: false,
	selected: null,
	over: false,
	column: null,
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
			return;
		}

		if (verb == "Column") {
			state.column = parseInt(rest);
			$("td.square").removeClass(["active-column", "left-column", "right-column"]);
			$(`.col${rest}`).addClass("active-column");
			if (state.column != 0)
				$(`.col${state.column - 1}`).addClass("left-column");
			if (state.column != 7)
				$(`.col${state.column + 1}`).addClass("right-column");
			return;
		}

		if (verb == "Board") {
			state.board.updatePieces(rest);
			return;
		}

		if (verb == "Start") {
			state.started = true;
			$("#status").text("Connected.");
			return;
		}

		if (verb == "Error") {
			$("#error").addClass("visible").text(rest);
			setTimeout(() => $("#error").removeClass("visible"), 2000);
			return;
		}

		if (verb == "Turn") {
			$("#status").text(rest == state.color? "It's your turn." : "It's the opponent's turn.");
			state.turn = rest;
			if (rest != state.color) {
				state.selected = null;
				$("td.square").removeClass("selected-piece");
			}
			return;
		}

		if (verb == "Win") {
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
	};
	
	ws.onclose = ev => {
		console.warn("Closing connection.");
		$("main").text("");
		$("#error").text("The server has shut down.").addClass("visible");
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
