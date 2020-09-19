const WS_HOST = "localhost";

let ws;

const pieceMap = {bishop: "♝", king: "♚", knight: "♞", pawn: "♟", queen: "♛", rook: "♜"};
const abbreviations = {b: "bishop", k: "king", h: "knight", p: "pawn", q: "queen", r: "rook"};

const state = {
	connected: false,
	usersTurn: false,
	joined: false,
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
