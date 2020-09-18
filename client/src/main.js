const WS_HOST = "localhost";

let ws;

function connect() {
	ws = new WebSocket(`ws://${WS_HOST}:38555`);

	ws.onmessage = ev => {
		console.log("Data:", ev.data);
	};
	
	ws.onclose = ev => {
		console.warn("Closing connection.");
	};
}
