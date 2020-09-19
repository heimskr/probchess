const WS_HOST = "localhost";

let ws;

const pieceMap = {bishop: "♝", king: "♚", knight: "♞", pawn: "♟", queen: "♛", rook: "♜"};
const abbreviations = {b: "bishop", k: "king", h: "knight", p: "pawn", q: "queen", r: "rook"};

function connect() {
	ws = new WebSocket(`ws://${WS_HOST}:38555`);

	ws.onmessage = ev => {
		console.log("Data:", ev.data);
	};
	
	ws.onclose = ev => {
		console.warn("Closing connection.");
	};
}

function send(...a) {
	ws.send(...a);
}
