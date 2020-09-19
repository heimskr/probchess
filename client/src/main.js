const WS_HOST = "localhost";

let ws;

const pieceMap = {bishop: "♝", king: "♚", knight: "♞", pawn: "♟", queen: "♛", rook: "♜"};
const abbreviations = {b: "bishop", k: "king", h: "knight", p: "pawn", q: "queen", r: "rook"};

const state = {
	connected: false,
	usersTurn: false,
	joined: false,
	color: null,
	matchID: null,
	board: null,
};

function connect() {
	ws = new WebSocket(`ws://${WS_HOST}:38555`);

	ws.onopen = ev => {
		console.log("Opened socket.");
		state.connected = true;
		renderJoin($("main"));
	};

	ws.onmessage = ev => {
		console.info(ev.data);
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
			state.board = new Chessboard();
			state.board.placePieces();
			renderBoard(state.board, $("main"));
			if (split[2] == "black")
				state.board.invert();
			return;
		}

		if (verb == "Column") {
			$("#extraStyle").text(`.col${rest}.black { background: #444; } .col${rest}.white { background: #bbb; }`);
			$("#column").text("Column: " + (1 + parseInt(rest)));
			return;
		}

		if (verb == "Board") {
			state.board.updatePieces(rest);
			return;
		}
	};
	
	ws.onclose = ev => {
		console.warn("Closing connection.");
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
